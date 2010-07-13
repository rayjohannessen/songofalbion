#include "stdafx.h"

#include "Unit.h"
#include "Globals.h"
#include "AnimationsManager.h"
#include "AbilitiesManager.h"
#include "Map.h"
#include "AbilitiesFunctions.h"
#include "AbilityObjectBase.h"
#include "CombatSkill.h"
#include "NonCombatSkill.h"
#include "ObjectManager.h"
#include "Pathfinding.h"
using namespace Pathfinding;
using namespace UnitDefines;

CUnit::~CUnit()
{
	if (m_nNumAnims)
	{
		UnitAnims::iterator iter, end;
		for (iter = m_mAnimations.begin(), end = m_mAnimations.end(); iter != end; ++iter)
			delete (*iter).second;
		m_mAnimations.clear();
	}
}
CUnit::CUnit() : 
	CObject(),
	m_nUnitType(-1),
	m_nMaxVitality(0),
	m_nVitality(0),
	m_nAtt(0),
	m_nDef(0),
	m_nMaxStamina(0),
	m_nStamina(0),
	m_nMaxMP(0),
	m_nMagPts(0),
	m_nRange(1),
	m_nNumAnims(0),
	m_strCurrAnim("NONE"),
	m_ptOriginalScrnOS(0.0f, 0.0f)
{
}

CUnit::CUnit(CUnit& unit)
{
	ClearAnims();
	CObject::CopyAll((CObject)unit);
	DeepCopyAll(unit);
}

CUnit::CUnit( int nUnitType, int type, point& coord, point& sPos, 
			 string name, const char* faction, int factionID)
			 : CObject(type, coord, sPos, name, faction, factionID)
{
	// most units' source rects should be the same size, use default size for now (w=128, h=128)
	m_nUnitType = nUnitType;
	SetImageID(0);

	ObjAnimations anims = Globals::g_pAnimManager->GetAnimations(m_strName);
	m_nNumAnims = anims.size();
	for (int i = 0; i < m_nNumAnims; ++i)
		m_mAnimations[anims[i].Name] = new CAnimation(DIR_E, anims[i]);
	m_strCurrAnim = anims[0].Name;

	switch (nUnitType)
	{
	case UNIT_UMKNIGHT:	// unmounted knight
		{
			// TODO::stats needs to be easily changed, with scripting such as lua...plan for this in the future
			m_nVitality	= m_nMaxVitality = 100;
			m_nStamina	= m_nMaxStamina = 3;
			m_nMagPts	= m_nMaxMP = 10;
			m_nAtt		= 12;
			m_nDef		= 10;
			m_nRange	= 1;

			// adjust screen point and size for selection rect
			m_ptOriginalScrnOS.Offset(30.0f, 15.0f);	// stores the original offset for use later when moving or scrolling
			m_ptSize.Offset(30, 55);

			m_rSelectionRect = rectf(pointf(m_ptScreenPos.x + m_ptOriginalScrnOS.x, m_ptScreenPos.y + m_ptOriginalScrnOS.y), m_ptSize);
		}
		break;
	}
	m_Timer.ResetTimer();
}

CUnit& CUnit::operator= (CUnit& unit)
{
	CObject::CopyAll((CObject&)unit);
	DeepCopyAll(unit);
	return *this;
}

void CUnit::DeepCopyAll( CUnit &unit ) 
{
	ClearAnims();
	m_nUnitType  		= unit.GetUnitType();
	m_nMaxVitality	 	= unit.GetMaxVit();
	m_nVitality	 		= unit.GetVitality();
	m_nAtt		 		= unit.GetAttk();
	m_nDef		 		= unit.GetDef();
	m_nMaxStamina		= unit.GetMaxStamina();
	m_nStamina			= unit.GetStamina();
	m_nMaxMP			= unit.GetMaxMagPts();
	m_nMagPts			= unit.GetMP();
	m_nRange			= unit.GetRange();
	m_nNumAnims			= unit.GetNumAnims();
	m_strCurrAnim	  	= unit.GetCurrAnimString();
	m_ptOriginalScrnOS	= unit.GetOrigScOS();
	m_Timer				= unit.m_Timer;

	UnitAnims::iterator iter, end;
	for (iter = unit.GetAnims().begin(), end = unit.GetAnims().end(); iter != end; ++iter)
		m_mAnimations[(*iter).second->GetName()] = new CAnimation(*((*iter).second));
}

void CUnit::Update(double fTimeStep, const pointf* moveAmt)
{
	if (moveAmt) // account for scrolling of the map
		CObject::Update(fTimeStep, moveAmt);

	if (m_Timer.Update())
		Globals::g_pObjManager->RemoveObj(Globals::GetPlayerByFactionID(GetFactionID()), this);

	if (IsAnimRun(m_strCurrAnim))
	{
		if (!m_ptMoveToScreenPos.IsWithinRange(m_ptScreenPos))
		{
			if (moveAmt)
				m_ptMoveToScreenPos += *moveAmt;
			// need to update position, move towards target position
			pointf newPos, dir, moveAmt;
			newPos	= m_ptScreenPos;
			dir		= (m_ptMoveToScreenPos - newPos);
			dir.Normalize();
			moveAmt = dir * (float)fTimeStep * MOVE_SPEED;
			newPos  = newPos + moveAmt;
			m_rSelectionRect += moveAmt;
	
			SetScreenPosWithoutOS(newPos);	// set screen position
		}
		else
		{
			m_mAnimations[m_strCurrAnim]->Stop();
			ChangeAnim(gAnimNames[AT_ATTACK]);
			m_ptScreenPos = m_ptMoveToScreenPos;
			Globals::g_pMap->ToggleMapFlagOff(MF_MOVING);	// TODO:: determine a better way to handle this
		}
	}
	// update current animation
	if(m_mAnimations[m_strCurrAnim]->Update(fTimeStep))
	{
		// an anim has finished playing...maybe do something now...
		if (IsAnimAttack(m_strCurrAnim))
		{
			//m_pCurrAbility->SetToUpdate(false);	// TODO:: complete ability apart from animation
		}
		else if (IsAnimHit(m_strCurrAnim))
		{
			ChangeAnim(gAnimNames[AT_ATTACK]);
		}
		else if (IsAnimDying(m_strCurrAnim))
		{
			// TODO:: add unit disappearing effect
			m_Timer.StartTimer(1.0f);	// delay removing this object for a bit
		}
	}
}

// draws the unit at its current frame
void CUnit::Render( const rect& viewPort )
{
	if (TestOnScreen(viewPort)) // don't draw if it's not onscreen
	{
		CObject::Render(viewPort);
		Globals::g_pTM->DrawWithZSort(
			m_mAnimations[m_strCurrAnim]->GetImageID(), 
			(int)m_ptScreenPos.x, (int)m_ptScreenPos.y, m_fZDepth,
			m_fScaleX, m_fScaleY, &m_mAnimations[m_strCurrAnim]->GetSourceRect(), 0.0f, 0.0f, 0.0f, m_dwColor);

// 		if (m_pCurrAbility && m_pCurrAbility->IsUpdating())
// 			m_pCurrAbility->Render();
	}
}

// performs a map move, only if it has enough movement points remaining
// returns true if move was made
bool CUnit::Move(point& coord, point& screenPos, int cost, eAnimationDirections facing)
{
	if (cost <= m_nStamina)
	{
		m_nStamina -= cost;
		if (m_nStamina < 0)
			m_nStamina = 0;
		SetCoord(coord);	// set grid coord
		m_ptMoveToScreenPos = (pointf)screenPos + (pointf)m_ptOffset;
		m_strCurrAnim = gAnimNames[AT_RUN];
		SetFacing(facing);
		m_mAnimations[m_strCurrAnim]->Play(1, true);
		return true;
	}
	return false;
}

bool CUnit::FaceTarget(const point& targetPt, int* newDir /*= NULL*/, bool setFacing /*= true*/)
{
	TargetDirPair pair;
	pair.first	= m_ptCoord.x - targetPt.x;
	pair.second	= m_ptCoord.y - targetPt.y;

	// determine how many spaces away the target coord is

	// TODO:: allow for unit to turn when WITHIN RANGE, instead of one tile away always
	// only turn precise if the unit is a tile away
	Path path;
	A_Star(Globals::g_pMap->GetL1Tiles()[m_ptCoord.y * Globals::g_pMap->GetNumCols() + m_ptCoord.x], 
		   Globals::g_pMap->GetL1Tiles()[targetPt.y * Globals::g_pMap->GetNumCols() + targetPt.x], path, 
		   this, Globals::g_pMap->GetL1Tiles(), Globals::g_pMap->GetL2Tiles(), Globals::g_pMap->GetNumColsRows());

	if (path.size())
	{
		// fetch direction to face from TargetDirMap that matches the correct "pair" of x and y differences
		eAnimationDirections dir = Globals::g_CoordToDir[m_ptCoord.y & 1][pair]; 
		if (setFacing)
			SetFacing(dir);	
		if (newDir)
			*newDir = dir;
		//return true;
	}
	return false;
}

bool CUnit::MouseInRect(point& mousePt)
{
	return m_rSelectionRect.IsPointInRect(pointf(mousePt));
}

void CUnit::SetScrnPos(point& sPos)
{
	CObject::SetScrnPos(sPos);
	m_rSelectionRect.left   = (m_ptScreenPos.x + m_ptOriginalScrnOS.x);
	m_rSelectionRect.top    = (m_ptScreenPos.y + m_ptOriginalScrnOS.y);
	m_rSelectionRect.right  = (m_rSelectionRect.left + m_ptSize.x);
	m_rSelectionRect.bottom = (m_rSelectionRect.top  + m_ptSize.y);
}

void CUnit::SetScreenPosWithoutOS(const pointf& pt)
{
	m_ptScreenPos.x = pt.x; m_ptScreenPos.y = pt.y; 
	m_rSelectionRect.left   = (m_ptScreenPos.x + m_ptOriginalScrnOS.x);
	m_rSelectionRect.top    = (m_ptScreenPos.y + m_ptOriginalScrnOS.y);
	m_rSelectionRect.right  = (m_rSelectionRect.left + m_ptSize.x);
	m_rSelectionRect.bottom = (m_rSelectionRect.top  + m_ptSize.y);
}

void CUnit::ChangeAnim(const string& animName)
{
	eAnimationDirections facing = m_mAnimations[m_strCurrAnim]->CurrDir();
	m_strCurrAnim = animName;
	SetFacing(facing);
}

void CUnit::ClearAnims()
{
	UnitAnims::iterator iter, end;
	if (m_nNumAnims)
	{
		for (iter = m_mAnimations.begin(), end = m_mAnimations.end(); iter != end; ++iter)
			delete (*iter).second;
		m_mAnimations.clear();
	}
}

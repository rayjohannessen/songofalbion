#include "stdafx.h"

#include "Object.h"
#include "BitmapFont.h"
#include "Globals.h"
#include "AbilityObjectBase.h"
#include "AbilitiesManager.h"
#include "UIQuickBar.h"
#include "HUD.h"
#include "NonCombatSkill.h"
#include "CombatSkill.h"
#include "Pathfinding.h"
#include "Map.h"
using namespace Pathfinding;

CObject::CObject() :
	m_nImageID(-1),
	m_bHovered(false),
	m_strName("BOB"),
	m_pCurrAttackAbility(NULL),
	m_pCurrDefenseAbility(NULL)
{ }

CObject::CObject(int type, point coord, point scrnPos, string name, const char* faction, int factionID) :
	m_nImageID(-1),
	m_nType(type),
	m_rSrc(0, 128, 0, 128),
	m_strName(name),
	m_ptScreenPos(scrnPos),	// gives us the top-left screen pos (used to make the rect for selection)
	m_ptCoord(coord),
	m_ptSize(0, 0),
	m_szFaction(faction),
	m_nFactionID(factionID),
	m_bHovered(false),
	m_fScaleX(1.0f),
	m_fScaleY(1.0f),
	m_dwColor(WHITE),
	m_pCurrAttackAbility(NULL),
	m_pCurrDefenseAbility(NULL),
	m_fZDepth(DEPTH_OBJECT)
{
	m_rSelectionRect = rectf(m_ptScreenPos, point(128, 128));

	// set up the offset so the object is centered on the tiles
	switch (type)
	{
	case OBJ_CITY:
		{
			m_ptOffset = point(-5, 0);
			m_fZDepth = DEPTH_CITY;
		}break;		
	case OBJ_UNIT:
		{
			m_ptOffset = point(20, -20);
			m_fScaleX = m_fScaleY = 0.7f;
			m_fZDepth = DEPTH_UNIT;
		}break;
	case OBJ_BUILDING:
		{
			m_ptOffset = point(-5, 0);
			m_fZDepth = DEPTH_BUILDING;
		}break;		
	}
	m_eDefaultAbilityType = BN_COMBAT_SKILLS;
	
	Globals::g_pAbilitiesManager->GetUnlockedStartingAbilities(m_strName, m_mAbilities);
	ClearQBSlotORSlots();
	AbilitiesIter iter, end; unsigned i;
	for (iter = m_mAbilities[m_eDefaultAbilityType].begin(), end = m_mAbilities[m_eDefaultAbilityType].end(), i = 0; i < NUM_QB_SLOTS && iter != end; ++iter, ++i)
		m_arrQBSlots[i] = (*iter)->GetQBObj();
	m_pCurrAttackAbility = m_pCurrDefenseAbility = (CCombatSkill*)m_arrQBSlots[0]->Ability;
}
// copy c-tor
CObject::CObject(CObject& obj)
{
	CopyAll(obj);
}

CObject::~CObject()
{
}

void CObject::Update(double fTimeStep, const pointf* moveAmt)
{
	m_ptScreenPos += (*moveAmt);
	m_rSelectionRect.top	+= moveAmt->y;
	m_rSelectionRect.bottom += moveAmt->y;
	m_rSelectionRect.left	+= moveAmt->x;
	m_rSelectionRect.right	+= moveAmt->x;
}

void CObject::Render(const rect& viewPort )
{
	// units draw themselves...
	if (m_nType != OBJ_UNIT)																
		Globals::g_pTM->DrawWithZSort(GetImageID(), (int)m_ptScreenPos.x, (int)m_ptScreenPos.y, m_fZDepth, m_fScaleX, m_fScaleY, GetSrc(), 0.0f, 0.0f, 0.0f, m_dwColor);	
	
	if (m_bHovered)
	{
		if (m_nType == OBJ_UNIT)
			Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->FactionImages()[m_nFactionID], (int)m_ptScreenPos.x-5, (int)m_ptScreenPos.y + 30, m_fZDepth, 0.9f, 0.9f);
		else
			Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->FactionImages()[m_nFactionID], (int)m_ptScreenPos.x-5, (int)m_ptScreenPos.y + 30, m_fZDepth, 0.9f, 0.9f);

		// TODO::add health bars on hover
	}
}

bool CObject::TestOnScreen(const rect& viewPort)
{
	rect v = viewPort;	// our current pos rect
	return v.IsRectIntersecting(rect(m_ptScreenPos, m_ptSize));
}

bool CObject::IsOnQuickBar(const CQuickBarObject& qbObj)
{
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
		if (m_arrQBSlots[i] == &qbObj)
			return true;
	return false;
}

CCombatSkill* CObject::GetCurrDefaultAbility(eButtonName& defType)	
{ 
	defType = m_eDefaultAbilityType;
	if (m_eDefaultAbilityType == BN_COMBAT_SKILLS) 
		return m_pCurrAttackAbility; 
	else 
		return m_pCurrDefenseAbility; 
}

void CObject::FindPathToTarget(const point& tileCoord, Path &path)
{
	path.clear();
	A_Star(Globals::g_pMap->GetL1Tiles()[m_ptCoord.y * Globals::g_pMap->GetNumCols() + m_ptCoord.x], 
		Globals::g_pMap->GetL1Tiles()[tileCoord.y * Globals::g_pMap->GetNumCols() + tileCoord.x], path, 
		/*this,*/ Globals::g_pMap->GetL1Tiles(), Globals::g_pMap->GetL2Tiles(), Globals::g_pMap->GetNumColsRows());
}

//////////////////////////////////////////////////////////////////////////
// PRIVATE
void CObject::ClearQBSlotORSlots()
{
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
		m_arrQBSlots[i] = NULL;
}
void CObject::CopyAll( CObject &obj )
{
	m_nImageID		= obj.GetImageID();
	m_nType			= obj.GetType();
	m_rSrc			= *obj.GetSrc();
	m_strName		= obj.GetName();
	m_ptScreenPos	= obj.GetSPos();
	m_ptCoord		= obj.GetCoord();
	m_ptSize		= obj.GetSize();
	m_szFaction 	= obj.GetFaction();
	m_nFactionID	= obj.GetFactionID();
	m_bHovered  	= obj.GetIsHovered();
	m_rSelectionRect= obj.GetSelectionRect();
	m_ptOffset		= obj.GetOS();
	m_fScaleX		= obj.GetScaleX();
	m_fScaleY		= obj.GetScaleY();
	m_fZDepth		= obj.GetZDepth();
	m_dwColor		= obj.GetColor();
	m_pCurrAttackAbility	= obj.GetCurrAttackAbility();
	m_pCurrDefenseAbility	= obj.GetCurrDefenseAbility();
	m_eDefaultAbilityType	= obj.GetDefAbilityType();
	m_mAbilities	= obj.GetAbilitiesMap();
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
		m_arrQBSlots[i] = obj.GetQBSlots()[i];
}
//	END PRIVATE
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Building.h"
#include "Globals.h"
#include "Assets.h"

CBuilding::CBuilding() :
	CObject(), 
	m_nBuildingType(0),
	m_nMaxHP(0),
	m_nHitPts(0),
	m_uiGarrisonCapacity(0)
{
	SetPostBaseCTORVars();
}

CBuilding::CBuilding(eBuildingType buildingType, int type, point coord, point sPos, string name, const char* faction, int factionID)
	: CObject(type, coord, sPos, name, faction, factionID, gBuildingNames[buildingType])
{
	m_nBuildingType = buildingType;
	switch (buildingType)
	{
	case BT_CASTLE:
		SetImageID(Globals::g_pAssets->GetMapasts()->Castle());
		SetSrcRect(rect(0, 64, 128, 256));
		m_ptSize = point(128, 64);
		m_nMaxHP = m_nHitPts = 1000;
		m_uiGarrisonCapacity = 20;
		break;
	case BT_TOWER:
		break;
	}
	SetPostBaseCTORVars();
}

void CBuilding::Update(double dTimeStep, const pointf* moveAmt/*double xOS, double yOS*/)
{
	if (moveAmt)
	{
		CObject::Update(dTimeStep, moveAmt);
	}
}

void CBuilding::Render( const rect& viewPort )
{
	if (TestOnScreen(viewPort))
	{
		Globals::g_pTM->Render(GetImageID(), (int)m_ptScreenPos.x, (int)m_ptScreenPos.y, m_fZDepth, m_fScaleX, m_fScaleY, GetSrc(), 0.0f, 0.0f, 0.0f, m_dwColor);	

		CObject::Render(viewPort);
	}
}

void CBuilding::SetPostBaseCTORVars()
{
	m_ptOffset = point(-5, 0);
	m_fZDepth = DEPTH_BUILDING;
	m_eDefaultAbilityType = BN_NONCOMBAT_SKILLS;
	SetStartingAbilities();
}
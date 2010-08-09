#include "stdafx.h"

#include "Building.h"
#include "Globals.h"
#include "Assets.h"

CBuilding::CBuilding() 
:	CObject(), 
	m_nBuildingType(0),
	m_nMaxHP(0),
	m_nHitPts(0),
	m_nCapacity(0)
{

}

CBuilding::CBuilding(int buildingType, int type, point coord, point sPos, 
					 string name, const char* faction, int factionID)
: CObject(type, coord, sPos, name, faction, factionID)
{
	m_nBuildingType = buildingType;
	switch (buildingType)
	{
	case BLDG_CASTLE:
		SetImageID(Globals::g_pAssets->GetMapasts()->Castle());
		SetSrcRect(rect(0, 64, 128, 256));
		m_ptSize = point(128, 64);
		m_nMaxHP = m_nHitPts = 1000;
		m_nCapacity = 20;
		break;
	case BLDG_TOWER:
		break;
	}
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
		CObject::Render(viewPort);
	}
}
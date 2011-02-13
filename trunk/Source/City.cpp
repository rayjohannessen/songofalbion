#include "stdafx.h"

#include "City.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "UICityInfo.h"

CCity::CCity() : 
	CObject(), 
	m_nCityType(0),
	m_nSize(0),
	m_pCityInfo(NULL)
{
	SetPostBaseCTORVars();
}

CCity::~CCity()
{ 
	SAFE_DELETE(m_pCityInfo);
}

CCity::CCity(int type, int cityType, int size, point coord, point sPos, string name, const char* faction, int factionID)
: CObject(type, coord, sPos, name, faction, factionID, CityDefines::gCityTypeNames[size-1])
{
	SetImageID(Globals::g_pAssets->GetMapasts()->City());
	SetSrcRect(rect(cityType, cityType+CITY_HEIGHT, (size-1)*CITY_WIDTH, (size-1)*CITY_WIDTH+CITY_WIDTH));	

	SetPostBaseCTORVars();

	// each city starts as the smallest size (located on the image at x = 0)
	m_nSize = size;
	m_nCityType = cityType;
	m_ptSize = point(CITY_WIDTH, CITY_HEIGHT);

	m_pCityInfo = new UICityInfo(this);
}

void CCity::Update(double dTimeStep, const pointf* moveAmt)
{
	if (moveAmt)	// for scrolling
	{
		CObject::Update(dTimeStep, moveAmt);
	}

	if (m_bDisplayInfo)
		m_pCityInfo->Update(dTimeStep);
}

void CCity::Input(const POINT& mouse)
{
	if (m_bDisplayInfo)
		m_pCityInfo->Input(mouse);
}

void CCity::Render( const rect& viewPort )
{
	rect pos(m_ptScreenPos, m_ptSize);
	if (TestOnScreen(viewPort))
	{
		Globals::g_pTM->Render(GetImageID(), (int)m_ptScreenPos.x, (int)m_ptScreenPos.y, m_fZDepth, m_fScaleX, m_fScaleY, GetSrc(), 0.0f, 0.0f, 0.0f, m_dwColor);	

		CObject::Render(viewPort);
		// draw the city names over the cities...
		Globals::g_pBitMapFont->DrawString(m_strName.c_str(), (int)m_ptScreenPos.x-5, (int)m_ptScreenPos.y-10, m_fZDepth, 1.1f, m_dwColor);
	}

	if (m_bDisplayInfo)
		m_pCityInfo->Render();
}

void CCity::SetPostBaseCTORVars()
{
	// set up the offset so the object is centered on the tiles
	m_ptOffset = point(-5, 0);
	m_fZDepth = DEPTH_CITY;
	m_eDefaultAbilityType = BN_NONCOMBAT_SKILLS;
	SetStartingAbilities();
}
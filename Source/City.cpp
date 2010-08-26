#include "stdafx.h"

#include "City.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "UICityInfo.h"

CCity::CCity() : CObject(), m_nCityType(0), m_nSize(0), m_pCityInfo(NULL)
{

}
CCity::~CCity()
{ 
	SAFE_DELETE(m_pCityInfo); 
}

CCity::CCity(int type, int cityType, int size, point coord, point sPos, string name, const char* faction, int factionID)
: CObject(type, coord, sPos, name, faction, factionID)
{
	SetImageID(Globals::g_pAssets->GetMapasts()->City());
	SetSrcRect(rect(cityType, cityType+CITY_HEIGHT, (size-1)*CITY_WIDTH, (size-1)*CITY_WIDTH+CITY_WIDTH));	

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
		m_pCityInfo->Render();
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
		CObject::Render(viewPort);
		// draw the city names over the cities...
		Globals::g_pBitMapFont->DrawString(m_strName.c_str(), (int)m_ptScreenPos.x-5, (int)m_ptScreenPos.y-10, m_fZDepth, 1.1f, m_dwColor);
	}

	if (m_bDisplayInfo)
		m_pCityInfo->Render();
}
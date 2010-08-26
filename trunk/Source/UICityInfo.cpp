#include "stdafx.h"

#include "UICityInfo.h"
#include "Globals.h"
#include "Assets.h"
#include "City.h"
#include "UIObjectInfoBase.h"
#include "BitmapFont.h"

// just starting default values
// grid sizes may change during game
const int inv_grid_rows = 10;
const int inv_grid_cols = 8;
const int equip_grid_rows = 10;
const int equip_grid_cols = 5;

UICityInfo::UICityInfo(CCity* const city) : 
	UIObjectInfoBase(OIT_CITY, 
				 Globals::g_pAssets->GetGUIasts()->Equipped(OIT_CITY), 
				 Globals::g_pAssets->GetGUIasts()->Info(OIT_CITY), 
				 Globals::g_pAssets->GetGUIasts()->Inventory(OIT_CITY),
				 // inventory (available) grid values 					// equip grid values
				 Grid(inv_grid_rows, inv_grid_cols, point(450, 100)),	Grid(equip_grid_rows, equip_grid_cols, point(10, 100)),
				 // inventory pos				// equip pos			// info pos
				 point(450, 100),				point(10, 100),			point(750, 100),
				 // inventory area size									// equip area size
				 size(inv_grid_cols*68+16, inv_grid_rows*68+16),		size(equip_grid_cols*68+16, equip_grid_rows*68+16)),
	m_pCity(city)
{

}
UICityInfo::~UICityInfo()
{

}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: Render
//////////////////////////////////////////////////////////////////////////
void UICityInfo::Render()
{
	UIObjectInfoBase::Render();

	// city name
	Globals::g_pBitMapFont->DrawStringAutoCenter(m_pCity->GetName().c_str(), 
												 rect(10, 10+Globals::g_pBitMapFont->GetSize(), 0, Globals::g_ptScreenSize.width),
												 DEPTH_INVOBJ, 1.5f, YELLOW_WHITE);
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: Update
//
// 
//////////////////////////////////////////////////////////////////////////
void UICityInfo::Update(double dTimeStep)
{

}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: Input
//
// 
//////////////////////////////////////////////////////////////////////////
void UICityInfo::Input(const POINT& mouse)
{
	UIObjectInfoBase::Input(mouse);
}
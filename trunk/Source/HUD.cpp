#include "stdafx.h"

#include "HUD.h"
#include "Map.h"
#include "Object.h"
#include "Unit.h"
#include "Player.h"
#include "BitmapFont.h"
#include "Globals.h"
#include "Assets.h"
#include "ObjectDefines.h"
#include "UIOptionsWindow.h"
#include "UIQuickBar.h"
#include "AbilityObjectBase.h"
using namespace ObjectDefines;

const int   POS_SLOT_Y_SPACE = 5;
const int	POS_SLOT_X_SPACE = 15;
const point	BTN_SIZE		 = point(117, 27);

const point POS_SLOT_1_1 = point(765, 655);
const point POS_SLOT_2_1 = point(765 + BTN_SIZE.x + 5, 655);
const point POS_SLOT_3_1 = point(765 + (BTN_SIZE.x + 5) * 2, 655);

CHUD* CHUD::GetInstance()
{
	static CHUD instance;
	return &instance;
}

void CHUD::Init()
{
	InitButtons();
	InitBtnSlots();
	SetObjectButtonMatches();
	m_pQuickBar = new CUIQuickBar(Globals::g_ptQBPos, QBSLOT_SIZE);
	m_pSelectedPlayerObj = m_pSelectedTarget = m_pPrevSelectedPlayerObjState = NULL;
}

void CHUD::Shutdown()
{
	SAFE_DELETE(g_pObjButtonsMap);
	SAFE_DELETE(m_pQuickBar);
	for (unsigned i = 0; i < m_vWindows.size(); ++i)
		SAFE_DELETE(m_vWindows[i]);
}

void CHUD::Render()
{
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->HUD(), 0, 0, DEPTH_HUDFRAME);	// the main HUD's frame bg
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->Frame(), 0, 0, DEPTH_HUDBORDER);	// the border
	Globals::g_pBitMapFont->DrawString(Globals::g_pCurrPlayer->GetProfile()->name.c_str(), 230, 25, DEPTH_PLAYERINFO);

	m_pQuickBar->Render();

	DrawResources();

	// draw any info for the currently selected player item/unit/building/etc (if any)
	DrawSelectedObjInfo();

	// draw any info for the currently selected target item/unit/building/etc (if any)
	DrawTargetInfo();

	// draw all active buttons
	DrawButtons();

	if (m_vWindows.size())
	{
		m_iWindowsIter = m_vWindows.begin(); m_iEnd = m_vWindows.end();
		for ( ; m_iWindowsIter != m_iEnd; ++m_iWindowsIter)
		{
			(*m_iWindowsIter)->Render();
		}
	}
}

void CHUD::Update(double fElapsedTime)
{
	for (int i = 0; !Globals::g_bWindowOpen && i < BL_NUM_LOCATIONS; ++i)
		if (m_arrBtnSlots[i].pButton)
			m_arrBtnSlots[i].pButton->Update(fElapsedTime);

	m_pPrevSelectedPlayerObjState = m_pSelectedPlayerObj;
	m_pSelectedPlayerObj = Globals::g_pMap->GetSelectedObject();
	m_pSelectedTarget	 = Globals::g_pMap->GetTarget();
	if (m_pPrevSelectedPlayerObjState != m_pSelectedPlayerObj)
	{
		if (m_pSelectedPlayerObj)
			SetButtons();
		else
			ClearButtons();
	}
}

int CHUD::Input(POINT& mouse)
{
	point mousePt = mouse;

	m_pQuickBar->Input(mouse);

	// handle button input
	for (int i = 0; !Globals::g_bWindowOpen && i < BL_NUM_LOCATIONS; ++i)
	{
		if (m_arrBtnSlots[i].pButton)
		{
			CUIOptionsWindow* pWindow = NULL;	// Input creates window if necessary
			m_arrBtnSlots[i].pButton->Input(mouse, (CUIWindowBase*&)pWindow);
			if ( pWindow )
			{
				Globals::g_bWindowOpen = true;
				m_vWindows.push_back((CUIWindowBase*)pWindow);
			}
		}
	}

	// TODO:: take out loop if the top one is going to remain the only one that calls Input
	if (m_vWindows.size())
	{
		CUIWindowBase* pWindow = NULL;
		m_iWindowsIter = m_vWindows.end() - 1; m_iEnd = m_vWindows.end();	// begin at the top window (last in vector)
		for ( ; m_iWindowsIter != m_iEnd; ++m_iWindowsIter)
		{
			eReturnCode code = (*m_iWindowsIter)->Input(mouse, pWindow);
			if (code < RC_NO_SELECTION)	// means we have a valid selection
			{
				if (pWindow)	// a new window needs to be added
				{
					// TODO:: add the window, etc...
					if (pWindow)
					{	m_vWindows.push_back(pWindow); break; }
				}
				if (code == RC_SELECTION)	// a selection was made from the options
				{
					break;	// no need to do anything here?
				}
				else if (code == RC_CLOSE)	// user pushed close button
				{
					delete (*m_iWindowsIter);
					m_vWindows.erase(m_iWindowsIter);
					if (!m_vWindows.size())
						Globals::g_bWindowOpen = false;
					break;
				}
			}
		}
	}

	return 0;
}

void CHUD::DrawResources(  )
{
	char buff[32];
	// resources	([0] is always human player)
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetCrystalRes());
	Globals::g_pBitMapFont->DrawString(buff, 600, 27, DEPTH_RESOURCES, 0.75f);
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetWoodRes());
	Globals::g_pBitMapFont->DrawString(buff, 699, 27, DEPTH_RESOURCES, 0.75f);
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetOreRes());
	Globals::g_pBitMapFont->DrawString(buff, 802, 27, DEPTH_RESOURCES, 0.75f);
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetGoldRes());
	Globals::g_pBitMapFont->DrawString(buff, 899, 27, DEPTH_RESOURCES, 0.75f);
}

void CHUD::DrawSelectedObjInfo( )
{
	// TODO:: have each object draw it's own info
	char buff[32];
	if (m_pSelectedPlayerObj)
	{
		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->PlayerInfoBG(), 575, 651, DEPTH_PLAYERINFOBG);
		switch(m_pSelectedPlayerObj->GetType())
		{
		case OBJ_UNIT:
			{
				CUnit* unit = (CUnit*)m_pSelectedPlayerObj;
				sprintf_s(buff, "Vit:%i/%i", unit->GetVitality(), unit->GetMaxVit());
				Globals::g_pBitMapFont->DrawString(buff, 585, 685, DEPTH_PLAYERINFO, 0.8f);
				sprintf_s(buff, "Sta:%i/%i", unit->GetStamina(), unit->GetMaxStamina());
				Globals::g_pBitMapFont->DrawString(buff, 585, 705, DEPTH_PLAYERINFO, 0.8f);
				sprintf_s(buff, "Mag:%i/%i", unit->GetMP(), unit->GetMaxMagPts());
				Globals::g_pBitMapFont->DrawString(buff, 585, 725, DEPTH_PLAYERINFO, 0.8f);
			}
			break;
		case OBJ_CITY:
			{
			}
			break;
		case OBJ_BUILDING:
			{
			}
			break;
		}

		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->FactionImages()[m_pSelectedPlayerObj->GetFactionID()], 582, 657, DEPTH_PLAYERINFO, 0.75f, 0.75f);
		Globals::g_pBitMapFont->DrawString(m_pSelectedPlayerObj->GetName().c_str(), 620, 660, DEPTH_PLAYERINFO, 0.8f);
	}
}

void CHUD::DrawTargetInfo( )
{
	// TODO:: have each object draw it's own info
	char buff[32];
	if (m_pSelectedTarget)
	{
		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->EnemyInfoBG(), 265, 651, DEPTH_PLAYERINFOBG);
		switch(m_pSelectedTarget->GetType())
		{
		case OBJ_UNIT:
			{
				CUnit* unit = (CUnit*)m_pSelectedTarget;
				sprintf_s(buff, "Vit:%i/%i", unit->GetVitality(), unit->GetMaxVit());
				Globals::g_pBitMapFont->DrawString(buff, 275, 685, DEPTH_PLAYERINFO, 0.8f);
				sprintf_s(buff, "Sta:%i/%i", unit->GetStamina(), unit->GetMaxStamina());
				Globals::g_pBitMapFont->DrawString(buff, 275, 705, DEPTH_PLAYERINFO, 0.8f);
				sprintf_s(buff, "Mag:%i/%i", unit->GetMP(), unit->GetMaxMagPts());
				Globals::g_pBitMapFont->DrawString(buff, 275, 725, DEPTH_PLAYERINFO, 0.8f);

			}
			break;
		case OBJ_CITY:
			{

			}
			break;
		case OBJ_BUILDING:
			{

			}
			break;
		}

		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->FactionImages()[m_pSelectedTarget->GetFactionID()], 272, 658, DEPTH_PLAYERINFO, 0.75f, 0.75f);
		Globals::g_pBitMapFont->DrawString(m_pSelectedTarget->GetName().c_str(), 310, 660, DEPTH_PLAYERINFO, 0.8f);
	}
}

void CHUD::InitBtnSlots()
{
	m_arrBtnSlots[BL_MENU].Rect		= rect(point(75, 20), BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_1_1].Rect	= rect(POS_SLOT_1_1, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_1_2].Rect	= rect(POS_SLOT_1_1, BTN_SIZE, point(0,BTN_SIZE.y+5));
	m_arrBtnSlots[BL_SLOT_1_3].Rect	= rect(POS_SLOT_1_1, BTN_SIZE, point(0,(BTN_SIZE.y+5)*2));
	m_arrBtnSlots[BL_SLOT_2_1].Rect = rect(POS_SLOT_2_1, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_2_2].Rect = rect(POS_SLOT_2_1, BTN_SIZE, point(0,BTN_SIZE.y+5));
	m_arrBtnSlots[BL_SLOT_2_3].Rect = rect(POS_SLOT_2_1, BTN_SIZE, point(0,(BTN_SIZE.y+5)*2));
	m_arrBtnSlots[BL_SLOT_3_1].Rect = rect(POS_SLOT_3_1, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_3_2].Rect = rect(POS_SLOT_3_1, BTN_SIZE, point(0,BTN_SIZE.y+5));
	m_arrBtnSlots[BL_SLOT_3_3].Rect = rect(POS_SLOT_3_1, BTN_SIZE, point(0,(BTN_SIZE.y+5)*2));
	SetButtonSlot(BL_MENU, &m_arrButtons[BN_MENU]);
}

void CHUD::InitButtons()
{
	m_arrButtons[BN_MENU].SetVariables(BN_MENU, BtnAction_OptionBox, "Menu");
	m_arrButtons[BN_GEAR].SetVariables(BN_GEAR, BtnAction_OptionBox, "Gear");
	m_arrButtons[BN_SKILLS].SetVariables(BN_SKILLS, BtnAction_OptionBox, "Skills");
	m_arrButtons[BN_COMBAT_SKILLS].SetVariables(BN_COMBAT_SKILLS, BtnAction_OptionBox, "CSkills");
	m_arrButtons[BN_NONCOMBAT_SKILLS].SetVariables(BN_NONCOMBAT_SKILLS, BtnAction_OptionBox, "Skills");

	m_arrButtons[BN_DISBAND].SetVariables( BN_DISBAND, BtnAction_Disband, "Disband" );
	m_arrButtons[BN_WAIT].SetVariables   ( BN_WAIT,    BtnAction_Wait, "Wait" );
	m_arrButtons[BN_SKIP].SetVariables   ( BN_SKIP,    BtnAction_Skip, "Skip" );
}

void CHUD::DrawButtons()
{
	for (eButtonSlot i = (eButtonSlot)0; i < BL_NUM_LOCATIONS; i = eButtonSlot(i + 1))
		if (m_arrBtnSlots[i].pButton)	// TODO:: remove safety check when all images are made
			m_arrBtnSlots[i].pButton->Render();
}

void CHUD::SetButtons()
{
	ButtonNamesVec names = (*g_pObjButtonsMap)[m_pSelectedPlayerObj->GetName()];
	for (ButtonNamesVec::iterator begin = names.begin(), end = names.end(); begin != end; ++begin)
	{
		// slot first, name second
		if (m_arrButtons[(*begin).second].GetUpID() > -1)
			SetButtonSlot((*begin).first, &m_arrButtons[(*begin).second]);
		else
			SetButtonSlot((*begin).first, NULL);	// there is no image (yet) for this button, set pButton to NULL
	}
}

void CHUD::ClearButtons()
{
	for (unsigned i = 0; i < BL_NUM_LOCATIONS; ++i)
	{
		if ((eButtonSlot)i != BL_MENU)	// never clear the menu button
			SetButtonSlot((eButtonSlot)i, NULL);
	}
}

inline
void CHUD::SetButtonSlot(eButtonSlot slotEnum, CButton* button )
{
	if (button)
		button->SetRect(m_arrBtnSlots[slotEnum].Rect); 
	m_arrBtnSlots[slotEnum].pButton = button; 
}

void CHUD::CloseCurrWindow()
{
	if (m_vWindows.size())
	{
		m_iWindowsIter = m_vWindows.end() - 1;
		delete (*m_iWindowsIter);
		m_vWindows.erase(m_iWindowsIter);
		if (!m_vWindows.size())
			Globals::g_bWindowOpen = false;
	}
}

void CHUD::AddWindow(CUIWindowBase* window)		
{ 
	m_vWindows.push_back(window); 
	Globals::g_bWindowOpen = true; 
}
void CHUD::ClearQBSlots()
{
	 m_pQuickBar->ClearSlots();
}
void CHUD::SetInitialQBSlots( CObject* const owner, CQuickBarObject** qbObjects)	
{
	m_pQuickBar->InitSlots(owner, qbObjects); 
}

void CHUD::SetQBSlot( int slot, CQuickBarObject* qbObj)
{
	m_pQuickBar->SetSlot(slot, qbObj); 
}

void CHUD::ToggleAddingQBObjects( CQuickBarObject* const qbObj)
{
	m_pQuickBar->ToggleAddingQBObj(qbObj);
}

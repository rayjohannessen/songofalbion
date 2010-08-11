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

// BUTTON POS/SIZE VALUES
const int	POS_X			 = 585;
const int	POS_Y			 = 656;
const point	BTN_SIZE		 = point(127, 27);

const point POS_SLOT_1_1 = point(POS_X, POS_Y);
const point POS_SLOT_2_1 = point(POS_X + BTN_SIZE.x, POS_Y);
const point POS_SLOT_3_1 = point(POS_X + BTN_SIZE.x * 2, POS_Y);
const point POS_END_TURN = point(885, 610);
const point POS_MENU	 = point(75, 19);

const int	POS_RES_Y	 = 25;

// UNIT INFO POS/SIZE VALUES
const float FACTION_SCALE	 = 0.75f;
const float INFO_TEXT_SCALE	 = 0.8f;
const int	TEXT_Y_SPACING	 = 20;
const point TEXT_OS_FROM_BG	 = point(10, 34);
const point FACTION_OS		 = point(7, 6);
const point NAME_OS			 = point(45, 9);
// SELECTED (FRIENDLY OBJ)
const point	POS_SEL_OBJ_BG	 = point(255, 651);
const point POS_NAME		 = point(POS_SEL_OBJ_BG).Offset(NAME_OS);
const point	POS_TEXT_BEGIN	 = point(POS_SEL_OBJ_BG).Offset(TEXT_OS_FROM_BG);
const point POS_VIT			 = point(POS_TEXT_BEGIN);
const point POS_STA			 = point(POS_TEXT_BEGIN.x, POS_TEXT_BEGIN.y + TEXT_Y_SPACING);
const point POS_MAG			 = point(POS_TEXT_BEGIN.x, POS_TEXT_BEGIN.y + TEXT_Y_SPACING * 2);
const point POS_FACTION		 = point(POS_SEL_OBJ_BG).Offset(FACTION_OS);
// SELECTED (TARGET OBJ)
const point POS_TARGET_BG	 = point(55, 651);
const point POS_NAME_T		 = point(POS_TARGET_BG).Offset(NAME_OS);
const point	POS_TEXT_BEGIN_T = point(POS_TARGET_BG).Offset(TEXT_OS_FROM_BG);
const point POS_VIT_T		 = point(POS_TEXT_BEGIN_T);
const point POS_STA_T		 = point(POS_TEXT_BEGIN_T.x, POS_TEXT_BEGIN_T.y + TEXT_Y_SPACING);
const point POS_MAG_T		 = point(POS_TEXT_BEGIN_T.x, POS_TEXT_BEGIN_T.y + TEXT_Y_SPACING * 2);
const point POS_FACTION_T	 = point(POS_TARGET_BG).Offset(FACTION_OS);


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
	Globals::g_pBitMapFont->DrawString(Globals::g_pCurrPlayer->GetProfile()->name.c_str(), 230, 22, DEPTH_PLAYERINFO, 1.0f, YELLOW_WHITE);

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

void CHUD::Update(double dTimeStep)
{
	for (int i = 0; !Globals::g_bWindowOpen && i < BL_NUM_LOCATIONS; ++i)
		if (m_arrBtnSlots[i].pButton)
			m_arrBtnSlots[i].pButton->Update(dTimeStep);

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

bool CHUD::Input(const POINT& mouse)
{
	point mousePt = mouse;

	m_pQuickBar->Input(mouse);

	// handle button input - don't handle if there is a window open
	bool mouseOverBtn = false;	
	for (int i = 0; !Globals::g_bWindowOpen && i < BL_NUM_LOCATIONS; ++i)
	{
		if (m_arrBtnSlots[i].pButton)
		{
			if (mouseOverBtn)// if mouse over button, no need to do point in rect checks, just make sure to reset any hovers
			{
				m_arrBtnSlots[i].pButton->SetStateToUp();
				continue;
			}
			CUIOptionsWindow* pWindow = NULL;	// Input creates window if necessary
			mouseOverBtn = m_arrBtnSlots[i].pButton->Input(mouse, (CUIWindowBase*&)pWindow);
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
					return false;	// no need to do anything here?
				}
				else if (code == RC_CLOSE)	// user pushed close button
				{
					delete (*m_iWindowsIter);
					m_vWindows.erase(m_iWindowsIter);
					if (!m_vWindows.size())
						Globals::g_bWindowOpen = false;
					return false;
				}
				else if (code == RC_TO_MENU)
				{
					delete (*m_iWindowsIter);
					m_vWindows.erase(m_iWindowsIter);
					if (!m_vWindows.size())
						Globals::g_bWindowOpen = false;

					return true;
				}
			}
		}
	}
	return false;
}

void CHUD::DrawResources(  )
{
	char buff[32];
	// resources	([0] is always human player)
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetCrystalRes());
	Globals::g_pBitMapFont->DrawString(buff, 600, POS_RES_Y, DEPTH_RESOURCES, 0.75f);
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetWoodRes());
	Globals::g_pBitMapFont->DrawString(buff, 699, POS_RES_Y, DEPTH_RESOURCES, 0.75f);
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetOreRes());
	Globals::g_pBitMapFont->DrawString(buff, 802, POS_RES_Y, DEPTH_RESOURCES, 0.75f);
	sprintf_s(buff, " %i", Globals::GetPlayers()[0]->GetGoldRes());
	Globals::g_pBitMapFont->DrawString(buff, 899, POS_RES_Y, DEPTH_RESOURCES, 0.75f);
}

void CHUD::DrawSelectedObjInfo( )
{
	char buff[32];
	if (m_pSelectedPlayerObj)
	{
		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->PlayerInfoBG(), POS_SEL_OBJ_BG.x, POS_SEL_OBJ_BG.y, DEPTH_PLAYERINFOBG);
		switch(m_pSelectedPlayerObj->GetType())
		{
		case OBJ_UNIT:
			{
				CUnit* unit = (CUnit*)m_pSelectedPlayerObj;
				sprintf_s(buff, "Vit:%i/%i", unit->GetVitality(), unit->GetMaxVit());
				Globals::g_pBitMapFont->DrawString(buff, POS_VIT.x, POS_VIT.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
				sprintf_s(buff, "Sta:%i/%i", unit->GetStamina(), unit->GetMaxStamina());
				Globals::g_pBitMapFont->DrawString(buff, POS_STA.x, POS_STA.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
				sprintf_s(buff, "Mag:%i/%i", unit->GetMP(), unit->GetMaxMagPts());
				Globals::g_pBitMapFont->DrawString(buff, POS_MAG.x, POS_MAG.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
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

		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->FactionImages()[m_pSelectedPlayerObj->GetFactionID()], POS_FACTION.x, POS_FACTION.y, DEPTH_PLAYERINFO, FACTION_SCALE, FACTION_SCALE);
		Globals::g_pBitMapFont->DrawString(m_pSelectedPlayerObj->GetName().c_str(), POS_NAME.x, POS_NAME.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
	}
}

void CHUD::DrawTargetInfo( )
{
	char buff[32];
	if (m_pSelectedTarget)
	{
		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->EnemyInfoBG(), POS_TARGET_BG.x, POS_TARGET_BG.y, DEPTH_PLAYERINFOBG);
		switch(m_pSelectedTarget->GetType())
		{
		case OBJ_UNIT:
			{
				CUnit* unit = (CUnit*)m_pSelectedTarget;
				sprintf_s(buff, "Vit:%i/%i", unit->GetVitality(), unit->GetMaxVit());
				Globals::g_pBitMapFont->DrawString(buff, POS_VIT_T.x, POS_VIT_T.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
				sprintf_s(buff, "Sta:%i/%i", unit->GetStamina(), unit->GetMaxStamina());
				Globals::g_pBitMapFont->DrawString(buff, POS_STA_T.x, POS_STA_T.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
				sprintf_s(buff, "Mag:%i/%i", unit->GetMP(), unit->GetMaxMagPts());
				Globals::g_pBitMapFont->DrawString(buff, POS_MAG_T.x, POS_MAG_T.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
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

		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->FactionImages()[m_pSelectedTarget->GetFactionID()], POS_FACTION_T.x, POS_FACTION_T.y, DEPTH_PLAYERINFO, FACTION_SCALE, FACTION_SCALE);
		Globals::g_pBitMapFont->DrawString(m_pSelectedTarget->GetName().c_str(), POS_NAME_T.x, POS_NAME_T.y, DEPTH_PLAYERINFO, INFO_TEXT_SCALE);
	}
}

void CHUD::InitBtnSlots()
{
	m_arrBtnSlots[BL_MENU].Rect		= rect(POS_MENU, BTN_SIZE);
	m_arrBtnSlots[BL_END_TURN].Rect	= rect(POS_END_TURN, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_1_1].Rect	= rect(POS_SLOT_1_1, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_1_2].Rect	= rect(POS_SLOT_1_1, BTN_SIZE, point(0, BTN_SIZE.y) );
	m_arrBtnSlots[BL_SLOT_1_3].Rect	= rect(POS_SLOT_1_1, BTN_SIZE, point(0, BTN_SIZE.y * 2));
	m_arrBtnSlots[BL_SLOT_2_1].Rect = rect(POS_SLOT_2_1, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_2_2].Rect = rect(POS_SLOT_2_1, BTN_SIZE, point(0, BTN_SIZE.y) );
	m_arrBtnSlots[BL_SLOT_2_3].Rect = rect(POS_SLOT_2_1, BTN_SIZE, point(0, BTN_SIZE.y * 2));
	m_arrBtnSlots[BL_SLOT_3_1].Rect = rect(POS_SLOT_3_1, BTN_SIZE);
	m_arrBtnSlots[BL_SLOT_3_2].Rect = rect(POS_SLOT_3_1, BTN_SIZE, point(0, BTN_SIZE.y) );
	m_arrBtnSlots[BL_SLOT_3_3].Rect = rect(POS_SLOT_3_1, BTN_SIZE, point(0, BTN_SIZE.y * 2));
	SetButtonSlot(BL_MENU, &m_arrButtons[BN_MENU]);
	SetButtonSlot(BL_END_TURN, &m_arrButtons[BN_END_TURN]);
}

void CHUD::InitButtons()
{
	m_arrButtons[BN_MENU].SetVariables			  (BN_MENU,			    BtnAction_OptionBox, "Menu");
	m_arrButtons[BN_END_TURN].SetVariables		  (BN_END_TURN,		    BtnAction_EndTurn,   "End");
	m_arrButtons[BN_GEAR].SetVariables			  (BN_GEAR,			    BtnAction_OptionBox, "Gear");
	m_arrButtons[BN_SKILLS].SetVariables		  (BN_SKILLS,			BtnAction_OptionBox, "Skills");
	m_arrButtons[BN_COMBAT_SKILLS].SetVariables	  (BN_COMBAT_SKILLS,	BtnAction_OptionBox, "CSkills");
	m_arrButtons[BN_NONCOMBAT_SKILLS].SetVariables(BN_NONCOMBAT_SKILLS, BtnAction_OptionBox, "Skills");

	m_arrButtons[BN_DISBAND].SetVariables( BN_DISBAND, BtnAction_Disband, "Disband" );
	m_arrButtons[BN_WAIT].SetVariables   ( BN_WAIT,    BtnAction_Wait,	  "Wait" );
	m_arrButtons[BN_SKIP].SetVariables   ( BN_SKIP,    BtnAction_Skip,	  "Skip" );
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
	// never clear the menu or the end turn button
	for (unsigned i = BL_SLOT_1_1; i < BL_NUM_LOCATIONS; ++i)
		SetButtonSlot((eButtonSlot)i, NULL);
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

void CHUD::AddWindow(CUIWindowBase* const window)		
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

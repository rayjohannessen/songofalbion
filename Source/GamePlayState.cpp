//////////////////////////////////////////////////////////////////////////
//	Filename	:	GamePlayState.cpp
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CGamePlayState class. This state handles
//					all gameplay code.
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "GamePlayState.h"
#include "BaseMenuState.h"
#include "Game.h"
#include "Map.h"
#include "HUD.h"
#include "Player.h"
#include "BitmapFont.h"
#include "Globals.h"
#include "ObjectManager.h"
#include "Assets.h"

//TEMPORARY
#include "Unit.h"

CGamePlayState::CGamePlayState(void)
{
	m_pCurrentMenuState = NULL;
	m_bIsPaused = false;
	Globals::g_pMap = CMap::GetInstance();
}

CGamePlayState::~CGamePlayState(void)
{
}

void CGamePlayState::Enter(void)
{
	// TODO:: get the player info from some other pre-game screen and set here
	m_nNumPlayers = 2;
	Globals::InitPlayers(m_nNumPlayers);

	Globals::InitHUD();
	Globals::g_pAssets->LoadInGameGUI();
	Globals::g_pAssets->LoadMap();
	Globals::g_pAssets->LoadUnits();

	Globals::g_pMap->Init(	Globals::g_pGame->GetScreenWidth(), Globals::g_pGame->GetScreenHeight() );

	Globals::InitObjManager();
 
	// TODO:: add a unit to each player for starters -- temporary
	// human
	CPlayer* player = Globals::GetPlayerByFactionID(0);
	CUnit* unit = new CUnit(UnitDefines::UNIT_UMKNIGHT, OBJ_UNIT, point(4, 5), Globals::g_pMap->IsoTilePlot(point(4, 5)), "UMKnight",
					player->GetProfile()->name.c_str(), player->GetProfile()->nFactionID);
	Globals::g_pObjManager->AddObject(player, (CObject*&)(unit), Globals::g_pMap->IsoTilePlot(point(4, 5) ));
	Globals::g_pObjManager->StartTurn(player);
	// comp
	player = Globals::GetPlayerByFactionID(1);
	unit = new CUnit(UnitDefines::UNIT_UMKNIGHT, OBJ_UNIT, point(5, 5), Globals::g_pMap->IsoTilePlot(point(5, 5)), "UMKnight",
					player->GetProfile()->name.c_str(), player->GetProfile()->nFactionID);
	Globals::g_pObjManager->AddObject(player, (CObject*&)(unit), Globals::g_pMap->IsoTilePlot(point(5, 5) ));
}

void CGamePlayState::Exit(void)
{
	if (Globals::g_pMap)
	{
		Globals::g_pMap->Shutdown();
		Globals::g_pMap = NULL;
	}
	if(m_pCurrentMenuState)
	{
		m_pCurrentMenuState->Exit();
		m_pCurrentMenuState = NULL;
	}
	Globals::ShutdownHUD();
	Globals::ShutdownObjManager();
	Globals::ClearPlayers();
}

CGamePlayState* CGamePlayState::GetInstance(void)
{
	static CGamePlayState instance;
	return &instance;
}

bool CGamePlayState::Input(double fElapsedTime, POINT mousePt)
{
	// map input
	eMapInputRet mapRet = Globals::g_pMap->Input(fElapsedTime, mousePt);
	if(mapRet == MIR_EXIT)
		return false;
	else if (mapRet == MIR_NEXTPLAYER)
		Globals::GotoNextPlayer();

	// HUD input
	Globals::g_pHUD->Input(mousePt);
	
	return true;
}

void CGamePlayState::Update(double fElapsedTime)
{
	if(!m_bIsPaused)
	{
		// Update map
		Globals::g_pMap->Update(fElapsedTime);
		// update objects
		Globals::g_pObjManager->Update(fElapsedTime, Globals::g_pMap->GetTotalFrameScroll());
		// update HUD
		Globals::g_pHUD->Update(fElapsedTime);
	}
}

void CGamePlayState::Render(void)
{
	// Render Map
	Globals::g_pMap->Render();
	// Render Objects
	Globals::g_pObjManager->Render();
	// Render HUD
	Globals::g_pHUD->Render();
}

void CGamePlayState::LoadGame(const char* fileName)
{
	ifstream ifs;
	string fName = fileName;
	string pathFileName = "SavedGames/" + fName;
	ifs.open(pathFileName.c_str(), ios_base::binary | ios_base::in);
	if (ifs.is_open())
	{

		ifs.close();
	}
}

void CGamePlayState::SaveGame(const char* fileName)
{
	ofstream ofs;
	string fName = fileName;
	string pathFileName = "SavedGames/" + fName;
	ofs.open(pathFileName.c_str(), ios_base::binary | ios_base::out);

	if (ofs.is_open())
	{
		ofs.close();
	}
}


void CGamePlayState::ChangeMap(bool bWorldMap, int mapID) // if no parameter sent, goes to WORLD_MAP by default
{
}
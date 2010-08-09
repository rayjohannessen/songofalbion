#include "stdafx.h"

#include "Globals.h"
#include "Player.h"
#include "Assets.h"
#include "BitmapFont.h"
#include "HUD.h"
#include "Map.h"
#include "ObjectManager.h"
#include "AnimationsManager.h"
#include "AbilitiesManager.h"
#include "Unit.h"
#include "DebugWindow.h"
#include "MenuFunctions.h"
CoordToDirMap			Globals::g_CoordToDir[2]	= {};
DirToCoordMap			Globals::g_AdjTileOffsets[2]= {};
bool					Globals::g_bWindowOpen		= false;
short					Globals::g_nNumPlayers		= 0;
short					Globals::g_nPlayerFactionID	= 0;
unsigned				Globals::m_nCurrPlayerInd	= 0;
point					Globals::g_ptQBPos		= point(575, 603);
point					Globals::g_ptScreenSize = point(1024, 768);
CSGD_Direct3D*			Globals::g_pD3D			= NULL;
CSGD_TextureManager*	Globals::g_pTM			= NULL;
CSGD_DirectInput*		Globals::g_pDI			= NULL;
CSGD_FModManager*		Globals::g_pFMOD		= NULL;
CBitmapFont*			Globals::g_pBitMapFont	= NULL;
CAssets*				Globals::g_pAssets		= NULL;
CHUD*					Globals::g_pHUD			= NULL;
CGame*					Globals::g_pGame		= NULL;
CMap*					Globals::g_pMap			= NULL;
ObjectManager*			Globals::g_pObjManager	= NULL;
Players*				Globals::g_vPlayers		= NULL;
CPlayer*				Globals::g_pCurrPlayer	= NULL;
UnitNamesTypes*			Globals::g_vUnitNames	= NULL;	// initialized in CAssets
CAnimationsManager*		Globals::g_pAnimManager	= NULL;	// initialized in CAssets
CAbilitiesManager*		Globals::g_pAbilitiesManager = NULL;
CMenu*					Globals::g_pMenus[NUM_MENU_TYPES] = {};

bool Globals::InitGlobals(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	g_vUnitNames    = new UnitNamesTypes;
	g_pD3D			= CSGD_Direct3D::GetInstance();
	g_pTM			= CSGD_TextureManager::GetInstance();
	g_pDI			= CSGD_DirectInput::GetInstance();
	g_pFMOD			= CSGD_FModManager::GetInstance();
	g_pAssets		= CAssets::GetInstance();
	g_pAnimManager  = CAnimationsManager::GetInstance();
	g_pAbilitiesManager = CAbilitiesManager::GetInstance();

	g_ptScreenSize = point(nScreenWidth, nScreenHeight);
	g_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, bIsWindowed);
	g_pTM->InitTextureManager(g_pD3D->GetDirect3DDevice(), g_pD3D->GetSprite());
	g_pAssets->Init();

	g_pBitMapFont	= CBitmapFont::GetInstance();
	g_pBitMapFont->LoadProfiles();

	if(!g_pFMOD->InitFModManager(hWnd))
		return false;

	g_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD, 0);
	g_pDI->InitDirectInput(hWnd, hInstance, DI_MOUSE, 0);
	g_pDI->InitDirectInput(hWnd, hInstance, DI_JOYSTICKS, 0);
	g_pDI->AcquireAll();

	g_pAbilitiesManager->Init();

	//////////////////////////////////////////////////////////////////////////
	// assign the directions to their corresponding point values and vice versa
	SetCoordDirMaps();

	// init menus 
	InitMenus();

	return true;
}
		
void Globals::ShutdownGlobals()
{
	if (g_pD3D)
	{
		g_pD3D->Shutdown();	
		g_pD3D = NULL;
	}
	if (g_pTM)
	{
		g_pTM->Shutdown();	
		g_pTM = NULL;
	}
	if (g_pDI)
	{
		g_pDI->Shutdown();
		g_pDI = NULL;
	}
	if (g_pFMOD)
	{
		g_pFMOD->Shutdown();
		g_pFMOD = NULL;
	}
	if (g_pAnimManager)
	{
		g_pAnimManager->Shutdown();
		g_pAnimManager = NULL;
	}
	if (g_pAssets)
	{
		g_pAssets->Shutdown();
		g_pAssets = NULL;
	}
	if (g_pAbilitiesManager)
	{
		g_pAbilitiesManager->Shutdown();
		g_pAbilitiesManager = NULL;
	}
	SAFE_DELETE(g_vUnitNames);
	for (unsigned i = 0; i < NUM_MENU_TYPES; ++i)
		SAFE_DELETE(g_pMenus[i]);
}

//////////////////////////////////////////////////////////////////////////
// other globals that must be initialized/shutdown separately
bool Globals::InitHUD()
{
	if (!g_pTM || !g_pAssets)
		return false;
	g_pHUD = CHUD::GetInstance();
	g_pHUD->Init();
	return true;
}
bool Globals::InitObjManager()
{
	g_pObjManager = new ObjectManager();
	g_pObjManager->Init();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Player-related
bool Globals::InitPlayers(short numPlayers)
{	
	m_nCurrPlayerInd = 0;
	g_nNumPlayers = numPlayers;
	g_vPlayers = new Players;
	CPlayer* pPlayers = new CPlayer("HumanPlayer", true);
	g_pCurrPlayer = pPlayers;	// human player starts always...?
	g_nPlayerFactionID = pPlayers->GetProfile()->nFactionID;
	g_vPlayers->push_back(pPlayers);

	// setup comp players
	for (short i = 1; i < numPlayers; ++i)
	{
		pPlayers = new CPlayer("CompPlayer", false, i); // TODO:: set faction of comps based on what player chose
		g_vPlayers->push_back(pPlayers);
	}

	return true;
}

void Globals::GotoNextPlayer()
{
	if (++m_nCurrPlayerInd == g_vPlayers->size()) 
		m_nCurrPlayerInd = 0; 
	g_pCurrPlayer = (*g_vPlayers)[m_nCurrPlayerInd]; 
	g_pObjManager->StartTurn(g_pCurrPlayer);
}

void Globals::ClearPlayers()
{
	for (PlayersIter iter = g_vPlayers->begin(); iter != g_vPlayers->end(); ++iter)
		delete (*iter);
	g_vPlayers->clear();
	delete g_vPlayers;
}
CPlayer* Globals::GetPlayerByFactionID(short id)
{
	PlayersIter iter = g_vPlayers->begin();
	PlayersIter end  = g_vPlayers->end();
	for (; iter != end; ++iter)
		if((*iter)->GetProfile()->nFactionID == id)
			return (*iter);
	return NULL;
}
//////////////////////////////////////////////////////////////////////////

void Globals::ShutdownHUD()
{
	if (g_pHUD)
	{
		g_pHUD->Shutdown();
		g_pHUD = NULL;
	}
}
void Globals::ShutdownObjManager()
{
	if (g_pObjManager)
	{
		g_pObjManager->Shutdown();
		delete g_pObjManager;
		g_pObjManager = NULL;
	}
}

void Globals::SetCoordDirMaps()
{
	g_CoordToDir[EVEN][TargetDirPair(1, 1)   ] = DIR_NW;
	g_CoordToDir[EVEN][TargetDirPair(0, 2)   ] = DIR_N;
	g_CoordToDir[EVEN][TargetDirPair(0, 1)   ] = DIR_NE;
	g_CoordToDir[EVEN][TargetDirPair(-1, 0)  ] = DIR_E;
	g_CoordToDir[EVEN][TargetDirPair(0, -1)  ] = DIR_SE;
	g_CoordToDir[EVEN][TargetDirPair(0, -2)  ] = DIR_S;
	g_CoordToDir[EVEN][TargetDirPair(1, -1)  ] = DIR_SW;
	g_CoordToDir[EVEN][TargetDirPair(1, 0)   ] = DIR_W;
	g_CoordToDir[ODD] [TargetDirPair (0, 1)  ] = DIR_NW;
	g_CoordToDir[ODD] [TargetDirPair (0, 2)  ] = DIR_N;
	g_CoordToDir[ODD] [TargetDirPair (-1, 1) ] = DIR_NE;
	g_CoordToDir[ODD] [TargetDirPair (-1, 0) ] = DIR_E;
	g_CoordToDir[ODD] [TargetDirPair (-1, -1)] = DIR_SE;
	g_CoordToDir[ODD] [TargetDirPair (0, -2) ] = DIR_S;
	g_CoordToDir[ODD] [TargetDirPair (0, -1) ] = DIR_SW;
	g_CoordToDir[ODD] [TargetDirPair (1, 0)  ] = DIR_W;

	g_AdjTileOffsets[EVEN][DIR_NW] = point(-1, -1);
	g_AdjTileOffsets[EVEN][DIR_N]  = point(0, -2);
	g_AdjTileOffsets[EVEN][DIR_NE] = point(0, -1);
	g_AdjTileOffsets[EVEN][DIR_E]  = point(1, 0);
	g_AdjTileOffsets[EVEN][DIR_SE] = point(0, 1);
	g_AdjTileOffsets[EVEN][DIR_S]  = point(0, 2);
	g_AdjTileOffsets[EVEN][DIR_SW] = point(-1, 1);
	g_AdjTileOffsets[EVEN][DIR_W]  = point(-1, 0);
	g_AdjTileOffsets[ODD] [DIR_NW] = point(0, -1);
	g_AdjTileOffsets[ODD] [DIR_N]  = point(0, -2);
	g_AdjTileOffsets[ODD] [DIR_NE] = point(1, -1);
	g_AdjTileOffsets[ODD] [DIR_E]  = point(1, 0);
	g_AdjTileOffsets[ODD] [DIR_SE] = point(1, 1);
	g_AdjTileOffsets[ODD] [DIR_S]  = point(0, 2);
	g_AdjTileOffsets[ODD] [DIR_SW] = point(0, 1);
	g_AdjTileOffsets[ODD] [DIR_W]  = point(-1, 0);
}

void Globals::InitMenus()
{
	// some options are not menus (play, back)
	MenuOptions options;
	options.push_back(MenuOption(MT_PLAY,	string("Play"),			OptionAction_Play));
	options.push_back(MenuOption(MT_MAIN,	string("Main Menu"),	OptionAction_MainMenu));
	options.push_back(MenuOption(MT_HELP,	string("Help Menu"),	OptionAction_Help));
	options.push_back(MenuOption(MT_OPTIONS,string("Option Menu"),	OptionAction_Options));
	options.push_back(MenuOption(MT_EXIT,	string("Exit"), OptionAction_Exit));

	point pos((g_ptScreenSize.width >> 1) - (11*g_pBitMapFont->GetSize() >> 1), 350 );
	g_pMenus[MT_MAIN]	= new CMenu(-1, pos, MT_MAIN, options, MainMenu::Render, MainMenu::Update, MainMenu::Input);

	options.erase(options.end()-1);
	options.push_back(MenuOption(MT_BACK,	string("Back"),			OptionAction_Back));

	g_pMenus[MT_HELP]	= new CMenu(-1, pos, MT_HELP, options, HelpMenu::Render, HelpMenu::Update, HelpMenu::Input);
	g_pMenus[MT_OPTIONS]= new CMenu(-1, pos, MT_OPTIONS, options, OptionMenu::Render, OptionMenu::Update, OptionMenu::Input);
}
//////////////////////////////////////////////////////////////////////////


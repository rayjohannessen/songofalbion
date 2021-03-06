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
#include "Menu.h"
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
ObjectNamesTypes*		Globals::g_vUnitNames	= NULL;	// initialized in CAssets
CAnimationsManager*		Globals::g_pAnimManager	= NULL;	// initialized in CAssets
CAbilitiesManager*		Globals::g_pAbilitiesManager = NULL;
CGameMenu*					Globals::g_pMenus[NUM_MENUOPTION_TYPES] = {};
CGameMenu*					Globals::g_pMenusInGame[NUM_INGAME_MENU_TYPES] = {};

bool Globals::InitGlobals(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	g_vUnitNames    = new ObjectNamesTypes;
	g_pD3D			= CSGD_Direct3D::GetInstance();
	g_pTM			= CSGD_TextureManager::GetInstance();
	g_pDI			= CSGD_DirectInput::GetInstance();
	g_pFMOD			= CSGD_FModManager::GetInstance();
	g_pAssets		= CAssets::GetInstance();
	g_pAnimManager  = CAnimationsManager::GetInstance();
	g_pAbilitiesManager = CAbilitiesManager::GetInstance();

	g_ptScreenSize = point(nScreenWidth, nScreenHeight);
	g_pD3D->InitDirect3D(hWnd, nScreenWidth, nScreenHeight, bIsWindowed, bIsWindowed);
	g_pDI->InitDirectInput(hWnd, hInstance, DI_KEYBOARD, 0);
	g_pDI->InitDirectInput(hWnd, hInstance, DI_MOUSE, 0);
//	g_pDI->InitDirectInput(hWnd, hInstance, DI_JOYSTICKS, 0);
	g_pDI->AcquireAll();
	g_pTM->InitTextureManager(g_pD3D->GetDirect3DDevice(), g_pD3D->GetSprite());

	g_pAssets->Init();

	g_pBitMapFont	= CBitmapFont::GetInstance();

	if(!g_pFMOD->InitFModManager(hWnd))
		return false;
	else
	{
		g_pAssets->LoadMenuSounds();
		g_pFMOD->Update();
	}

	//////////////////////////////////////////////////////////////////////////
	// assign the directions to their corresponding point values and vice versa
	SetCoordDirMaps();

	g_pAssets->LoadMenuAssets();
	// init menus 
	InitMenus();

	return true;
}
		
void Globals::ShutdownGlobals()
{
	SAFE_SHUTDOWN_SINGLETON(g_pD3D);
	SAFE_SHUTDOWN_SINGLETON(g_pTM);
	SAFE_SHUTDOWN_SINGLETON(g_pDI);
	SAFE_SHUTDOWN_SINGLETON(g_pFMOD);
	SAFE_SHUTDOWN_SINGLETON(g_pAnimManager);
	SAFE_SHUTDOWN_SINGLETON(g_pAssets);
	SAFE_SHUTDOWN_SINGLETON(g_pAbilitiesManager);

	SAFE_DELETE(g_vUnitNames);
	for (unsigned i = 0; i < NUM_MENUOPTION_TYPES; ++i)
		SAFE_DELETE(g_pMenus[i]);
	for (unsigned i = 0; i < NUM_INGAME_MENU_TYPES; ++i)
		SAFE_DELETE(g_pMenusInGame[i]);
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
	SAFE_SHUTDOWN_SINGLETON(g_pHUD);
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
	// play option is not a menu

	//////////////////////////////////////////////////////////////////////////
	// menus out-of-game
	int hoverID = Globals::g_pAssets->GetGUIasts()->MenuGlows();
	int menuClick = Globals::g_pAssets->GetMenuSnds()->MenuClickSnd();
	MenuOptions options;
	options.push_back(MenuOption(MOT_PLAY,	string("Play"),		OptionAction_Play,	menuClick,	rect(317, 531, 258, 459), Globals::g_pAssets->GetMenuSnds()->MenuHoverSnd(MOT_PLAY), rect(0, 341, 425, 692), hoverID, point(236, 261)));
	options.push_back(MenuOption(MOT_MAIN,	string("Main"),		OptionAction_MainMenu, menuClick));
	options.push_back(MenuOption(MOT_HELP,	string("Help"),		OptionAction_Help,	menuClick,	rect(235, 317, 171, 291), Globals::g_pAssets->GetMenuSnds()->MenuHoverSnd(MOT_HELP), rect(367, 502, 815, 965), hoverID, point(154, 216)));
	options.push_back(MenuOption(MOT_OPTIONS,string("Options"),	OptionAction_Options,menuClick,rect(326, 556, 792, 980),  Globals::g_pAssets->GetMenuSnds()->MenuHoverSnd(MOT_OPTIONS), rect(0, 363, 707, 1024), hoverID, point(708, 256)));
	options.push_back(MenuOption(MOT_EXIT,	string("Exit"),		OptionAction_Exit,	menuClick,	rect(460, 567, 515, 743), Globals::g_pAssets->GetMenuSnds()->MenuHoverSnd(MOT_EXIT), rect(0, 254, 0, 412), hoverID, point(416, 380)));

	point pos((g_ptScreenSize.width >> 1) - (11*g_pBitMapFont->GetSize() >> 1) + 50, 330 );	// 11 is the size of the longest menu name string
	int menuMusic = Globals::g_pAssets->GetMenuSnds()->MenuMusic(MOT_MAIN);
	g_pMenus[MOT_MAIN]	 = new CGameMenu(Globals::g_pAssets->GetGUIasts()->MenuBGs(MOT_MAIN), rect(0,768,0,1024),
									 menuMusic, pos, MOT_MAIN, options, MainMenu::Render, MainMenu::Update, MainMenu::Input, false);
	g_pMenus[MOT_HELP]	 = new CGameMenu(Globals::g_pAssets->GetGUIasts()->MenuBGs(MOT_HELP), rect(0,768,0,1024),
									menuMusic, pos, MOT_HELP, options, HelpMenu::Render, HelpMenu::Update, HelpMenu::Input);
	g_pMenus[MOT_OPTIONS]= new CGameMenu(Globals::g_pAssets->GetGUIasts()->MenuBGs(MOT_OPTIONS), rect(0,446,0,711),
									menuMusic, pos, MOT_OPTIONS, options, OptionMenu::Render, OptionMenu::Update, OptionMenu::Input);

	//////////////////////////////////////////////////////////////////////////
	// menus in-game (gameplay state)
	MenuOptions optionsInGame;
	optionsInGame.push_back(MenuOption(MOT_PLAY,	string("Resume"),	OptionAction_Resume,	menuClick));
	optionsInGame.push_back(MenuOption(MOT_HELP,	string("Help"),		OptionAction_Help,		menuClick));
	optionsInGame.push_back(MenuOption(MOT_OPTIONS, string("Options"),	OptionAction_Options,	menuClick));
	optionsInGame.push_back(MenuOption(MOT_MAIN,	string("Main"),		OptionAction_MainMenu,	menuClick));
	optionsInGame.push_back(MenuOption(MOT_EXIT,	string("Exit"),		OptionAction_Exit,		menuClick));
	// these need an offset for the fact that there's no main menu
	g_pMenusInGame[MOT_HELP-1]	  = new CGameMenu(Globals::g_pAssets->GetGUIasts()->MenuBGs(MOT_HELP), rect(0,768,0,1024), menuMusic, pos, MOT_HELP, optionsInGame, HelpMenu::Render, HelpMenu::Update, HelpMenu::Input);
	g_pMenusInGame[MOT_OPTIONS-1] = new CGameMenu(Globals::g_pAssets->GetGUIasts()->MenuBGs(MOT_OPTIONS), rect(0,446,0,711),  menuMusic, pos, MOT_OPTIONS, optionsInGame, OptionMenu::Render, OptionMenu::Update, OptionMenu::Input);
}
//////////////////////////////////////////////////////////////////////////


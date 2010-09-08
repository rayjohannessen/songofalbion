#pragma once

#include "MenuDefines.h"
class CBitmapFont;
class CAssets;
class CSGD_DirectInput;
class CSGD_Direct3D;
class CSGD_TextureManager;
class CSGD_FModManager;
class CPlayer;
class CHUD;
class CGame;
class CMap;
class ObjectManager;
class CPlayer;
class CAnimationsManager;
class CUnit; 
class CAbilitiesManager;
class CMenu;
class DebugWindow;
enum eAnimationDirections;

struct ObjectData 
{
	string	Name;
	int		Type;
	ObjectData() {}
};

typedef string ObjectName;
typedef string AbilityName;
typedef int UnitType;
typedef vector<ObjectData> ObjectNamesTypes;
typedef pair<int, int> TargetDirPair;
typedef map<pair<int, int>, eAnimationDirections> CoordToDirMap;
typedef map<eAnimationDirections, point> DirToCoordMap;
typedef vector<CPlayer*> Players;

class Globals
{
	typedef vector<CPlayer*>::iterator	PlayersIter;
	static unsigned m_nCurrPlayerInd;

	static void SetCoordDirMaps();
public:

	static CoordToDirMap			g_CoordToDir[2];
	static DirToCoordMap			g_AdjTileOffsets[2];
	static CSGD_Direct3D*			g_pD3D;
	static CSGD_TextureManager*		g_pTM;
	static CSGD_DirectInput*		g_pDI;
	static CSGD_FModManager*		g_pFMOD;
	static CBitmapFont*				g_pBitMapFont;
	static CAssets*					g_pAssets;
	static CHUD*					g_pHUD;
	static CGame*					g_pGame;
	static CMap*					g_pMap;
	static ObjectManager*			g_pObjManager;
	static Players*					g_vPlayers;
	static CPlayer*					g_pCurrPlayer;
	static ObjectNamesTypes*		g_vUnitNames;
	static CAnimationsManager*		g_pAnimManager; 	// initialized in CAssets
	static CAbilitiesManager*		g_pAbilitiesManager;
	static CMenu*					g_pMenus[NUM_MENUOPTION_TYPES];
	static CMenu*					g_pMenusInGame[NUM_INGAME_MENU_TYPES];
	static short					g_nNumPlayers;
	static short					g_nPlayerFactionID;
	static bool						g_bWindowOpen;
	static point					g_ptQBPos;
	static point					g_ptScreenSize;
	
	static bool InitGlobals(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);

	static void InitMenus();
	static bool InitHUD();
	static bool InitObjManager();
	static bool InitPlayers(short numPlayers);
	static void ShutdownHUD();
	static void ShutdownObjManager();
	static void ClearPlayers();
	static void ShutdownGlobals();
	static void GotoNextPlayer();

	// ACCESSORS
	static CPlayer* GetPlayerByFactionID(short id);
	static vector<CPlayer*>& GetPlayers()	{ return *g_vPlayers;	}
	static CPlayer* GetCurrPlayer()			{ return g_pCurrPlayer;	}

	// MUTATORS
	static void SetCurrPlayer(unsigned ind)	{ m_nCurrPlayerInd = ind; g_pCurrPlayer = (*g_vPlayers)[m_nCurrPlayerInd]; }
};
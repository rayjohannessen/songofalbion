#pragma once

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
class CBaseMenuState;
class DebugWindow;
enum eAnimationDirections;

struct UnitData 
{
	string	Name;
	int		Type;
	UnitData() {}
};

typedef string ObjectName;
typedef string AbilityName;
typedef int UnitType;
typedef vector<UnitData> UnitNamesTypes;
typedef pair<int, int> TargetDirPair;
typedef map<pair<int, int>, eAnimationDirections> CoordToDirMap;
typedef map<eAnimationDirections, point> DirToCoordMap;
typedef vector<CPlayer*> Players;

enum eMenus { M_MAIN, M_HELP, M_OPTIONS, NUM_MENUS };

class Globals
{
	typedef vector<CPlayer*>::iterator	PlayersIter;
	static unsigned m_nCurrPlayerInd;

	static void SetCoordDirMaps();
public:

// 	typedef string ObjName;
// 	typedef vector<pair<eButtonSlot, eButtonName>> ButtonNamesVec;
// 	typedef pair<eButtonSlot, eButtonName> ButtonPair;
// 	typedef map<ObjName, vector<pair<eButtonSlot, eButtonName>>> ObjectButtonsMap;
// 	typedef map<ObjName, vector<pair<eButtonSlot, eButtonName>>>::iterator ObjButtonsIter;

//	static ObjectButtonsMap*		g_pObjButtonsMap = NULL;
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
	static UnitNamesTypes*			g_vUnitNames;
	static CAnimationsManager*		g_pAnimManager; 	// initialized in CAssets
	static CAbilitiesManager*		g_pAbilitiesManager;
	static CBaseMenuState*			g_pMenus[NUM_MENUS];
	static short					g_nNumPlayers;
	static short					g_nPlayerFactionID;
	static bool						g_bWindowOpen;
	static point					g_ptQBPos;
	
	static bool InitGlobals(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);

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
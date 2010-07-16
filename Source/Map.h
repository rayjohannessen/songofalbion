#ifndef MAP_H
#define MAP_H

#include "Timer.h"
#include "BattleManager.h"
#include "Pathfinding.h"
using namespace Pathfinding;

class CUnit;
class CGame;
class CPlayer;
class CObject;
class CTile;
class CFreeTile;
enum eAnimationDirections;
enum MapMovementFlags {FLAG_NONE, FLAG_COLLISION, FLAG_ENEMY_SPAWN, };
enum Quadrants {TOP_LEFT, TOP_RIGHT, BTM_LEFT, BTM_RIGHT,};

#define SCROLL_DELAY 0.05f
#define SCROLL_AREA 10
#define X_OS 17
#define Y_OS 55

enum eMapInputRet {MIR_EXIT, MIR_NEXTPLAYER, MIR_CONTINUE, };
enum eCardinalDirections {N, S, E, W,};
enum {MSA_CITY, MSA_UNIT, MSA_BUILDING, MSA_ADDING, MSA_REMOVE, MSA_SELECT, };
enum eMapFlags { MF_CENTER_MAP, MF_CENTERING, 
				 MF_CENTER_DIR_N, MF_CENTER_DIR_S, MF_CENTER_DIR_E, MF_CENTER_DIR_W,
				 MF_MOUSE_BELOW_BTM, MF_CENTER_DIR_END = MF_MOUSE_BELOW_BTM, MF_MOUSE_IN_QUICK_BAR, 
				 MF_DO_SCROLL_N, MF_DO_SCROLL_S, MF_DO_SCROLL_E, MF_DO_SCROLL_W, MF_DO_SCROLL_END,
				 MF_IN_SCROLL_AREA_N, MF_IN_SCROLL_AREA_S, MF_IN_SCROLL_AREA_E, MF_IN_SCROLL_AREA_W, MF_IN_SCROLL_AREA_END,
				 MF_ATTACKING,
				 MF_MOVING,};

class CMap
{
	typedef map<unsigned char, eAnimationDirections> InputToDirMap;
	typedef InputToDirMap::iterator InputToDirMapIter;
	InputToDirMapIter m_InputMapEndIter, m_InputMapIter;
	struct sTileset 
	{
		string name;
		int id;
	};
	sTileset m_pTilesets[4];	// tilesets used for currently rendering map

	// characters, by default, render behind the OBJECTS layer (CHARACTER_BEHIND)
	//DEPTH depth;
	
	CGame*			m_pGame;

	// TODO::temp
	int m_nCurrPlaceType;	// used to place different objects with the mouse
	int m_nCurrFactionPlace;
	CTriggerTimer* m_pTimer;	// used for scrolling
	int frames;
	// attacks
	CTimer m_AttkTimer;
	CBattleManager m_BattleMngr;
	int m_nScreenWidth;
	int m_nScreenHeight;
	Path m_vPath;

	// Mouse variables
	int   m_nCurrMouseID;
	point m_ptMouseScrn;
	point m_ptMouseWrld;
	point m_ptCurrMouseTile;

	// objects
	CObject* m_pCurrHoverObject;
	CObject* m_pCurrPlayerSelectedObj;
	CObject* m_pEnemyObj;
	CObject* placeObj;

	// Map variables
	int m_nMapFlags;

	string m_strCurrVersion;
	int m_nMapWidth;
	int m_nMapHeight;
	int m_nNumCols;
	int m_nNumRows;
	int m_nTotalNumTiles;
	int m_nCurrSelectedTile;	// where the mouse cursor is at on the map (tile ID)

	int m_nOSx;	// offsets to account for the HUD frame
	int m_nOSy;
	double m_fScrollX;			// the current scroll of the camera
	double m_fScrollY;
	pointf m_ptTotalMapScroll;	// final amount the map has scrolled, used to update objects (dt has been applied)
	int m_nMaxScrollX;			// how far the camera can be scrolled
	int m_nMaxScrollY;

	CTile* m_pTilesL1;			// ground tiles
	CTile* m_pTilesL2;			// added terrain tiles
	CFreeTile* m_pFreeTiles;	// any floating objects
	int m_nFreeTileCount;
	int m_nFTosX;
	int m_nFTosY;

	rect m_rViewport;			// the visible portion of the screen minus the HUD (screen space values)

	void LoadMap(string fileName);

//////////////////////////////////////////////////////////////////////////
// Input
	point GetMouseTile(point);

	//////////////////////////////////////////////////////////////////////////
	// handling various input
	bool HandleKBInput();
	void HandleMouseInput();
	void HandleViewScroll( POINT &mouse, double fElapsedTime );
	void HandleMovement( double fElapsedTime, POINT& mouse );

	void DoMove( int newX, int newY, eAnimationDirections facing );
	void FindNewCoord( int facing, int &newY, int &newX );
	CObject* HoverObject(point& coord);
	void Deselect(CObject*& obj);
	void Select(CObject* const obj);
	bool DetermineMoveSpecifics(const point& pt);
	int  DetermineTotalMoveCost(bool attacking);
//////////////////////////////////////////////////////////////////////////
// Drawing map functions - private

	// offsets
	void SetOffsetX(int os) {m_nOSx = os;}
	void SetOffsetY(int os) {m_nOSy = os;}
	void SetFTosX(int os)	{m_nFTosX = os;}
	void SetFTosY(int os)	{m_nFTosY = os;}

	// scrolling
	void ScrollMapUp(double fElapsedTime);
	void ScrollMapDown(double fElapsedTime);
	void ScrollMapLeft(double fElapsedTime);
	void ScrollMapRight(double fElapsedTime);

	void CenterMap( double fElapsedTime );

	void DrawTiles();
//////////////////////////////////////////////////////////////////////////

	// make it a singleton
	CMap();
	~CMap(){}
	CMap(const CMap&);
	CMap& operator= (const CMap&);
public:

	static CMap* GetInstance();

	void NewMap();

	///////////////////////////////////////////////////////////////////
	//	Function:	Init
	//
	//	Purpose:	Init and initialize the current state
	//////////////////////////////////////////////////////////////////
	void Init(int screenWidth, int screenHeight);
	//////////////////////////////////////////////////////////////////
	//	Function: Shutdown
	//
	//	Purpose: Cleans up all the state info
	///////////////////////////////////////////////////////////////////
	void Shutdown(void);

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Render the Map and all objects on it
	//////////////////////////////////////////////////////////////////////////
	void Render();

	////////// DEBUG /////////////////////////////////////////////////////////
	void DrawDebug();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	IsoTilePlot
	//
	//	Purpose		:	Determines at which pixel on the screen the given map
	//					ID (xID, yID = pt.x, pt.y) should be drawn
	//
	//	Return		:	The pixel point to draw at
	//////////////////////////////////////////////////////////////////////////
	point IsoTilePlot(const point& pt) const;

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	DrawPlacingObj
	//
	//	Purpose		:	When the player is choosing a tile to place a newly
	//					acquired object, a temporary one is rendered at the 
	//					current cursor tile
	//////////////////////////////////////////////////////////////////////////
	void DrawPlacingObj();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Update the Map
	//////////////////////////////////////////////////////////////////////////
	void Update(double fElapsedTime);

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for the map, mouse and/or keyboard 
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	eMapInputRet Input(double, POINT&);

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	//////////////////////////////////////////////////////////////////////////
	inline void ToggleMapFlagOff(eMapFlags flag)	{ BIT_OFF(m_nMapFlags, flag);	}
	inline void ToggleMapFlagOn(eMapFlags flag)		{ BIT_ON(m_nMapFlags, flag);	}
	void SelectObj(CObject*& obj, bool deselectAll = false);
	// deselects the obj on the map if it is selected (e.g., if a unit is destroyed that is selected)
	void ActionIfSelected(CObject* obj);
	void InitiateAttack();

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	inline CObject* GetSelectedObject()			{ return m_pCurrPlayerSelectedObj;  }
	inline CObject* GetTarget()					{ return m_pEnemyObj;		 }
	inline CObject* GetHoverObject()			{ return m_pCurrHoverObject; }
	inline const rect& GetViewport()   			{ return m_rViewport;		 }
	inline const pointf* GetTotalFrameScroll()	{ if (m_ptTotalMapScroll.x != 0.0f || m_ptTotalMapScroll.y != 0.0f) return &m_ptTotalMapScroll; else return NULL;}
	inline int GetScreenWidth()		const		{ return m_nScreenWidth;	 }
	inline int GetScreenHeight()	const		{ return m_nScreenHeight;	 }
	inline CTile* GetL1Tiles()		const		{ return m_pTilesL1;		 }
	inline CTile* GetL2Tiles()		const		{ return m_pTilesL2;		 }
	inline int	  GetNumCols()		const		{ return m_nNumCols;		 }
	inline int	  GetNumRows()		const		{ return m_nNumRows;		 }
	inline point  GetNumColsRows()	const		{ return point(m_nNumCols, m_nNumRows);	}
};

#endif MAP_H
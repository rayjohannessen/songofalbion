#include "stdafx.h"

#include "Map.h"
#include "Player.h"
#include "City.h"
#include "Unit.h"
#include "Building.h"
#include "Assets.h"
#include "HUD.h"
#include "BitmapFont.h"
#include "Tile.h"
#include "Game.h"
#include "Globals.h"
#include "ObjectManager.h"
#include "AnimationDefinesEnums.h"
#include "CombatSkill.h"

#if _DEBUG
	static bool debug = true;	// draw debug info in debug mode only
	static point lastCoordClicked;
#else
	static bool debug = false;
#endif

const float DEFAULT_SCROLL_SPEED = 435.0f;	// an option for the user to set this will be given
const float DEFAULT_CENTER_SPEED = 500.0f;
static map<unsigned char, eAnimationDirections> gInputToDirection;	// each DIK code maps to a direction, used for movement

CMap::CMap() :
	m_nMapFlags(0),
	m_strCurrVersion("TED-Version-1.1"),
	m_nCurrPlaceType(MSA_SELECT),
	m_nCurrFactionPlace(0),
	frames(0),
	m_nOSx(X_OS),
	m_nOSy(Y_OS),
	m_pTimer(NULL),
	m_pVictor(NULL)
{
	//////////////////////////////////////////////////////////////////////////
	// TODO:: temps, make sure to remove
	m_nCurrPlaceType = MSA_SELECT;
	m_nCurrFactionPlace = 0;
	//////////////////////////////////////////////////////////////////////////
	// assign the map keys their corresponding directions
	gInputToDirection[DIK_NUMPAD1] = DIR_SW;
	gInputToDirection[DIK_NUMPAD2] = DIR_S;
	gInputToDirection[DIK_NUMPAD3] = DIR_SE;
	gInputToDirection[DIK_NUMPAD4] = DIR_W;
	gInputToDirection[DIK_NUMPAD6] = DIR_E;
	gInputToDirection[DIK_NUMPAD7] = DIR_NW;
	gInputToDirection[DIK_NUMPAD8] = DIR_N;
	gInputToDirection[DIK_NUMPAD9] = DIR_NE;
	m_InputMapEndIter = gInputToDirection.end();
}

CMap* CMap::GetInstance()
{
	static CMap instance;
	return &instance;
}


void CMap::NewMap()
{

}

point CMap::IsoTilePlot(const point& pt) const
{
	point newPt( pt.x * g_TileWidth + (pt.y & 1) * (g_TileWidth >> 1) + m_nOSx - (int)m_fScrollX,
					pt.y * (g_TileHeight >> 1) + m_nOSy - (int)m_fScrollY);
	return newPt;
}

void CMap::Render()
{
	DrawTiles();
	DrawPlacingObj();

// 	if (debug)
// 		DrawDebug();

	////////////////////////////////////////////////////////////////////////// 
	// draw the current mouse image
	Globals::g_pTM->DrawWithZSort(m_nCurrMouseID, m_ptMouseScrn.x, m_ptMouseScrn.y, 0.0f, 1.0f, 1.0f);
}
void CMap::DrawTiles()
{
	int tileID; BIT_OFF(m_nMapFlags, MF_MOUSE_BELOW_BTM);
	for (int y = 0; y < m_nNumRows; ++y)
	{
		for (int x = 0; x < m_nNumCols; ++x)
		{
			tileID = y * m_nNumCols + x;	// convert to 1D array
			point mapPt(x, y);

			//////////////////////////////////////////////////////////////////////////
			// not necessarily used in the final build
			if (mapPt == m_ptCurrMouseTile)
				m_pTilesL1[tileID].SetAlpha(100);	// mouse is hovering over this tile
			else
				m_pTilesL1[tileID].SetAlpha(255);
			//////////////////////////////////////////////////////////////////////////

			mapPt = IsoTilePlot(mapPt);				// find the screen coords to draw to
			
			if (mapPt.x > m_nScreenWidth || mapPt.y < (-g_TileHeight) )	
				break;	// skip to next row if this tile's going to draw off the screen
			else if (mapPt.x < (-g_TileWidth) )
				continue;	// try the next tile...
			else if (mapPt.y > m_nScreenHeight + g_TileHeight)
			{ BIT_ON(m_nMapFlags, MF_MOUSE_BELOW_BTM); break; }	// no need to draw any more tiles (still draw any free-placed tiles)

			if (m_pTilesL1[tileID].DestXID() > -1)
				Globals::g_pTM->DrawWithZSort( m_pTilesL1[tileID].ImageID(), mapPt.x, mapPt.y, DEPTH_TILES, 1.0f, 1.0f, 
					m_pTilesL1[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, m_pTilesL1[tileID].Color());
			if (m_pTilesL2[tileID].DestXID() > -1)
				Globals::g_pTM->DrawWithZSort( m_pTilesL2[tileID].ImageID(), mapPt.x, mapPt.y, DEPTH_TILES, 1.0f, 1.0f,
					m_pTilesL2[tileID].SourceRect(), 0.0f, 0.0f, 0.0f, m_pTilesL2[tileID].Color());
		}
		if (BIT_TEST_ON(m_nMapFlags, MF_MOUSE_BELOW_BTM))
			break;
	}
	for (int i = 0; i < m_nFreeTileCount; ++i)	// TODO:: draw with z?
	{
		Globals::g_pTM->Draw( m_pFreeTiles[i].ImageID(), m_pFreeTiles[i].DestX(), m_pFreeTiles[i].DestY(), 1.0f, 1.0f, 
			m_pFreeTiles[i].SourceRect(), /*0, 0, m_pFreeTiles[i].Rotation(),*/ D3DCOLOR_ARGB(m_pFreeTiles[i].Alpha(), 255, 255, 255));
	}
}

void CMap::DrawPlacingObj()
{
	if (m_pPlaceObj)
	{
		m_pPlaceObj->SetScrnPos(IsoTilePlot(m_ptCurrMouseTile));
		m_pPlaceObj->Render(m_rViewport);
	}
}
void CMap::Update(double dElapsedTime)
{
	// if the timer's target time amount has been reached, we know we need to start scrolling
	if(m_pTimer->IsTimerRunning() && m_pTimer->Update(dElapsedTime))
	{
		for (int i = MF_IN_SCROLL_AREA_N, scrollInd = MF_DO_SCROLL_N; i < MF_IN_SCROLL_AREA_END; ++i, ++scrollInd)
			if (BIT_TEST_ON(m_nMapFlags, i))
				BIT_ON(m_nMapFlags, scrollInd);
	}

	if (m_BattleMngr.Update(dElapsedTime))
		BIT_OFF(m_nMapFlags, MF_ATTACKING); // the battle is complete

	if (BIT_TEST_ON(m_nMapFlags, MF_CENTER_MAP))
		CenterMap(dElapsedTime);
}

eMapInputRet CMap::Input(double fElapsedTime, POINT& mouse)
{
	m_ptTotalMapScroll = pointf(0.0f, 0.0f);
	m_ptMouseScrn = m_ptMouseWrld = mouse;

	if (Globals::g_bWindowOpen)	// only move mouse if a window is open
		return MIR_CONTINUE;

	m_ptMouseWrld.Offset((int)m_fScrollX, (int)m_fScrollY);
	m_ptCurrMouseTile = GetMouseTile(m_ptMouseWrld);

	if (BIT_TEST_ON(m_nMapFlags, MF_ATTACKING) || BIT_TEST_ON(m_nMapFlags, MF_MOVING))
		return MIR_CONTINUE;	// do not accept input in these cases

	// TODO:: need to get specific buttons differently, they might be in different spots for each unit/object
	// disband unit / destroy building
	if ( Globals::g_pDI->KeyDown(DIK_LCONTROL) && Globals::g_pDI->KeyPressed(DIK_D) )
	{
		if (m_pCurrPlayerSelectedObj && m_pCurrPlayerSelectedObj->GetType() == OBJ_UNIT)
			Globals::g_pHUD->GetButton(BL_SLOT_2_2)->SimulatePressed();
	}
	else
		// scrolling - keyboard & mouse
		HandleViewScroll(mouse, fElapsedTime);

	// unit movement
	HandleMovement(fElapsedTime, mouse);

	if (!HandleKBInput())
		return MIR_NEXTPLAYER;

	if (NULL != (m_pCurrHoverObject = HoverObject(m_ptCurrMouseTile)) )
	{
		m_ptCurrMouseTile = m_pCurrHoverObject->GetCoord();	// set mouse to hover object's tile NO MATTER WHAT
		m_pCurrHoverObject->SetHovered(true);
		if (m_pCurrHoverObject->GetFactionID() == Globals::GetCurrPlayer()->GetProfile()->nFactionID)
			m_pCurrHoverObject->SetColor(DARKBLUE);
		else
			m_pCurrHoverObject->SetColor(DARKRED);
	}
	HandleMouseInput();

	return MIR_CONTINUE;
}

void CMap::HandleMovement( double fElapsedTime, POINT& mouse )
{
	// handle movement, only if there is a selected object (and it's a unit)
	if ( m_pCurrPlayerSelectedObj && m_pCurrPlayerSelectedObj->GetType() == OBJ_UNIT )
	{
		// if we have a valid direction input key that is pressed, start to make the move
		if ((m_InputMapIter = gInputToDirection.find(Globals::g_pDI->GetBufferedDIKCodeEx())) != m_InputMapEndIter)
		{
			eAnimationDirections facing = m_InputMapIter->second;
			int newX = m_pCurrPlayerSelectedObj->GetCoord().x;
			int newY = m_pCurrPlayerSelectedObj->GetCoord().y;		
			FindNewCoord(facing, newY, newX);

			DetermineMoveSpecifics(point(newX, newY));
		}
	}
}
bool CMap::DetermineMoveSpecifics(const point& pt)
{
	// determine if there's an enemy there, if so, see if it's already selected or not
	CUnit* pTarget = NULL;
	if ( (pTarget = Globals::g_pObjManager->IsSpaceOccupied(pt)) )
	{
		if ( !m_pEnemyObj )	// there's not currently a target, so just select as new target
		{
			m_pEnemyObj = pTarget;
			m_pEnemyObj->SetColor(DARKRED);
			m_pEnemyObj->SetHovered(true);
		}
		else
		{
			if (pTarget != m_pEnemyObj)	// in case there's already a target selected, simply select it (NO attack) and deselect current
			{	// deselect current target
				m_pEnemyObj->SetColor(WHITE);
				m_pEnemyObj->SetHovered(false);
				// set the new target
				m_pEnemyObj = pTarget;
				m_pEnemyObj->SetColor(DARKRED);
				m_pEnemyObj->SetHovered(true);
			}
			else	// otherwise, the current target was clicked again, begin pathfinding to determine if reachable
			{
				// find the path
				m_pCurrPlayerSelectedObj->FindPathToTarget(pt, m_vPath);
				if (m_vPath.size())
				{
// TODO:: find and display path(s) available when unit is first clicked
					CUnit* unit = ((CUnit*)m_pCurrPlayerSelectedObj);
					if (m_vPath[0]->DestID() != m_pCurrPlayerSelectedObj->GetCoord())	// make sure the units aren't on the same tile first
					{
						if (DetermineTotalMoveCost(true) <= unit->GetStamina())
						{
							unit->SetNewPath(&m_vPath);
							return true;
						}
					} 
					else	// same tile, just start attacking now
					{
						if (unit->GetStamina() >= unit->GetCurrAttackAbility()->GetCombatProps().AttackStam)
						{
							m_vPath.clear();
							InitiateAttack(false);
							unit->SetNeighbor(((CUnit*)m_pEnemyObj));
							((CUnit*)m_pEnemyObj)->SetNeighbor(unit);
						}
					}
				}
			}
		}
	}
	else	// we're just moving to an empty space, begin pathfinding to determine if reachable
	{
		// find the path
		m_pCurrPlayerSelectedObj->FindPathToTarget(pt, m_vPath);
		if (m_vPath.size())
		{
			// TODO:: find and display path(s) available when unit is first clicked
			CUnit* unit = ((CUnit*)m_pCurrPlayerSelectedObj);
			if (DetermineTotalMoveCost(false) <= unit->GetStamina())
			{
				unit->SetNewPath(&m_vPath);
				return true;
			}
		}
	}
	return false;
}

CObject* CMap::HoverObject(point& coord)
{
	// reset the color of the currently hovered object, if any
	if ( m_pCurrHoverObject && m_pCurrHoverObject != m_pCurrPlayerSelectedObj && m_pCurrHoverObject != m_pEnemyObj)
		Deselect(m_pCurrHoverObject);

	// determine if a city, unit, or building has been selected,
	// if so change its color and return that object
	for (unsigned player = 0; player < Globals::GetPlayers().size(); ++player)
	{
		CPlayer* pPlayer = Globals::GetPlayers()[player];
		CUnit* units = pPlayer->GetUnits();
		for (int i = 0; i < pPlayer->GetNumUnits(); ++i)
			if (units[i].MouseInRect(m_ptMouseScrn))
				if ((CObject*)&units[i] != m_pCurrPlayerSelectedObj)
					return (CObject*)&units[i]; 

		CCity* cities = pPlayer->GetCities();
		for (int i = 0; i < pPlayer->GetNumCities(); ++i)
			if (coord == cities[i].GetCoord())
				if((CObject*)&cities[i] != m_pCurrPlayerSelectedObj)
					return (CObject*)&cities[i];

		CBuilding* bldngs = pPlayer->GetBuildings();
		for (int i = 0; i < pPlayer->GetNumBldngs(); ++i)
			if (coord == bldngs[i].GetCoord())
				if ((CObject*)&bldngs[i] != m_pCurrPlayerSelectedObj)
					return (CObject*)&bldngs[i]; 
	}
	return NULL;	// no objects are being hovered
}

void CMap::Init(int screenWidth, int screenHeight)
{
	m_nMapFlags = 0;
	m_nScreenWidth = screenWidth; m_nScreenHeight = screenHeight;
	m_rViewport = rect(55, m_nScreenHeight-125, 0, m_nScreenWidth);
	LoadMap("Resources/Map/test.dat");
	m_nCurrMouseID = Globals::g_pAssets->GetGUIasts()->Mouse();

	m_pPlaceObj = m_pEnemyObj = m_pVictor = NULL;
	m_pCurrHoverObject = NULL;

	m_pTimer = new CTriggerTimer(false, false);
}

void CMap::Shutdown()
{
	SAFE_DELETE_ARRAY(m_pTilesL1);
	SAFE_DELETE_ARRAY(m_pTilesL2);
	SAFE_DELETE_ARRAY(m_pFreeTiles);
	SAFE_DELETE(m_pPlaceObj);
	SAFE_DELETE(m_pTimer);
}

point CMap::GetMouseTile(point mouse)
{
	mouse.Offset(-m_nOSx, -m_nOSy);
	point ptCurrMouseTileID;
	int hWidth, hHeight; hWidth = (g_TileWidth >> 1); hHeight = (g_TileHeight >> 1);
	// first find which rectangle the point's in:
	int rectIDx, rectIDy;
	ptCurrMouseTileID.x = rectIDx = mouse.x / g_TileWidth; ptCurrMouseTileID.y = rectIDy = mouse.y / g_TileHeight;
	if (ptCurrMouseTileID.y > 0)
		ptCurrMouseTileID.y = (ptCurrMouseTileID.y << 1);
	// now find which quadrant it's in based upon the point's relative position in this rect
	int adjX, adjY;
	adjX = mouse.x - (rectIDx * g_TileWidth); adjY = mouse.y - (rectIDy * g_TileHeight);
	int quadrant = TOP_LEFT;
	if (adjX >= hWidth)
		quadrant = TOP_RIGHT;
	if (adjY >= hHeight)
	{
		if (quadrant > 0)
			quadrant = BTM_RIGHT;
		else
			quadrant = BTM_LEFT;
	}
	// now determine if the point is inside or outside the actual tile based upon which quadrant it's in
	int result = -1;
	switch (quadrant)
	{
	case BTM_RIGHT:   // for bottoms, if result > 0, the point is outside the tile
		{
			float d = (float)(hHeight - g_TileHeight) / (float)(g_TileWidth - hWidth);
			result = adjY - g_TileHeight - (int)(d * (float)(adjX - hWidth));
			if (result > 0)
				++ptCurrMouseTileID.y;
		}
		break;
	case BTM_LEFT:
		{
			float d = (float)(hHeight - g_TileHeight) / (float)(-hWidth);
			result = adjY - g_TileHeight - (int)(d * (float)(adjX - hWidth));
			if (result > 0)
			{ --ptCurrMouseTileID.x; ++ptCurrMouseTileID.y; }
		}
		break;
	case TOP_RIGHT:   // for tops, if result is < 0, the point is outside the tile
		{
			float d = (float)hHeight / (float)(g_TileWidth - hWidth);
			result = adjY - (int)(d * (float)(adjX - hWidth));
			if (result < 0)
				--ptCurrMouseTileID.y;
		}
		break;
	case TOP_LEFT:
		{
			float d = (float)hHeight / (float)(-hWidth);
			result = adjY - (int)(d * (float)(adjX - hWidth));
			if (result < 0)
			{ --ptCurrMouseTileID.x; --ptCurrMouseTileID.y; }
		}
		break;
	}
	if (ptCurrMouseTileID.x < 0)
		ptCurrMouseTileID.x = 0;
	if (ptCurrMouseTileID.y < 0)
		ptCurrMouseTileID.y = 0;
	if (ptCurrMouseTileID.x > m_nNumCols - 1)
		ptCurrMouseTileID.x = m_nNumCols - 1;
	if (ptCurrMouseTileID.y > m_nNumCols - 1)
		ptCurrMouseTileID.y = m_nNumCols - 1;
	return ptCurrMouseTileID;
}
void CMap::ScrollMapUp(double fTimeStep)
{
	m_ptTotalMapScroll.y = (DEFAULT_CENTER_SPEED * (float)fTimeStep);
	m_fScrollY -= m_ptTotalMapScroll.y;
	if (m_fScrollY < 0.0f)
	{
		m_fScrollY = 0.0f;
		m_ptTotalMapScroll.y = 0.0f;
	}
}
void CMap::ScrollMapDown(double fTimeStep)
{
	m_ptTotalMapScroll.y = -(DEFAULT_CENTER_SPEED * (float)fTimeStep);
	m_fScrollY -= m_ptTotalMapScroll.y;
	if (m_fScrollY > m_nMaxScrollY)
	{
		m_fScrollY = (float)m_nMaxScrollY;
		m_ptTotalMapScroll.y = 0.0f;
	}
}
void CMap::ScrollMapLeft(double fTimeStep)
{
	m_ptTotalMapScroll.x = (DEFAULT_CENTER_SPEED * (float)fTimeStep);
	m_fScrollX -= m_ptTotalMapScroll.x;
	if (m_fScrollX < 0.0f)
	{
		m_fScrollX = 0.0f;
		m_ptTotalMapScroll.x = 0.0f;
	}
}
void CMap::ScrollMapRight(double fTimeStep)
{
	m_ptTotalMapScroll.x = -(DEFAULT_CENTER_SPEED * (float)fTimeStep);
 	m_fScrollX -= m_ptTotalMapScroll.x;
	if (m_fScrollX > m_nMaxScrollX)
	{
		m_fScrollX = (float)m_nMaxScrollX;
		m_ptTotalMapScroll.x = 0.0f;
	}
}
void CMap::LoadMap(string fileName)
{
	ifstream ifs;
	ifs.exceptions(~ios_base::goodbit);

	try
	{
		ifs.open(fileName.c_str(), ios_base::in | ios_base::binary);
	}
	catch(ios_base::failure &)
	{
		if (!ifs.is_open())
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Failed to open file: %s", fileName );
			MessageBox(0, szBuffer, "Incorrect file name.", MB_OK);
//			m_pGame->ChangeState(CMainMenuState::GetInstance());
		}
		if (ifs.eof())
		{ifs.close();return;}		
	}
	//read input from the given binary file
	string version, eat, name, tilesetName, fName;
	char file[256];
	char buff[256];
	ZeroMemory(buff, 256);
	byte size;
	ifs.read(reinterpret_cast<char*>(&size), 1);
	ifs.read(buff, size);

	version = buff;
	try
	{
		if (version == m_strCurrVersion)	// make sure we are loading the current version
		{
			// Read in basic map info:
			// Total number of tiles on map
			// number of columns
			// number of rows
			ifs.read(reinterpret_cast<char*>(&m_nTotalNumTiles), 4);
			ifs.read(reinterpret_cast<char*>(&m_nNumCols), 4);
			ifs.read(reinterpret_cast<char*>(&m_nNumRows), 4);

			//////////////////////////////////////////////////////////////////////////
			//	set up the map so that it centers at the beginning
			m_nMapWidth = g_TileWidth * m_nNumCols; m_nMapHeight = (g_TileHeight >> 1) * m_nNumRows;
			m_nMaxScrollX = 0;
			m_nMaxScrollY = 0;
			if (m_nMapWidth > m_nScreenWidth)
				m_nMaxScrollX = (m_nMapWidth) - (m_nScreenWidth) + (g_TileWidth>>1) + X_OS*2;
			if (m_nMapHeight > m_nScreenHeight-150)
				m_nMaxScrollY = (m_nMapHeight) - (m_nScreenHeight-250);

			// allocate memory for layer 1, 2, and 3(free placed tiles)
			m_pTilesL1  = new CTile[m_nNumRows*m_nNumCols];
			m_pTilesL2  = new CTile[m_nNumRows*m_nNumCols];
			m_pFreeTiles= new CFreeTile[m_nNumRows*m_nNumCols];
// 			SetOffsetX(m_nScrollX - (m_nTileWidth >> 1));
// 			SetOffsetY(m_nSCrollY - (m_nTileHeight >> 1));
// 			SetFTosX( - (m_nMapWidth >> 1));
// 			SetFTosY( - (m_nTileHeight >> 1));
		}
		else // didn't have the correct version number...
		{
			char szBuffer[128];
			sprintf_s(szBuffer, "Current version: %s ...does not match loaded version: %s", m_strCurrVersion.c_str(), version.c_str());
			MessageBox(0, szBuffer, "Incorrect version.", MB_OK);
			ifs.close();
//			m_pGame->ChangeState(CMainMenuState::GetInstance());
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close();
	}

	int tilesetCount = 0;	// how many tilesets are we going to be drawing from?
	int numCols = 0;
	int xID, yID, destID, sourceID, flag, width, height, cost; // tile variables
	float rotation;
	string trigger;	// tile trigger string
	byte red, green, blue; // for tileset key color
	int spawnTileCount = 0; cost = 0;

	try 
	{
		ZeroMemory(buff, 256);
		ifs.read(reinterpret_cast<char*>(&size), 1);
		ifs.read(buff, size);
		name = buff;
		//////////////////////////////////////////////////////////////////////////
		// Loading in tilesets
		// if there's a tileset..load the image with the correct key color
		while (name == "Tileset")
		{
			ZeroMemory(buff, size);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			fName = buff;
			strcpy_s(file, fName.c_str());

			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;

			ifs.read(reinterpret_cast<char*>(&red), 1);
			ifs.read(reinterpret_cast<char*>(&green), 1);
			ifs.read(reinterpret_cast<char*>(&blue), 1);
			// store the tileset information so we can determine where each tile comes from
			m_pTilesets[tilesetCount].id = Globals::g_pTM->LoadTexture(file, D3DCOLOR_ARGB(255,red, green, blue));
			m_pTilesets[tilesetCount++].name = name;

			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
		}
	}
	catch (ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close();
	}
	int imageID; int sPos;
	sPos = ifs.tellg();
	try
	{
		//////////////////////////////////////////////////////////////////////////
		// Layer ONE
		int count = 0;
		while (name == "Layer1")
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;
			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					numCols = Globals::g_pTM->GetTextureWidth(imageID) / g_TileWidth;
					break;
				}
			}
			ifs.read(reinterpret_cast<char*>(&sourceID), 4);
			ifs.read(reinterpret_cast<char*>(&xID), 4);
			ifs.read(reinterpret_cast<char*>(&yID), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;
			ifs.read(reinterpret_cast<char*>(&cost), 4);
			// TODO:: temp
			cost = 1;

			// setting up each tile of the first layer
			destID = yID * m_nNumCols + xID;	// the id of the tile in the 1-d array
			m_pTilesL1[destID] = CTile(sourceID, imageID, numCols, xID, yID, width, height, flag, trigger, cost);
			// 			if (flag > 3)	// tile is a spawn point
			// 				AddMapSpawnTile(&m_pTilesL1[destID], ++spawnTileCount);
			rotation = 0;
			// skip ahead to determine if we have more tiles
			// or if we move on to layer 2 ("L") or 3 ("F")
			sPos = ifs.tellg();
			ifs.seekg(sPos+1);
			eat = ifs.peek();
			count++;
			if (eat == "L" || eat == "F")
				break;
			else
				ifs.seekg(sPos);
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.clear();ifs.close(); return;
	}
	//	m_nSpawnCnt = spawnTileCount;
	try
	{
		//if (m_pTilesL1[0].DestXID() != -1)
		//{
		ifs.seekg(sPos);	// set the input stream position back to where it needs to be
		//////////////////////////////////////////////////////////////////////////
		// Layer TWO
		ZeroMemory(buff, 256);
		ifs.read(reinterpret_cast<char*>(&size), 1);
		ifs.read(buff, size);
		name = buff;
		//}
		while (name == "Layer2")
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;
			if (tilesetName == "FreePlace")
				break;
			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					numCols = Globals::g_pTM->GetTextureWidth(imageID) / g_TileWidth;
					break;
				}
			}
			ifs.read(reinterpret_cast<char*>(&sourceID), 4);
			ifs.read(reinterpret_cast<char*>(&xID), 4);
			ifs.read(reinterpret_cast<char*>(&yID), 4);
			ifs.read(reinterpret_cast<char*>(&flag), 4);
			ifs.read(reinterpret_cast<char*>(&width), 4);
			ifs.read(reinterpret_cast<char*>(&height), 4);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;
			ifs.read(reinterpret_cast<char*>(&cost), 4);

			// setting up each tile of the first layer
			destID = yID * m_nNumCols + xID;
			m_pTilesL2[destID] = CTile(sourceID, imageID, numCols, xID, yID, width, height, flag, trigger, cost);
			sPos = ifs.tellg();
			ifs.seekg(sPos+1);
			eat = ifs.peek();
			if (eat == "F")
				break;
			else
				ifs.seekg(sPos);
		}

	}
	catch (ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.clear();ifs.close(); return;	
	}

	try
	{
		if (m_pTilesL2[0].DestXID() != -1 && m_pTilesL1[0].DestXID() != -1 || eat[0] == 'F')
		{
			ifs.seekg(sPos);
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			name = buff;
		}
		//////////////////////////////////////////////////////////////////////////
		// the Free Placed layer 
		int srcPosX, srcPosY, destX, destY, count = 0;	// unique variables to load in for CFreeTiles
		// this is the last layer, so check for end of file
		while (!ifs.eof())
		{
			// read in tile's tileset name (which tileset it came from)
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			tilesetName = buff;

			// determine which image id this tile should have
			for (int tsIndex = 0; tsIndex < tilesetCount; ++tsIndex)
			{
				if (tilesetName == m_pTilesets[tsIndex].name)
				{
					imageID = m_pTilesets[tsIndex].id;
					break;
				}
			}

			ifs.read(reinterpret_cast<char*>(&srcPosX), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&srcPosY), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&flag), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&destX), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&destY), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&width), sizeof(int));
			ifs.read(reinterpret_cast<char*>(&height), sizeof(int));
			ZeroMemory(buff, 256);
			ifs.read(reinterpret_cast<char*>(&size), 1);
			ifs.read(buff, size);
			trigger = buff;
			int rot = 0;
			ifs.read(reinterpret_cast<char*>(&rot), sizeof(int));
			rotation = (float)rot / 10.0f;

			m_pFreeTiles[count++] = CFreeTile(srcPosX, srcPosY, imageID, destX, destY, width, height, flag, trigger, rotation);
			m_nFreeTileCount = count;
		}
	}
	catch(ios_base::failure &)
	{
		if (!ifs.eof())
			throw;
		else
			ifs.close(); return;
	}
}

void CMap::DrawDebug()
{
	if (!Globals::g_pBitMapFont)
		return;
	char buff[64] = {0};
	//////////////////////////////////////////////////////////////////////////
	// fps
	++frames;
	if (frames == 60)
	{
		frames = 0;
		double fps = 60.0f / m_pTimer->GetElapsed();
		sprintf_s(buff, "FPS: %.1f", fps);
		m_pTimer->ResetTimer(false);
	}
	if (buff[0] != 0)
		Globals::g_pBitMapFont->DrawString(buff, 5, 670);
	//////////////////////////////////////////////////////////////////////////

	sprintf_s(buff, "Placing: %i", m_nCurrPlaceType);
	Globals::g_pBitMapFont->DrawString(buff, 5, 650);
	sprintf_s(buff, "x=%i,y=%i", m_ptCurrMouseTile.x,m_ptCurrMouseTile.y);
	Globals::g_pBitMapFont->DrawString(buff, 5, 620);
	sprintf_s(buff, "mouse x=%i, y=%i", m_ptMouseScrn.x, m_ptMouseScrn.y);
	Globals::g_pBitMapFont->DrawString(buff, 5, 595);
}

void CMap::HandleMouseInput()
{
	if (BIT_TEST_ON(m_nMapFlags, MF_MOUSE_IN_QUICK_BAR))	// no map interaction 
		return;

	if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT) && m_rViewport.IsPointInRect(m_ptMouseScrn))
	{
#if _DEBUG
		DebugWnd->DebugPoint(m_ptCurrMouseTile, &string("Tile Coord"));
		lastCoordClicked = m_ptCurrMouseTile;	// TODO:: TEMP
#endif
	
		if (m_nCurrPlaceType == MSA_SELECT)
		{
			SelectObj(m_pCurrHoverObject);
		}
		// adding an object
		else if (m_nCurrPlaceType < MSA_ADDING && m_pPlaceObj)
		{
			m_pPlaceObj->SetCoord(m_ptCurrMouseTile);
			Globals::g_pObjManager->AddObject(Globals::GetCurrPlayer(), m_pPlaceObj, IsoTilePlot(m_ptCurrMouseTile));
		}
		// removing an object
		else if (m_nCurrPlaceType == MSA_REMOVE)
		{
			if (!m_pCurrHoverObject)
				return;
			Globals::g_pObjManager->RemoveObj(Globals::GetCurrPlayer(), m_pCurrHoverObject);	// removing the currently hovered obj
		}
	}
	// attacking target (if there is a target), selecting if no target
	else if (Globals::g_pDI->MouseButtonPressed(MOUSE_RIGHT) && m_rViewport.IsPointInRect(m_ptMouseScrn))
	{
		// moving with mouse
		if (m_pCurrPlayerSelectedObj && m_pCurrPlayerSelectedObj->GetType() == OBJ_UNIT) // a unit has already been selected (of the current player)
		{
			if (m_ptCurrMouseTile != m_pCurrPlayerSelectedObj->GetCoord() || m_pCurrHoverObject)	// make sure nothing happens if right-clicking the object that's selected
			{
				if (m_pCurrHoverObject)	// some object is being right-clicked now
				{
						// right-clicking on current player's object, show extra info
					if (m_pCurrHoverObject->GetFactionID() == Globals::g_pCurrPlayer->GetProfile()->nFactionID)
					{	// TODO:: display stats or something extra (equipment panel maybe)
	
					} 
					else// an enemy object is being right-clicked
					{
						if (m_pEnemyObj)	
						{
							if (m_pCurrHoverObject == m_pEnemyObj) // right-clicking to attack an already-selected enemy object
							{
								DetermineMoveSpecifics(m_ptCurrMouseTile);
							} 
							else
							{
								Select(m_pCurrHoverObject);
							}
						} 
						else	// right-clicking enemy object
						{
							Select(m_pCurrHoverObject);
						}
					}
				} 
				else	// then we're just moving to open terrain
				{
					DetermineMoveSpecifics(m_ptCurrMouseTile);
				}
			}
		}
	}
}

void CMap::CenterMap( double fElapsedTime )
{
	pointf sPos = m_pCurrPlayerSelectedObj->GetSPos();
	point size = m_pCurrPlayerSelectedObj->GetSize();
	bool bDone = false;
	if (BIT_TEST_ON(m_nMapFlags, MF_CENTERING))
	{
		if (BIT_TEST_ON(m_nMapFlags, MF_CENTER_DIR_S))
		{
			if ((int)(sPos.y+(size.y>>1)) > (m_nScreenHeight>>1) && (int)m_fScrollY < m_nMaxScrollY)
				ScrollMapDown(fElapsedTime); 
			else
				BIT_OFF(m_nMapFlags, MF_CENTER_DIR_S);
		}
		else if (BIT_TEST_ON(m_nMapFlags, MF_CENTER_DIR_N))
		{
			if ((int)(sPos.y+(size.y>>1)) < (m_nScreenHeight>>1) && (int)m_fScrollY > 0)
				ScrollMapUp(fElapsedTime);  
			else
				BIT_OFF(m_nMapFlags, MF_CENTER_DIR_N);
		}
		if (BIT_TEST_ON(m_nMapFlags, MF_CENTER_DIR_W))
		{
			if ((int)(sPos.x+(size.x>>1)) < (m_nScreenWidth>>1)+1 && (int)m_fScrollX > 0)
				ScrollMapLeft(fElapsedTime); 
			else
				BIT_OFF(m_nMapFlags, MF_CENTER_DIR_W);
		}
		else if (BIT_TEST_ON(m_nMapFlags, MF_CENTER_DIR_E))
		{
			if ((int)(sPos.x+(size.x>>1)) > (m_nScreenWidth>>1)+1 && (int)m_fScrollX < m_nMaxScrollX)
				ScrollMapRight(fElapsedTime);  
			else
				BIT_OFF(m_nMapFlags, MF_CENTER_DIR_E);
		}
		bDone = true;
		for (int i = MF_CENTER_DIR_N; i < MF_CENTER_DIR_END; ++i)
			if (BIT_TEST_ON(m_nMapFlags, i))
				bDone = false;
		if (bDone)
		{BIT_OFF(m_nMapFlags, MF_CENTER_MAP); BIT_OFF(m_nMapFlags, MF_CENTERING); }
	}
	else
	{
		if ((int)(sPos.x+(size.x>>1)) > (m_nScreenWidth>>1) && (int)m_fScrollX < m_nMaxScrollX)
		{BIT_ON(m_nMapFlags, MF_CENTER_DIR_E);}
		else if ((int)(sPos.x+(size.x>>1)) < (m_nScreenWidth>>1) && (int)m_fScrollX > 0)
		{BIT_ON(m_nMapFlags, MF_CENTER_DIR_W);}
		if ((int)(sPos.y+(size.y>>1)) > (m_nScreenHeight>>1) && (int)m_fScrollY < m_nMaxScrollY)
		{BIT_ON(m_nMapFlags, MF_CENTER_DIR_S);}
		else if ((int)(sPos.y+(size.y>>1)) < (m_nScreenHeight>>1) && (int)m_fScrollY > 0)
		{BIT_ON(m_nMapFlags, MF_CENTER_DIR_N);}
		BIT_ON(m_nMapFlags, MF_CENTERING);
	}
}

void CMap::HandleViewScroll( POINT &mouse, double fElapsedTime )
{
	if (Globals::g_pDI->KeyDown(DIK_DOWNARROW) || mouse.y >= m_nScreenHeight/*m_rViewport.bottom*/ - SCROLL_AREA)
	{
		if ( BIT_TEST_ON(m_nMapFlags, MF_DO_SCROLL_S) || Globals::g_pDI->KeyDown(DIK_DOWNARROW))
			ScrollMapDown(fElapsedTime);
		else
			BIT_ON(m_nMapFlags, MF_IN_SCROLL_AREA_S);
	}
	else {BIT_OFF(m_nMapFlags, MF_DO_SCROLL_S); BIT_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_S);}
	//////////////////////////////////////////////////////////////////////////
	if (Globals::g_pDI->KeyDown(DIK_UPARROW) || mouse.y <= /*m_rViewport.top +*/ SCROLL_AREA)
	{
		if (BIT_TEST_ON(m_nMapFlags, MF_DO_SCROLL_N) || Globals::g_pDI->KeyDown(DIK_UPARROW))
			ScrollMapUp(fElapsedTime);
		else
			BIT_ON(m_nMapFlags, MF_IN_SCROLL_AREA_N);
	}
	else {BIT_OFF(m_nMapFlags, MF_DO_SCROLL_N); BIT_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_N);}
	//////////////////////////////////////////////////////////////////////////
	if (Globals::g_pDI->KeyDown(DIK_RIGHTARROW) || mouse.x >= m_nScreenWidth/*m_rViewport.right*/ - SCROLL_AREA)
	{
		if (BIT_TEST_ON(m_nMapFlags, MF_DO_SCROLL_E) || Globals::g_pDI->KeyDown(DIK_RIGHTARROW))
			ScrollMapRight(fElapsedTime);
		else
			BIT_ON(m_nMapFlags, MF_IN_SCROLL_AREA_E);
	}
	else {BIT_OFF(m_nMapFlags, MF_DO_SCROLL_E); BIT_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_E);}
	//////////////////////////////////////////////////////////////////////////
	if (Globals::g_pDI->KeyDown(DIK_LEFTARROW) || mouse.x <= /*m_rViewport.left +*/ SCROLL_AREA)
	{
		if (BIT_TEST_ON(m_nMapFlags, MF_DO_SCROLL_W) || Globals::g_pDI->KeyDown(DIK_LEFTARROW))
			ScrollMapLeft(fElapsedTime);
		else
			BIT_ON(m_nMapFlags, MF_IN_SCROLL_AREA_W);
	}
	else {BIT_OFF(m_nMapFlags, MF_DO_SCROLL_W); BIT_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_W);}
	//////////////////////////////////////////////////////////////////////////

	// NOT in any scroll area, reset & stop the timer
	if (m_pTimer->IsTimerRunning() && 
		BIT_TEST_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_N) &&
		BIT_TEST_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_S) &&
		BIT_TEST_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_E) &&
		BIT_TEST_OFF(m_nMapFlags, MF_IN_SCROLL_AREA_W)
		)
			m_pTimer->ResetTimer();
	else if (!m_pTimer->IsTimerRunning() && 
		(BIT_TEST_ON(m_nMapFlags, MF_IN_SCROLL_AREA_N) ||
		 BIT_TEST_ON(m_nMapFlags, MF_IN_SCROLL_AREA_S) ||
		 BIT_TEST_ON(m_nMapFlags, MF_IN_SCROLL_AREA_E) ||
		 BIT_TEST_ON(m_nMapFlags, MF_IN_SCROLL_AREA_W))
		
		)
			m_pTimer->StartTimer(SCROLL_DELAY);
}

bool CMap::HandleKBInput()
{
	// TODO:: change this to real functionality....
	if (Globals::g_pDI->KeyPressed(DIK_1))
	{
		m_nCurrPlaceType = MSA_CITY;
		if (m_pPlaceObj)
			delete m_pPlaceObj;
		m_pPlaceObj = new CCity(OBJ_CITY, CITY_CELTIC, 1, m_ptCurrMouseTile, IsoTilePlot(m_ptCurrMouseTile), "Sycharth",
							Globals::GetCurrPlayer()->GetProfile()->name.c_str(), Globals::GetCurrPlayer()->GetProfile()->nFactionID);
	}
	else if (Globals::g_pDI->KeyPressed(DIK_2))
	{
		m_nCurrPlaceType = MSA_UNIT;
		if (m_pPlaceObj)
			delete m_pPlaceObj;
		m_pPlaceObj = new CUnit(UnitDefines::UNIT_UMKNIGHT, OBJ_UNIT, m_ptCurrMouseTile, IsoTilePlot(m_ptCurrMouseTile), "UMKnight",
							Globals::GetCurrPlayer()->GetProfile()->name.c_str(), Globals::GetCurrPlayer()->GetProfile()->nFactionID);
	}
	else if (Globals::g_pDI->KeyPressed(DIK_3))
	{
		m_nCurrPlaceType = MSA_BUILDING;
		if (m_pPlaceObj)
			delete m_pPlaceObj;
		m_pPlaceObj = new CBuilding(BLDG_CASTLE, OBJ_BUILDING, m_ptCurrMouseTile, IsoTilePlot(m_ptCurrMouseTile), "Castle",
							Globals::GetCurrPlayer()->GetProfile()->name.c_str(), Globals::GetCurrPlayer()->GetProfile()->nFactionID);
	}
	else if (Globals::g_pDI->KeyPressed(DIK_5))
	{
		m_nCurrFactionPlace = 0;
		m_nCurrPlaceType = MSA_SELECT;
	}
	// bring up combat skills (if applicable)
 	else if (Globals::g_pDI->KeyPressed(DIK_C) && m_pCurrPlayerSelectedObj)
		Globals::g_pHUD->AddWindow(Globals::g_pHUD->GetButton(BL_SLOT_1_1)->SimulatePressed());
	// skip current unit
	else if (Globals::g_pDI->KeyPressed(DIK_SPACE) && m_pCurrPlayerSelectedObj)
		Globals::g_pHUD->GetButton(BL_SLOT_2_1)->SimulatePressed();
	// come back to this unit later (wait)
	else if (Globals::g_pDI->KeyPressed(DIK_W) && m_pCurrPlayerSelectedObj)
		Globals::g_pHUD->GetButton(BL_SLOT_2_3)->SimulatePressed();
	// center the map on the currently selected unit
	else if (m_pCurrPlayerSelectedObj && Globals::g_pDI->KeyPressed(DIK_E))
		BIT_ON(m_nMapFlags, MF_CENTER_MAP);	

	else if (Globals::g_pDI->KeyPressed(DIK_ESCAPE))
	{
		if (!m_pEnemyObj)	// bring up menu if no target (does not include friendly targets)
			Globals::g_pHUD->AddWindow(Globals::g_pHUD->GetButton(BL_MENU)->SimulatePressed());
		else
			Deselect(m_pEnemyObj);
	}
	// end turn
	else if (Globals::g_pDI->KeyPressed(DIK_Q))
		Globals::g_pHUD->GetButton(BL_END_TURN)->SimulatePressed();

	return true;
}

void CMap::Deselect(CObject*& obj)
{
	obj->SetColor(WHITE);
	obj->SetHovered(false);
	obj = NULL;
}
void CMap::Select(CObject* const obj)
{
	// selecting yourself
	if (obj->GetFactionID() == Globals::GetCurrPlayer()->GetProfile()->nFactionID)
	{
		if (m_pCurrPlayerSelectedObj)
			Deselect(m_pCurrPlayerSelectedObj);
		m_pCurrPlayerSelectedObj = obj;
		m_pCurrPlayerSelectedObj->SetHovered(true);
		m_pCurrPlayerSelectedObj->SetColor(DARKBLUE);

		// if there's an enemy already selected, and curr player's is an OBJ_UNIT, determine if we need to face it
		if (m_pEnemyObj && m_pCurrPlayerSelectedObj->GetType() == OBJ_UNIT)
		{
			((CUnit*)m_pCurrPlayerSelectedObj)->FindPathToTarget(m_pEnemyObj->GetCoord(), m_vPath);
			// NOTE: face the obj only if the m_pCurrPlayerSelectedObj can "see" it
			// for now, that means one tile away, don't bother changing facing if they're on the same tile
			if (m_vPath.size() == 1 && m_pEnemyObj->GetCoord() != m_pCurrPlayerSelectedObj->GetCoord())	
				((CUnit*)m_pCurrPlayerSelectedObj)->FaceTarget(m_pEnemyObj->GetCoord());
		}
		Globals::g_pHUD->SetInitialQBSlots(obj, obj->GetQBSlots());
	} 
	// selecting an enemy
	else
	{
		// if the object being selected is already selected, and the curr player has a UNIT selected, attack it
		if (m_pEnemyObj == obj && dynamic_cast<CUnit *>(m_pCurrPlayerSelectedObj) )
		{
			DetermineMoveSpecifics(m_pEnemyObj->GetCoord());
		}
		else if (m_pEnemyObj)	// there was another enemy object, deselect it first
			Deselect(m_pEnemyObj);
		m_pEnemyObj = obj;
		m_pEnemyObj->SetHovered(true);
		m_pEnemyObj->SetColor(DARKRED);

		// if we have a unit selected, determine if we need to face the newly selected enemy
		if (m_pCurrPlayerSelectedObj && m_pCurrPlayerSelectedObj->GetType() == OBJ_UNIT)
		{
			((CUnit*)m_pCurrPlayerSelectedObj)->FindPathToTarget(m_pEnemyObj->GetCoord(), m_vPath);
			// NOTE: face the obj only if the m_pCurrPlayerSelectedObj can "see" it
			// for now, that means one tile away
			if (m_vPath.size() == 1 && m_pEnemyObj->GetCoord() != m_pCurrPlayerSelectedObj->GetCoord())	
				((CUnit*)m_pCurrPlayerSelectedObj)->FaceTarget(m_pEnemyObj->GetCoord());
		}
	}

	//BIT_ON(m_nMapFlags, MF_CENTER_MAP);
}
void CMap::InitiateAttack(bool setfacing /*= true*/)
{
	CUnit* playerUnit = ((CUnit*)m_pCurrPlayerSelectedObj);
	if (setfacing)
		playerUnit->FaceTarget(m_pEnemyObj->GetCoord());

// TODO:: SETUP ATTACK WHERE ATTACKER GOES TO ENEMY TILE PART-WAY, DOES COMBAT, AND STAYS (unless a special ability allows it either to leave)
	// also need to decrement stamina for the terrain cost

	playerUnit->DecrementStamina(playerUnit->GetCurrAttackAbility()->GetCombatProps().AttackStam);
	BIT_ON(m_nMapFlags, MF_ATTACKING);

	// TODO:: units will not turn to face if a surprise attack is happening:
	((CUnit*)m_pEnemyObj)->SetFacing(playerUnit->GetOppositeFacing());
	
	// animate attack..delay damage shown to user until attack is finished
	if (playerUnit->GetCurrAnimString() != "Attack")
		playerUnit->ChangeAnim("Attack");
	playerUnit->GetCurrAnim().Play();
	m_BattleMngr.Init(playerUnit, m_pEnemyObj, m_pVictor);
}

void CMap::ActionIfSelected(CObject* obj)
{
	if (obj == m_pCurrPlayerSelectedObj)	// if the attacking unit is killed
	{
		Deselect(m_pCurrPlayerSelectedObj);
		if (m_pEnemyObj)
			Deselect(m_pEnemyObj);
	}
	else if (obj == m_pEnemyObj)		// if the target unit is killed
		Deselect(m_pEnemyObj);
}
void CMap::FindNewCoord( int facing, int &newY, int &newX )
{
	switch (facing)
	{
	case DIR_SW:
		{
			if ((newY & 1) == EVEN)
				--newX;
			++newY;
		}break;
	case DIR_S:
		{
			newY += 2; 
		}break;
	case DIR_SE:
		{
			if ((newY & 1) == ODD)
				++newX;
			++newY;
		}break;
	case DIR_W:
		{
			--newX;
		}break;
	case DIR_E:
		{
			++newX;
		}break;
	case DIR_NW:
		{
			if ((newY & 1) == EVEN)
				--newX;
			--newY;
		}break;
	case DIR_N:
		{
			newY -= 2;
		}break;
	case DIR_NE:
		{
			if ((newY & 1) == ODD)
				++newX;
			--newY;
		}break;
	}
}


void CMap::SelectObj(CObject*& obj, bool deselectAll /*= false*/)
{
	// this function has to handle selection/deselection for human input AND computer

	// if no object is passed in, we need to deselect, starting with any enemy object selected, then the current player's selected
	if (!obj)
	{
		if (m_pEnemyObj)
		{
			Deselect(m_pEnemyObj);
			if (m_pCurrPlayerSelectedObj && deselectAll)
			{
				Deselect(m_pCurrPlayerSelectedObj);
				Globals::g_pHUD->ClearQBSlots();
			}
		}
		else if (m_pCurrPlayerSelectedObj)
		{
			Deselect(m_pCurrPlayerSelectedObj);
			Globals::g_pHUD->ClearQBSlots();
		}
		return;
	}

	// If we reach this point, an object is selected, if it's an enemy, turn it red, if it's yours, turn it blue
	Select(obj);
}

void CMap::DoMove( int newX, int newY, eAnimationDirections facing )
{
	if (point(newX, newY) != m_pCurrPlayerSelectedObj->GetCoord())
	{
		CUnit* currPlayerUnit = ((CUnit*)m_pCurrPlayerSelectedObj);
		BIT_ON(m_nMapFlags, MF_MOVING);
		if (currPlayerUnit->GetMoveToPos().x < 150 || currPlayerUnit->GetMoveToPos().x > m_nScreenWidth - 150
			|| currPlayerUnit->GetMoveToPos().y < 150 || currPlayerUnit->GetMoveToPos().y > m_rViewport.bottom - 150)
			BIT_ON(m_nMapFlags, MF_CENTER_MAP);
	}
}

int  CMap::DetermineTotalMoveCost(bool attacking)
{
	PathIter iter = m_vPath.begin();
	PathIter end = m_vPath.end();
	int cost = 0;
	for (; iter != end; ++iter)
	{
		// TODO:: put all layers of tiles into each tile
		cost += (*iter)->TerrainCost();
	}
	if (attacking)
		cost += m_pCurrPlayerSelectedObj->GetCurrAttackAbility()->GetCombatProps().AttackStam;
	return cost;
}
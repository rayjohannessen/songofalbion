#include "stdafx.h"

#include "Pathfinding.h"
#include "AnimationDefinesEnums.h"
#include "Globals.h"
#include "Object.h"

namespace Pathfinding
{
	// CPFEntity (PathFindingEntity)
	CPFEntity::CPFEntity(const CTile* tile, CPFEntity* parent /* = NULL */) :
		m_pParent(parent),
		m_pTile(tile),
		m_nGCost(0),
		m_nHCost(0),
		m_nFCost(0)
	{

	}
	CPFEntity::CPFEntity(const CTile* tile, CPFEntity* parent, int gCost, int hCost) :
		m_pParent(parent),
		m_pTile(tile),
		m_nGCost(gCost),
		m_nHCost(hCost),
		m_nFCost(gCost + hCost)
	{

	}
	// End CPFEntity
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// A-Star search algorithm
	void	A_Star(const CTile& startTile, const CTile& targetTile, Path& path, /*const CObject* const object,*/ 
				   const CTile* const tilesL1, const CTile* const tilesL2, point& numColsRows)
	{
		// local variables needed for the algorithm
		EntityMap created;
		EntityDeque open;
		CPFEntity* pCurrEntity;
		const CTile* pTile;
		point adjTileCoord;
		pointf targetTilePos = IsoTilePlot(targetTile.DestID());

		// put the start tile in the open
		open.push_back(new CPFEntity(&startTile));
		created[&startTile] = open[0];

		while (open.size())
		{
			pCurrEntity = open.front(); 
			open.pop_front();

			if (pCurrEntity->Tile() == &targetTile)
			{	// found the tile, we're done searching
				// we start with target tile, push_front so that the path can be iterated start tile -> target tile
				path.push_front(pCurrEntity->Tile());
				while (pCurrEntity->Parent() && pCurrEntity->Parent()->Tile() != &startTile)
				{
					// TODO:: determine the farthest tile this object can actually reach
					// and only put those tiles in the path
					pCurrEntity = pCurrEntity->Parent();
					path.push_front(pCurrEntity->Tile());
				}
				// clean up memory
				EntityMapIter iter = created.begin(), end = created.end();
				for (; iter != end; ++iter)
					delete iter->second;
				return;
			}

			// find all the surrounding tiles, add them to the open if they're not already on there
			for (unsigned i = 0; i < 8; ++i)
			{									
				adjTileCoord = pCurrEntity->Tile()->DestID();
													// determine if the tile's y is odd/even, lookup the direction, returning the offset point
				adjTileCoord += Globals::g_AdjTileOffsets[pCurrEntity->Tile()->DestYID() & 1][(eDirections)i];

				// make sure this tile is on the map
				// TODO:: determine if the tile has an enemy on it
				// TODO:: Determine last tile unit can go to
				if (adjTileCoord.x > -1 && adjTileCoord.x < numColsRows.x && adjTileCoord.y > -1 && adjTileCoord.y < numColsRows.y)
				{					// standard x,y col/row to 1D array index formula
					pTile = &tilesL1[adjTileCoord.y * numColsRows.x + adjTileCoord.x];
					pointf thisTilePos = IsoTilePlot(pTile->DestID());
					int given = ComputeGivenCost(pTile->TerrainCost(), thisTilePos, pCurrEntity);

					// also make sure it hasn't already been created
					if (created.find(pTile) == created.end())
					{
						CPFEntity* entity = new CPFEntity(pTile, pCurrEntity, given, (int)SqrDist(thisTilePos, targetTilePos));
						open.push_back(entity);
						created[pTile] = entity;
					}
					else	// if it is, see if this is a better route
					{
						if (created[pTile]->G() > given)
						{
							created[pTile]->G(given);
							created[pTile]->F(given + created[pTile]->H());
							created[pTile]->Parent(pCurrEntity);
							for(unsigned i = 0; i < open.size(); ++i)
								if (open[i] == created[pTile])
									open[i] = created[pTile];
						}
					}
				}
			}
		}
	}
	// End A-Star
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Utility functions used in pathfinding
	int	ComputeGivenCost(int terrainCost, const pointf& tilePos, const CPFEntity* const parentEntity)
	{
		// thisEntity's terrain cost * distance to parent + parent's gCost
		return int(terrainCost * SqrDist(tilePos, IsoTilePlot(parentEntity->Tile()->DestID())) + parentEntity->G()); 
	}

	float SqrDist(const pointf& start, const pointf& end)
	{
		return sqrt( (start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y) );
	}

	point IsoTilePlot(point& pt)
	{
		return point( pt.x * g_TileWidth + (pt.y & 1) * (g_TileWidth >> 1), pt.y * (g_TileHeight >> 1));
	}
	// End Utility functions
	//////////////////////////////////////////////////////////////////////////
}
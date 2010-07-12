#pragma once

#include "Tile.h"
#include "Unit.h"

//////////////////////////////////////////////////////////////////////////
// For Song of Albion, with isometric tiles (128x64), a distance of 64 pixels
//	will be equivalent to costing 1 stamina for a unit to move. This makes
//	east/west movement more expensive since it is actually farther than all
//	other directions. This is done for ease of distance checks, since isometric
//	maps don't lend themselves to an easy way of this, also ensuring consistency
//	in movement cost and therefore pathfinding estimates.
//////////////////////////////////////////////////////////////////////////
namespace Pathfinding
{
	class CPFEntity 
	{
		CPFEntity*	m_pParent;
		const CTile*m_pTile;
		int			m_nGCost;	// given
		int			m_nHCost;	// heuristic
		int			m_nFCost;	// final

	public:
		explicit CPFEntity(const CTile* tile, CPFEntity* parent = NULL);
		explicit CPFEntity(const CTile* tile, CPFEntity* parent, int gCost, int hCost);

		//////////////////////////////////////////////////////////////////////////
		// ACCESSORS
		inline int	G()	const	{ return m_nGCost;	}
		inline int	H()	const	{ return m_nHCost;	}
		inline int	F()	const	{ return m_nFCost;	}
		inline const CTile* Tile() const		{ return m_pTile;	}
		inline CPFEntity* const Parent() const	{ return m_pParent;	}

		//////////////////////////////////////////////////////////////////////////
		// MUTATORS
		inline void G(int gCost)		{ m_nGCost = gCost;	}
		inline void H(int hCost)		{ m_nHCost = hCost;	}
		inline void F(int fCost)		{ m_nFCost = fCost;	}
		inline void Parent(CPFEntity* const entity)	{ m_pParent = entity; }
	};
	typedef map<const CTile*, CPFEntity*> EntityMap;
	typedef map<const CTile*, CPFEntity*>::const_iterator EntityMapIter;
	typedef deque<CPFEntity*> EntityDeque;
	typedef deque<CPFEntity*>::const_iterator EntityDequeIter;
	typedef deque<const CTile*> Path;
	//////////////////////////////////////////////////////////////////////////
	//	FUNCTION: A_Star
	//	startTile - the tile to start pathfinding from
	//	targetTile- where the pathfinding ends
	//	path	  - the end path result, will be empty if no path found, ordered start->finish
	//	unit	  - needed to determine valid moves from any special terrain cost modifiers and amount of stamina
	//	tilesL1   - array of layer 1's tiles
	//	tilesL2	  - array of layer 2's tiels
	//
	//	RETURN void
	//////////////////////////////////////////////////////////////////////////
	void	A_Star(const CTile& startTile, const CTile& targetTile, Path& path, const CUnit* const unit, const CTile* const tilesL1, const CTile* const tilesL2, point& numColsRows);



	//////////////////////////////////////////////////////////////////////////
	// Utility functions
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	TileDistance
	//
	// RETURN (float): squared distance between two points
	//////////////////////////////////////////////////////////////////////////
	float	SqrDist(const pointf& start, const pointf& end);
	int		ComputeGivenCost(int terrainCost, const pointf& tilePos, const CPFEntity* const parentEntity);
	inline point IsoTilePlot(point& pt);
}

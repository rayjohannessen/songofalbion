//////////////////////////////////////////////////////////////////////////
//	Filename	:	CTile.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To define the CTile and CFreeTile classes...which will be used in drawing
//					the individual tiles of the maps
//////////////////////////////////////////////////////////////////////////
#ifndef TILE_H
#define TILE_H

#include "Structs.h"
#include <string>
using std::string;

typedef DWORD D3DCOLOR;
#define D3DCOLOR_ARGB(a,r,g,b) \
	((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
// for all regular tiles that are put into the grid
class CTile
{
private:
	int			m_nSourceID;		// where from the source is this tile?
	//int			m_nSourceTS;
	int 		m_nImageID;			// which image does this tile come from?
	point		m_ptDestID;			// the ID of the tile on the battle map

	int 		m_nHeight;			// tile's width
	int 		m_nWidth;			// tile's height
	int 		m_nFlag;			// collision/other flags

	int			m_nTerrainCost;

	//////////////////////////////////////////////////////////////////////////

	int			m_nAlpha;
	DWORD		m_dwColor;
	string		m_strTrigger;		// trigger string, used to trigger a specific event from this tile
	rect		m_rSrc;				// the source rect of the tile

public:
	CTile(void);
	~CTile(void);
	CTile(int nSourceID, int nImageID, int numCols, int nDestX, int nDestY, int nWidth, int nHeight, int nFlag, string strTrigger, int);

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	rect* SourceRect()			{ return &m_rSrc;		}
	int  SourceID()		const 	{ return m_nSourceID;	}
	int  ImageID()		const 	{ return m_nImageID;	}
	int  DestXID()		const 	{ return m_ptDestID.x;	}
	int  DestYID()		const 	{ return m_ptDestID.y;	}
	point DestID()		const	{ return m_ptDestID;	}
	int  Height()		const 	{ return m_nHeight;		}
	int  Width()		const 	{ return m_nWidth;		}
	int  Flag()			const 	{ return m_nFlag;		}
	int  Alpha()		const 	{ return m_nAlpha;		}
	int  TerrainCost()	const 	{ return m_nTerrainCost;}
	string Trigger()	const 	{ return m_strTrigger;	}
	DWORD  Color()		const 	{ return D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255);}

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
// 	void SourceID(int val) { m_nSourceID = val; }
// 	void ImageID(int val) { m_nImageID = val; }
// 	void DestinationID(int val) { m_nDestinationID = val; }
// 	void Height(int val) { m_nHeight = val; }
// 	void Width(int val) { m_nWidth = val; }
// 	void Flag(int val) { m_nFlag = val; }
// 	void Trigger(std::string val) { m_strTrigger = val; }
	void SetAlpha(int alpha)		{m_nAlpha = alpha;		}
	void SetTerrainCost(int cost)	{m_nTerrainCost = cost;	}
};

// for all freely placed tiles that are NOT put into the grid...mostly the tiles that make it look pretty
class CFreeTile
{
private:
	int m_nSourceX;			// where from the source is this tile?
	int m_nSourceY;

	int m_nImageID;			// which image does this tile come from?
	//int m_nSourceTS;
	int m_nDestX;			// where is the tile placed on the screen?
	int m_nDestY;
	int m_nHeight;			// tile's width
	int m_nWidth;			// tile's height
	float m_fRotation;
	int m_nFlag;			// collision/other flags
	int m_nAlpha;			// tile's alpha
	std::string m_strTrigger;	// trigger string, used to trigger a specific event from this tile
	rect m_rSrc;

public:
	CFreeTile();
	~CFreeTile();
	CFreeTile(int nSourceX, int nSourceY, int nImageID, int nDestX, int nDestY, int nWidth, int nHeight, int nFlag, string strTrigger, float rotation = 0.0f);

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	rect* SourceRect()	  { return &m_rSrc;		}
	int ImageID()		const { return m_nImageID;	}
	//int SourceTS()		const { return m_nSourceTS; }
	int SourceX()		const { return m_nSourceX;	}
	int SourceY()		const { return m_nSourceY;	}
	int DestX()			const { return m_nDestX;	}
	int DestY()			const { return m_nDestY;	}
	int Height()		const { return m_nHeight;	}
	int Width()			const { return m_nWidth;	}
	int Flag()			const { return m_nFlag;		}
	int Alpha()			const { return m_nAlpha;	}
	float Rotation()	const { return m_fRotation;}
	string Trigger() const { return m_strTrigger; }

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
// 	void ImageID(int val) { m_nImageID = val; }
// 	void SourceX(int val) { m_nSourceX = val; }
// 	void SourceY(int val) { m_nSourceY = val; }
// 	void DestX(int val) { m_nDestX = val; }
// 	void DestY(int val) { m_nDestY = val; }
// 	void Height(int val) { m_nHeight = val; }
// 	void Width(int val) { m_nWidth = val; }
// 	void Flag(int val) { m_nFlag = val; }
// 	void Trigger(std::string val) { m_strTrigger = val; }
	void SetAlpha(int alpha) {m_nAlpha = alpha;}
};

const static int g_TileWidth = 128;
const static int g_TileHeight= 64;

#endif
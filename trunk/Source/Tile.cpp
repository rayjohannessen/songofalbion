#include "stdafx.h"

#include "Tile.h"

//////////////////////////////////////////////////////////////////////////
// CTile
CTile::CTile() :
m_nSourceID(0),
m_nImageID(0),
m_ptDestID(-1, 0),
m_nHeight(0),
m_nWidth(0),
m_nFlag(0),
m_nTerrainCost(1),
m_nAlpha(255),
m_strTrigger(""),
m_rSrc()
{
}

CTile::CTile(int nSourceID, int nImageID, int numCols, int nDestX, int nDestY, int nWidth, int nHeight, int nFlag, string strTrigger, int tCost) :
m_nSourceID(nSourceID),
m_nImageID(nImageID),
m_ptDestID(nDestX, nDestY),
m_nHeight(nHeight),
m_nWidth(nWidth),
m_nFlag(nFlag),
m_nTerrainCost(tCost),
m_nAlpha(255),
m_strTrigger(strTrigger)
{
	// setup source rect here
	int left, top; 
	left = (nSourceID % numCols) * nWidth;
	top = (nSourceID / numCols) * nHeight;
	m_rSrc = rect(top, top + nHeight, left, left + nWidth);
}

CTile::~CTile()
{
}

//////////////////////////////////////////////////////////////////////////
// CFreeTile
CFreeTile::CFreeTile()
{

}

CFreeTile::CFreeTile(int nSourceX, int nSourceY, int nImageID, int nDestX, int nDestY, int nWidth, int nHeight, int nFlag, string strTrigger, float rotation /* = 0.0f */) :
m_rSrc(nSourceY, nSourceY + nHeight, nSourceX, nSourceX + nWidth),
m_nImageID(nImageID),
m_nSourceX(nSourceX),
m_nSourceY(nSourceY),
m_nDestX(nDestX),
m_nDestY(nDestY),
m_nWidth(nWidth),
m_nHeight(nHeight),
m_nFlag(nFlag),
m_strTrigger(strTrigger),
m_fRotation(rotation),
m_nAlpha(255)
{
}

CFreeTile::~CFreeTile()
{
}
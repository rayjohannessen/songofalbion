////////////////////////////////////////////////////////
//   File Name	:	"BitmapFont.h"
//
//   Author		:	Ray Johannessen
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////

#ifndef BITMAPFONT_H
#define BITMAPFONT_H

#include "Structs.h"

#define DEFAULT_SCALE 0.65f
struct BFProfile 
{
	char	StartChar;		// ascii start character value
	bool	HasLowerCase;	// does the bitmap font support lower case characters
	int		ID;				// image id
	int		NumCols;
	int		Size;			// height/width of each cell
	float	Scale;			// to offset the scale to begin with (different from the scale passed into the draw function)

	BFProfile(char _startChar, bool _lcase, int _id, int _numCols, int _size, float _scale = DEFAULT_SCALE) : 
		StartChar(_startChar), 
		HasLowerCase(_lcase), 
		ID(_id),
		NumCols(_numCols),
		Size(_size),
		Scale(_scale)
	{}
};

enum eBMProf { BFP_DEFAULT, BFP_MEDIEVAL, NUM_BFPROFILES};


class CBitmapFont
{
	BFProfile* m_arrProfiles[NUM_BFPROFILES];
	BFProfile* m_pCurrBMProf;

	inline rect CellAlgorithm(int ID);

	CBitmapFont(void);
	CBitmapFont(const CBitmapFont&);
	CBitmapFont& operator=(const CBitmapFont&);
	~CBitmapFont(void);

public:
	///////////////////////////////////////////////////////////////////
	// Function: "GetInstance"
	//
	//  Purpose: Gets instance to the only instance of the class.
	///////////////////////////////////////////////////////////////////
	static CBitmapFont* GetInstance(void);

	///////////////////////////////////////////////////////////////////
	// Function: "Load"
	//
	//  Purpose: Loads the sprite sheet and sets its members from passed in data
	///////////////////////////////////////////////////////////////////
	void LoadProfiles();

	///////////////////////////////////////////////////////////////////
	// Function: “drawstring”
	//
	//	NOTE: does not support newline
	//
	// Purpose: Draws the string passed in to the screen at the specified location.
	////////////////////////////////////////////////////////////////////
	void DrawString( const char* szstring, int posX, int posY, float posZ = 0.05f, float scale = 1.0f, DWORD color = -1); 

	//////////////////////////////////////////////////////////////////////////
	// Function		:		DrawStringAutoCenter
	//
	//	NOTE: specify w/h to zero if no centering is needed for that orientation.
	//			does not support newline and does not wrap
	//
	// Purpose		:		Auto centers within the confies of the given rect
	//
	// RETURN(point):		The start point of the string after being centered
	//////////////////////////////////////////////////////////////////////////
	point DrawStringAutoCenter (const char* szString, const rect& r, float zPos = 0.09f, float fScale = 1.0f, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
	inline void SetBMProfile(eBMProf prof)	{ m_pCurrBMProf = m_arrProfiles[prof];	}		

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	inline const int GetSize() const	{ return int(float(m_pCurrBMProf->Size) * m_pCurrBMProf->Scale); }
};

#endif
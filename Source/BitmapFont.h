////////////////////////////////////////////////////////
//   File Name	:	"BitmapFont.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a way for the user to draw text
//					using bitmap fonts
////////////////////////////////////////////////////////

#ifndef BITMAPFONT_H
#define BITMAPFONt_H

#include "Structs.h"

class CBitmapFont
{
private:
	int m_nImageID;
	int m_nCharWidth;
	int m_nCharHeight;
	int m_nNumCols;
	char m_cStartChar;

	rect CellAlgorithm(int ID);

	///////////////////////////////////////////////////////////////////
	//	Function:	"CBitmapFont(Constructor)"
	///////////////////////////////////////////////////////////////////
	CBitmapFont(void);
	CBitmapFont(const CBitmapFont&);
	CBitmapFont& operator=(const CBitmapFont&);

	///////////////////////////////////////////////////////////////////
	//	Function:	"~CBitmapFont(Destructor)"
	///////////////////////////////////////////////////////////////////
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
	//
	///////////////////////////////////////////////////////////////////
	void Load();

	///////////////////////////////////////////////////////////////////
	// Function: “drawstring”
	//
	// Purpose: Draws the string passed in to the screen at the specified location.
	////////////////////////////////////////////////////////////////////
	void DrawString( const char* szstring, int posX, int posY, float posZ = 0.05f, float scale = 1.0f, DWORD color = -1); 

	///////////////////////////////////////////////////////////////////
	// Function: “DrawChar”
	//
	// Purpose: Draws the character passed in to the screen at the specified location.
	////////////////////////////////////////////////////////////////////
	void DrawChar(char c, int posX, int posY, float scale = 1, DWORD dwcolor = NULL);

	//////////////////////////////////////////////////////////////////////////
	// Function		:		DrawStringAutoCenter
	//
	// Purpose		:		Same as DrawString, except it auto centers horizontally(good for scrolling)
	//////////////////////////////////////////////////////////////////////////
	void DrawStringAutoCenter (const char* szString, int ScreenWidth, int yPos, float zPos = 0.09f, float fScale = 1.0f, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////
	// Function		:		DrawStringAutoCenterBox
	//
	// Purpose		:		Same as DrawString, except it auto centers horizontally(good for scrolling)
	//////////////////////////////////////////////////////////////////////////
	point DrawStringAutoCenterBox (const char* szString, int boxWidth, int startX, int yPos, float zPos = 0.09f, float fScale = 1.0f, DWORD dwColor = D3DCOLOR_XRGB(255,255,255));

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
	void SetNewID(int imageID) {m_nImageID = imageID;}
	void SetCharWidth(int width) {m_nCharWidth = width;}
	void SetCharHeight(int height) {m_nCharHeight = height;}
	void SetNumCols(int cols) {m_nNumCols = cols;}
	void ChangeBMFont(int imageID, int width, int height, int cols);
	void Reset();
	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	inline point GetSize() const	{ return point(m_nCharWidth, m_nCharHeight); }
};

#endif
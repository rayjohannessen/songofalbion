//////////////////////////////////////////////////////////////////////////
//	Filename	:	Box.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To dynamically create menu boxes and populate them with
//					specific information.
//////////////////////////////////////////////////////////////////////////
#ifndef BOX_H
#define BOX_H


#include <string>
#include "Timer.h"
using std::string;

enum {BOX_NO_BACK, BOX_WITH_BACK, };	// boxType
enum {BTN_SPECIAL, BTN_ITEM, BTN_ENDTURN, BTN_ENTER = 99, BTN_BACK = 100, };	// buttons

class CBox
{
	struct MY_POINT_FLOAT
	{
		float x;
		float y;
		MY_POINT_FLOAT& operator= (MY_POINT_FLOAT& pt)
		{
			x = pt.x; 
			y = pt.y;
			return *this;
		}
		MY_POINT_FLOAT& operator= (POINT& pt)
		{
			x = (float)pt.x; 
			y = (float)pt.y;
			return *this;
		}
		bool operator != (POINT& pt)
		{
			return ((float)pt.x != x || (float)pt.y != y);
		}
	};

// 	// which image is currently being used for the BG
	int m_nCurrImage;
	int m_BoxType;
	bool m_bAcceptInput;
	bool m_bCenterText;
	bool m_bCenterBox;

	// size/position variables
	int m_nLongestString;
	int m_nBoxWidth;
	int m_nBoxHeight;
	int m_nBoxRight;
	int m_nBoxBottom;
	int m_nPosX;
	int m_nPosY;
	float m_fPosZ;
	int m_nSpacing;
	int m_nStartXOriginal;
	int m_nStartYOriginal;
	int m_nStartTextY;
	int m_nStartTextX;
	float m_fScaleX;
	float m_fScaleY;
	float m_fTextZ;
	int m_nAlpha;
	int r;
	int g;
	int b;

	// menu items (buttons)
	bool m_bHasTitle;
	bool m_bEnterText;
	int  m_nTitleWidth;
	string* m_sItems;
	string* m_sOriginal;	// when accepting input
	float m_fTextScale;
	int m_nNumItems;
	DWORD m_dwColor;

	// mouse selection (buttons being hovered over/pressed)
	int m_nCurrSelectedIndex;
	int m_nCurrInputIndex;
	bool m_bMadeNew;
	bool m_bOverwrote;
	bool m_bIsMsgBox;
	bool m_bIsActive;
	bool m_bIsMouseInBox;
	int m_nBackType;
	MY_POINT_FLOAT m_ptOldMouse;

	// input (keys)
	string m_sInput;

	CTimer m_Timer;
 
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"CheckMouse"
	//
	//	Purpose		:	Determine if the mouse is over the box, if so, which
	//					index (button) is it over?
	//////////////////////////////////////////////////////////////////////////
	void CheckMouse(POINT mousePt);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"CheckKeysInputBox"
	//
	//	Purpose		:	Handle keyboard input for an input box
	//////////////////////////////////////////////////////////////////////////
	void CheckKeysInputBox();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"CheckKeys"
	//
	//	Purpose		:	Handle keyboard input for menu selection
	//////////////////////////////////////////////////////////////////////////
	void CheckKeys();

 public:
	CBox(int numItems, string* sItems, 
		int posX, int posY, float posZ = 0.11f, bool bhasTitle = false,
		int spacing = 35, int startX = 35, int startY = 25, 
		int imageID = -1, float fTextScale = 1.0f, 
		int red = 0, int green = 0, int blue = 0);
	~CBox();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"Render"
	//
	//	Purpose		:	Draws the drawing of the box and it's contents
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	"Input"
	//
	//	Purpose		:	Handles user input, selections and such
	//////////////////////////////////////////////////////////////////////////
	int Input(POINT mousePt, float felapsed);

	void PlaySnd();
 
	//////////////////////////////////////////////////////////////////////////
	//	Accessors / Mutators
	//////////////////////////////////////////////////////////////////////////
	int CurrImage() const { return m_nCurrImage; }
	void CurrImage(int val) { m_nCurrImage = val; }
	bool AcceptInput() const { return m_bAcceptInput; }
	void AcceptInput(bool val = true) { m_bAcceptInput = val; }
	int GetInputIndex()	const {return m_nCurrInputIndex;}
	void SetAlpha(int alpha) {m_nAlpha = alpha;}
	float PosZ() const { return m_fPosZ; }
	void PosZ(float val) { m_fPosZ = val; }
	int PosY() const { return m_nPosY; }
	void PosY(int val) { m_nPosY = val; }
	int PosX() const { return m_nPosX; }
	void PosX(int val) { m_nPosX = val; }
	void SetScale(float scale) {m_fScaleY = m_fScaleX = scale;}
	void SetScaleX(float xScale) {m_fScaleX = xScale;}
	void SetScaleY(float yScale) {m_fScaleY = yScale;}
	int BoxHeight() const { return m_nBoxHeight; }
	void BoxHeight(int val) { m_nBoxHeight = val; }
	int BoxWidth() const { return m_nBoxWidth; }
	void BoxWidth(int val) { m_nBoxWidth = val; }
	bool IsActiveBox() const { return m_bIsActive; }
	void SetActive(bool IsActive = true);
	void SetType(int type) {m_nBackType = type;}
	int BoxType() const { return m_BoxType; }
	void BoxType(int val = -1) { m_BoxType = val; }
	int BoxRight() {return m_nBoxRight;}
	bool IsMouseInBox() const { if (this != NULL)return m_bIsMouseInBox; else return 0;}
	void IsMouseInBox(bool val) { m_bIsMouseInBox = val; }
	bool IsMsgBox() const { return m_bIsMsgBox; }
	void IsMsgBox(bool val) { m_bIsMsgBox = val; if(val) m_nCurrSelectedIndex = -1;}
	string* GetItems() {return m_sItems;}
	bool GetMadeNew() {return m_bMadeNew;}
	bool GetOverwrote() {return m_bOverwrote;}
	void CenterText(bool center=true) {m_bCenterText = center;}
	void CenterBox();
};

#endif
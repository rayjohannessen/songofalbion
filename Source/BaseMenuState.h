//////////////////////////////////////////////////////////////////////////
//	Filename	:	BaseMenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	Base Class for all menu states
//////////////////////////////////////////////////////////////////////////
#ifndef CBASEMENUSTATE_H
#define CBASEMENUSTATE_H
#include "IGameState.h"

class CGame;

class CBaseMenuState : public IGameState
{
private:
	int	m_nBGImageID;				// bg image ID
	int m_nMenuClick;				// sound ID for when the user moves from one menu selection to the next
	int m_nScreenWidth;				// the program's width and height
	int m_nScreenHeight;
	int m_nBGX;						// where the background x is
	int m_nBGY;						// where the background y is
	int	m_nImageWidth;				// bg image width	
	int m_nImageHeight;				// bg image height
	int m_nCursorImageID;			// the menu selection cursor image
	int	m_nCurrMenuSelection;		// where is the menu cursor at?
	int m_nMenuCursorX;				// screen x for cursor
	int m_nMenuCusorY;				// screen y for cursor
	int m_nMenuX;					// used for menu items, should be the same for all
	int m_nMenuY;					
	int m_nMenuItemSpacing;			// the distance between one menu item and the next

	//CBaseMenuState* m_pCurrentState;// a pointer to the current menu state

	CBaseMenuState(const CBaseMenuState&);
	CBaseMenuState& operator= (const CBaseMenuState&);
protected:

	int m_nMouseX;
	int m_nMouseY;

public:
	CBaseMenuState();
	~CBaseMenuState();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Any update code goes here, pure virtual, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Update(double fElapsedTime);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	draw everything to the screen, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for all menu states, mouse or keyboard, virtual, to be overwritten
	//
	//	Return		:	true/false, false if we are exiting the game
	//////////////////////////////////////////////////////////////////////////
	virtual bool Input(double elapsedTime, POINT mousePt);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Enter
	//
	//	Purpose		:	When the state is first entered, execute this code, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Enter();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Exit
	//
	//	Purpose		:	When the state exits, execute this code, virtual, to be overwritten
	//////////////////////////////////////////////////////////////////////////
	virtual void Exit();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	CenterBGImage
	//
	//	Purpose		:	Automatically centers the background image
	//////////////////////////////////////////////////////////////////////////
	void CenterBGImage();

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	int GetCurrMenuSelection()		{return m_nCurrMenuSelection;}
	int GetMenuItemSpacing()		{return m_nMenuItemSpacing;}
	int GetCursorX()				{return m_nMenuCursorX;}
	int GetCursorY()				{return m_nMenuCusorY;}
	int GetScreenWidth()			{return m_nScreenWidth;}
	int GetScreenHeight()			{return m_nScreenHeight;}
	int GetBGImageID()				{return m_nBGImageID; }
	int GetMenuX() const			{return m_nMenuX; }
	int GetMenuY() const			{return m_nMenuY; }
	int GetBGImageHeight()			{return m_nImageHeight; }
	int GetBGImageWidth()			{return m_nImageWidth; }
	int GetBGx()					{return m_nBGX;}
	int GetBGy()					{return m_nBGY;}

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
	void SetCursorX(int x)						{m_nMenuCursorX = x;}
	void SetCursorY(int y)						{m_nMenuCusorY = y;}
	void SetCurrMenuSelection(int selection)	{m_nCurrMenuSelection = selection;}
	void SetBGImageID(int bgImageID)			{m_nBGImageID = bgImageID;}
	void SetBGWidth(int width)					{m_nImageWidth = width;}
	void SetBGHeight(int height)				{m_nImageHeight = height;}
	void SetCursorImageID(int cursorID) 		{m_nCursorImageID = cursorID;}
	void SetMenuX(int val)						{m_nMenuX = val; }
	void SetMenuY(int val)						{m_nMenuY = val; }
};

#endif
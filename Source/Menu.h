//////////////////////////////////////////////////////////////////////////
//	Filename	:	MenuState.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	A generic menu class, Used to create menus...with 
//					specialized rendering, input, and update functions
//////////////////////////////////////////////////////////////////////////
#ifndef CMenuSTATE_H
#define CMenuSTATE_H

#include "MenuDefines.h"

class CMenu
{
	MenuOption*	m_pCurrHover;
	eMenuOptionType	m_eType;
	int			m_nBGImageID;				// bg image ID
	int			m_nMenuItemSpacing;			// the distance between one menu item and the next
	DWORD		m_dwColor;
	DWORD		m_dwHoverClr;
	point		m_ptBGPos;
	point		m_ptMouse;
	MenuOptions m_vMenuOptions;

	RenderPtr m_fpRender;
	UpdatePtr m_fpUpdate;
	InputPtr  m_fpInput;

	CMenu(const CMenu&);
	CMenu& operator= (const CMenu&);

protected:

	point m_ptMousePos;

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	CenterBGImage
	//
	//	Purpose		:	Automatically centers the background image
	//////////////////////////////////////////////////////////////////////////
	void CenterBGImage();

public:

	CMenu(int imageID, point& menuPos, eMenuOptionType menuType, MenuOptions& options, 
		RenderPtr renderFunc, UpdatePtr updateFunc, InputPtr inputFunc,
		DWORD clr = YELLOW_WHITE, DWORD hoverClr = LIGHT_RED, int itemSpacing = 30, const point& bgPos = point(0, 0));

	~CMenu();

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Update
	//
	//	Purpose		:	Updates basics (menu options color...), any other updates
	//					are performed by the updateFunc
	//////////////////////////////////////////////////////////////////////////
	void Update(double dTimeStep);
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Render
	//
	//	Purpose		:	Renders basics (menu options), calls renderFunc to
	//					render specific components of the appropriate eMenuType
	//////////////////////////////////////////////////////////////////////////
	void Render();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Input
	//
	//	Purpose		:	Handle any user input for all menu states, mouse or keyboard
	//					any other input is handled by the specific inputFunc
	//
	//	Return		:	return true if changing from this menu to something else
	//////////////////////////////////////////////////////////////////////////
	bool Input(double elapsedTime, const POINT& mousePt);

	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Enter
	//
	//	Purpose		:	Play click sound, other misc stuff
	//////////////////////////////////////////////////////////////////////////
	void Enter();
	//////////////////////////////////////////////////////////////////////////
	//	Function	:	Exit
	//
	//	Purpose		:	TODO:: may not be necessary..???
	//////////////////////////////////////////////////////////////////////////
	void Exit();

	//////////////////////////////////////////////////////////////////////////
	//	Accessors
	//////////////////////////////////////////////////////////////////////////
	inline eMenuOptionType GetType()			const	{	return m_eType;				}
	inline int GetMenuItemSpacing()		const	{	return m_nMenuItemSpacing;	}
	inline int GetBGImageID()			const	{	return m_nBGImageID;		}
	inline const point& GetBGPos()		const 	{	return m_ptBGPos;			}
	inline const MenuOptions& GetOpts() const	{	return m_vMenuOptions;		}

	//////////////////////////////////////////////////////////////////////////
	//	Mutators
	//////////////////////////////////////////////////////////////////////////
};

#endif
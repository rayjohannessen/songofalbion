#pragma once

#include "UIWindowBase.h"
#include "Timer.h"
////////////////////////////////////////////////////////////////////////
//	OptionsWindow	- provides a window to display options 
//						and handle input for options
////////////////////////////////////////////////////////////////////////

class CAbilityObjectBase;
class CUIOptionsWindow : public CUIWindowBase
{
	const static int cornerOS = 22;
	const static rect cornerSrcTL;
	const static rect cornerSrcTR;
	const static rect cornerSrcBL;
	const static rect cornerSrcBR;
	const static rect sideSrc;

	CTimer m_Timer;	// delay for escape key
	CAbilityObjectBase* m_pAbility;
	bool	m_bHasCloseBtn;
	float	m_fFrameWMultiplier;
	float	m_fFrameHMultiplier;
	
	CUIOptionsWindow() {}

	void AddCloseButton();
	void DrawFrame();

public:

	CUIOptionsWindow( int imageID, CWindowVariablesBase* variables, bool hasCloseBtn = true );

	virtual ~CUIOptionsWindow();

	//////////////////////////////////////////////////////////////////////////
	//	Function:	Input
	//	Params:		POINT mouse - current screen pos of mouse
	//	Return:		eReturnCode - tells what to do with the window (close or nothing)
	//
	//	This function will call a clicked option's button action function if 
	//	applicable. Also determines if any options are being hovered or if the
	//	mouse is inside the window at all.
	//
	//////////////////////////////////////////////////////////////////////////
	eReturnCode  Input(POINT mouse, CUIWindowBase* window = NULL);
	void Update(float dTimeStep);
	void Render();
};

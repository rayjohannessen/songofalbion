#include "stdafx.h"

#include "UIOptionsWindow.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Assets.h"

const rect CUIOptionsWindow::cornerSrcTL	= rect(0, 71, 0, 76);
const rect CUIOptionsWindow::cornerSrcTR	= rect(0, 71, 76, 152);
const rect CUIOptionsWindow::cornerSrcBL	= rect(90, 161, 0, 76);
const rect CUIOptionsWindow::cornerSrcBR	= rect(90, 161, 76, 152);
const rect CUIOptionsWindow::sideSrc		= rect(71, 90, 0, 256);

CUIOptionsWindow::CUIOptionsWindow( int imageID, CWindowVariablesBase* variables, bool hasCloseBtn /*= true*/ ) : 
	CUIWindowBase(imageID, variables),
	m_bHasCloseBtn(hasCloseBtn),
	m_pAbility(NULL)
{
	// the close button will always be the last option in the list
	if(hasCloseBtn)
		AddCloseButton();

	// determine the width modifier for frame purposes:
	m_fFrameWMultiplier = (windowRect().width() / 256.0f);
	m_fFrameHMultiplier = (windowRect().height()/ 256.0f);
}

CUIOptionsWindow::~CUIOptionsWindow()
{

}

eReturnCode  CUIOptionsWindow::Input(POINT mouse, CUIWindowBase* window /*= NULL*/)
{
	m_Timer.Update();
	if (m_Timer.GetElapsed() > 0.2 && Globals::g_pDI->KeyPressed(DIK_ESCAPE))	// check for escape key pressed first
		return RC_CLOSE;

	eReturnCode index = RC_OUTSIDE_WINDOW;
	OptionsListIter iter = optionsList().begin();
	OptionsListIter end = optionsList().end();
	if (windowRect().IsPointInRect(mouse))
	{
		if (canAlpha())
			SetAlpha(255);

		for ( ; iter != end; ++iter)
		{
			if ( (*iter).IsMouseOverOption(point(mouse)) && Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT)  )
			{							// simply determines if the current option == close button (last element in optionsList)
				Globals::g_pFMOD->PlaySound(Globals::g_pAssets->GetMenuSnds()->MenuClickSnd());
				index = (*iter).GetRetCode();
				window = (*iter).ExecuteOptionAction(); // returns null if no window is created
				return index; // we're done
			}
		}
		index = RC_NO_SELECTION;
	}
	else // mouse is out of window, make sure any previously hovered options are marked as not hovered now
	{
		if (canAlpha())
			SetAlpha(TRANSPARENCY_LEVEL);	// TODO::constant alpha value for now...
		for ( ; iter != end; ++iter)
			(*iter).m_bIsHovered = false;
	}
	return index;
}

void CUIOptionsWindow::Update(float dTimeStep)
{

}

void CUIOptionsWindow::Render()
{
	// the window bg
	Globals::g_pTM->DrawWithZSort(m_nImageID, windowRect().left, windowRect().top, zPos(), scale().x, scale().y, NULL, 0.0f, 0.0f, 0.0f, bgClr());

	// the title
	if (title().size())
	{
		rect r(windowRect()); r.top += optionProperties().EdgePadding; r.bottom = 0; // don't center on the height
		Globals::g_pBitMapFont->DrawStringAutoCenter(title().c_str(), r, zPosTxtFrame(), titleScale(), titleClr());
	}
	// the options
	OptionsListIter iter = optionsList().begin();
	OptionsListIter end = optionsList().end();
	for ( ; iter != end; ++iter )
	{
		(*iter).Render(zPosTxtFrame(), 1.0f, textClr());
	}
	// the frame
	DrawFrame();
}

//////////////////////////////////////////////////////////////////////////
// private
void CUIOptionsWindow::AddCloseButton()
{
	point pos;

	int buttonSize = Globals::g_pBitMapFont->GetSize();
	switch (closePos())
	{
	case CP_TOP_LEFT:
		{
			pos.x = windowRect().left + CLOSE_BTN_EDGE_DIST;
			pos.y = windowRect().top  + CLOSE_BTN_EDGE_DIST;
		}break;
	case CP_TOP_RIGHT:
		{
			pos.x = windowRect().right - (CLOSE_BTN_EDGE_DIST + buttonSize);
			pos.y = windowRect().top + CLOSE_BTN_EDGE_DIST;
		}break;
	case CP_BOTTOM_LEFT:
		{
			pos.x = windowRect().left + CLOSE_BTN_EDGE_DIST;
			pos.y = windowRect().bottom - (CLOSE_BTN_EDGE_DIST + buttonSize);
		}break;
	case CP_BOTTOM_RIGHT:
		{
			pos.x = windowRect().right - (CLOSE_BTN_EDGE_DIST + buttonSize);
			pos.y = windowRect().bottom - (CLOSE_BTN_EDGE_DIST + buttonSize);
		}break;
	}
	rect r = rect(pos, point(buttonSize, buttonSize));
	COption opt = COption("X", RC_CLOSE, NULL, NULL, &r);
	opt.m_nTopBtmOffset = 0;
	optionsList().push_back(opt);
}

void CUIOptionsWindow::DrawFrame()
{
	//////////////////////////////////////////////////////////////////////////
	// sides
	//////////////////////////////////////////////////////////////////////////
	// top
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().left, windowRect().top-12, zPosTxtFrame(), 
		m_fFrameWMultiplier, 1.0f, &(rect)sideSrc, 0.0f, 0.0f, 0.0f, frameClr());	
	// btm
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().left, windowRect().bottom-5, zPosTxtFrame(), 
		m_fFrameWMultiplier, 0.95f, &(rect)sideSrc, 0.0f, 0.0f, 0.0f, frameClr());	
	// left
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().left+5, windowRect().top, zPosTxtFrame(), 
		0.95f, m_fFrameHMultiplier, &(rect)sideSrc, 0.0f, 0.0f, 1.57f, frameClr());
	// right
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().right+12, windowRect().top, zPosTxtFrame(), 
		0.95f, m_fFrameHMultiplier, &(rect)sideSrc, 0.0f, 0.0f, 1.57f, frameClr());

	//////////////////////////////////////////////////////////////////////////
	// corners
	//////////////////////////////////////////////////////////////////////////
	// top-left
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().left-cornerOS, windowRect().top-cornerOS, zPosTxtFrame(), 
		1.0, 1.0f, &(rect)cornerSrcTL, 0.0f, 0.0f, 0.0f, frameClr());
	// top-right
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().right-cornerSrcTL.width()+cornerOS, windowRect().top-cornerOS, zPosTxtFrame(), 
		1.0, 1.0f, &(rect)cornerSrcTR, 0.0f, 0.0f, 0.0f, frameClr());
	// btm-left
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().left-cornerOS, windowRect().bottom-cornerSrcBL.height()+cornerOS+1, zPosTxtFrame(), 
		1.0, 1.0f, &(rect)cornerSrcBL, 0.0f, 0.0f, 0.0f, frameClr());
	// btm-right
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->WindowFrame(), windowRect().right-cornerSrcBR.width()+cornerOS, windowRect().bottom-cornerSrcBR.height()+cornerOS+1, zPosTxtFrame(), 
		1.0, 1.0f, &(rect)cornerSrcBR, 0.0f, 0.0f, 0.0f, frameClr());
}
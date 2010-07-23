#include "stdafx.h"

#include "UIOptionsWindow.h"
#include "Globals.h"
#include "BitmapFont.h"

CUIOptionsWindow::CUIOptionsWindow( int imageID, CWindowVariablesBase* variables, bool hasCloseBtn /*= true*/ ) : 
	CUIWindowBase(imageID, variables),
	m_pAbility(NULL)
{
	// the close button will always be the last option in the list
	m_bHasCloseBtn = hasCloseBtn;
	if(hasCloseBtn)
		AddCloseButton();
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
		for ( ; iter != end; ++iter)
		{
			if ( (*iter).IsMouseOverOption(point(mouse)) && Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT)  )
			{							// simply determines if the current option == close button (last element in optionsList)
				if ( m_bHasCloseBtn && (&(*iter) == &(*(optionsList().end()-1))) )
					index = RC_CLOSE;
				else	// no close button or the mouse is on a different option, call the option's function
				{
					window = (*iter).ExecuteOptionAction(); // returns null if no window is created
					index = RC_SELECTION;
				}
				return index;	// we're done
			}
		}
		index = RC_NO_SELECTION;
	}
	else // mouse is out of window, make sure any previously hovered options are marked as not hovered now
	{
		for ( ; iter != end; ++iter)
			(*iter).m_bIsHovered = false;
	}
	return index;
}

void CUIOptionsWindow::Update(float fTimeStep)
{

}

void CUIOptionsWindow::Render()
{
	// the window bg
	Globals::g_pTM->DrawWithZSort(m_nImageID, windowRect().left, windowRect().top, m_fBGZPos, scale().x, scale().y, NULL, 0.0f, 0.0f, 0.0f /*, color()*/);

	// the title
	if (title().size())
	{
		rect r(windowRect()); r.top += optionProperties().EdgePadding; r.bottom = 0; // don't center on the height
		Globals::g_pBitMapFont->DrawStringAutoCenter(title().c_str(), windowRect(), zPos(), titleScale(), titleClr());
	}
	// the options
	OptionsListIter iter = optionsList().begin();
	OptionsListIter end = optionsList().end();
	for ( ; iter != end; ++iter )
	{
		(*iter).Render(zPos(), 1.0f, color(), transparent());
	}
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
	COption opt = COption("X", NULL, NULL, &r);
	opt.m_nTopBtmOffset = 0;
	optionsList().push_back(opt);
}
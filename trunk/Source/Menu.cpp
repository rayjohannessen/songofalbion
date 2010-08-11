#include "stdafx.h"

#include "Menu.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "Wrappers/CSGD_DirectInput.h"

CMenu::CMenu(int imageID, point& menuPos, eMenuOptionType menuType, MenuOptions& options,
			 RenderPtr renderFunc, UpdatePtr updateFunc, InputPtr inputFunc,
			 DWORD clr, DWORD hoverClr, int itemSpacing /*= 30*/, const point& bgPos /*= point(0, 0)*/) 
	:
m_pCurrHover(NULL),
m_eType(menuType),
m_nBGImageID(imageID),
m_nMenuItemSpacing(itemSpacing),
m_dwColor(clr),
m_dwHoverClr(hoverClr),
m_ptBGPos(bgPos)
{
	int bmfSize = Globals::g_pBitMapFont->GetSize();
	point thisPt= menuPos;
	m_fpRender = renderFunc;
	m_fpUpdate = updateFunc;
	m_fpInput  = inputFunc;
	for (unsigned i = 0; i < options.size(); ++i)
	{	// set the options except for this one (we can't click to goto the menu we're already on)
		if (options[i].Type != m_eType)
		{
			options[i].SetRect(rect(thisPt, size(options[i].Text.size() * bmfSize, bmfSize)));
			m_vMenuOptions.push_back(options[i]);
			thisPt.y += m_nMenuItemSpacing;
		}
	}
}
CMenu::~CMenu()
{

}

void CMenu::Render()
{
	if (m_nBGImageID > -1)
		Globals::g_pTM->DrawWithZSort(m_nBGImageID, m_ptBGPos.x, m_ptBGPos.y, 1.0f, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(150, 150, 150));

	// draw the title
	Globals::g_pBitMapFont->DrawStringAutoCenter(gMenuTitles[m_eType], rect(25, 0, 0, Globals::g_ptScreenSize.width), DEPTH_WNDOPTIONS, 2.0f, m_dwColor);

	// render options
	rect r;
	MenuOptIter iter, end;
	for (iter = m_vMenuOptions.begin(), end = m_vMenuOptions.end(); iter != end; ++iter)
	{
		r = (*iter).Rect; 
		r.right = 0; r.bottom = 0;
		Globals::g_pBitMapFont->DrawStringAutoCenter((*iter).Text.c_str(), r, DEPTH_WNDOPTIONS, 1.0f, m_dwColor);
	}
	if (m_pCurrHover)
	{
		r = m_pCurrHover->Rect; 
		r.right = 0; r.bottom = 0;
		Globals::g_pBitMapFont->DrawStringAutoCenter(m_pCurrHover->Text.c_str(), r, DEPTH_WNDOPTIONS, 1.0f, m_dwHoverClr);
	}

	// draw the mouse cursor
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->Mouse(), m_ptMouse.x, m_ptMouse.y, DEPTH_MOUSE);

	m_fpRender(this);
}

void CMenu::Update(double dTimeStep)
{
	m_fpUpdate(dTimeStep, this);
}

bool CMenu::Input(double elapsedTime, const POINT& mousePt)
{
	m_pCurrHover = NULL;
	m_ptMouse = mousePt;
	m_fpInput(elapsedTime, mousePt, this);

	MenuOptIter iter = m_vMenuOptions.begin(), end = m_vMenuOptions.end();
	for (; iter != end; ++iter)
	{
		if ( (*iter).Rect.IsPointInRect(mousePt))
		{
			m_pCurrHover = &(*iter);
			if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT))
			{
				(*iter).Action(NULL);
				return true;
			}
		}
	}
	return false;
}

void CMenu::Enter()
{
}
void CMenu::Exit()
{

}

//////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////
void CMenu::CenterBGImage()
{
	m_ptBGPos = point((Globals::g_ptScreenSize.width >> 1) - (Globals::g_pTM->GetTextureWidth(m_nBGImageID) >> 1), 
					  (Globals::g_ptScreenSize.height >> 1) - (Globals::g_pTM->GetTextureHeight(m_nBGImageID) >> 1));
}
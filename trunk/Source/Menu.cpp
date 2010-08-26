#include "stdafx.h"

#include "Menu.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Assets.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "Wrappers/CSGD_DirectInput.h"

CMenu::CMenu(int bgImageID, const rect& bgSrc, int musicID, point& optionsStartPos, eMenuOptionType menuType, MenuOptions& options,
			 RenderPtr renderFunc, UpdatePtr updateFunc, InputPtr inputFunc, bool optionsInLine /*= true*/,
			 DWORD clr /*= YELLOW_WHITE*/, DWORD hoverClr /*= LIGHT_RED*/, int itemSpacing /*= 30*/, const point& bgPos /*= point(0, 0)*/) 
	:
m_bOptionsInLine(optionsInLine),
m_pCurrHover(NULL),
m_eType(menuType),
m_nBGImageID(bgImageID),
m_nMusicID(musicID),
m_nMenuItemSpacing(itemSpacing),
m_dwColor(clr),
m_dwHoverClr(hoverClr),
m_ptBGPos(bgPos),
m_rBGSrc(bgSrc)
{
	int bmfSize = Globals::g_pBitMapFont->GetSize();
	m_fpRender = renderFunc;
	m_fpUpdate = updateFunc;
	m_fpInput  = inputFunc;

	// otherwise, the rects have been set before-hand
	if (optionsInLine)
	{
		point thisPt= optionsStartPos;
		for (unsigned i = 0; i < options.size(); ++i)
		{	// set the options except for this one (we can't click to goto the menu we're already on)
			if (options[i].Type != m_eType)
			{
				options[i].SetRect(rect(thisPt, size(options[i].Text.size() * int((float)bmfSize * OptionsScale), int((float)bmfSize * OptionsScale))));
				m_vMenuOptions.push_back(options[i]);
				thisPt.y += m_nMenuItemSpacing;
			}
		}
	}
	else
	{
		for (unsigned i = 0; i < options.size(); ++i)
		{	// set the options except for this one (we can't click to goto the menu we're already on)
			if (options[i].Type != m_eType)
				m_vMenuOptions.push_back(options[i]);
		}
	}
	if (m_rBGSrc.width() < Globals::g_ptScreenSize.width && m_rBGSrc.height() < Globals::g_ptScreenSize.height)
	{
		CenterBGImage();
	}
}
CMenu::~CMenu()
{

}

void CMenu::Render()
{
	if (m_nBGImageID > -1)
 		Globals::g_pTM->Render(m_nBGImageID, m_ptBGPos.x, m_ptBGPos.y, 1.0f, 1.0f, 1.0f, &m_rBGSrc, 0.0f, 0.0f, 0.0f, 0xaaffffff);

	Globals::g_pTM->Render(Globals::g_pAssets->GetGUIasts()->SOATitle(), 270, 25, 1.0f, 1.0f, 1.0f, &rect(454, 591, 0, 565));

	// draw the title
	Globals::g_pBitMapFont->DrawStringAutoCenter(gMenuTitles[m_eType], rect(145, 0, 0, Globals::g_ptScreenSize.width), DEPTH_WNDOPTIONS, TitleScale, m_dwColor);

	// render options
 	rect r;
	MenuOptIter iter, end;
	for (iter = m_vMenuOptions.begin(), end = m_vMenuOptions.end(); iter != end; ++iter)
	{
		r = (*iter).Rect; 
		if (m_bOptionsInLine)
		{ r.right = 0; r.bottom = 0; }
		Globals::g_pBitMapFont->DrawStringAutoCenter((*iter).Text.c_str(), r, DEPTH_WNDOPTIONS, OptionsScale, m_dwColor);
	}
	if (m_pCurrHover)
	{
		if (!m_bOptionsInLine) // draw the hover image
			Globals::g_pTM->Render(m_pCurrHover->HoverID, m_pCurrHover->HoverPos.x, m_pCurrHover->HoverPos.y, DEPTH_WNDOPTIONS, 1.0f, 1.0f, &m_pCurrHover->SrcRect);

		r = m_pCurrHover->Rect; 
		if (m_bOptionsInLine)
		{ r.right = 0; r.bottom = 0; }
		Globals::g_pBitMapFont->DrawStringAutoCenter(m_pCurrHover->Text.c_str(), r, DEPTH_WNDOPTIONS, OptionsScale, m_dwHoverClr);
	}

	// draw the mouse cursor
	Globals::g_pTM->Render(Globals::g_pAssets->GetGUIasts()->Mouse(), m_ptMouse.x, m_ptMouse.y, DEPTH_MOUSE);

	m_fpRender(this);
}

void CMenu::Update(double dTimeStep)
{
	m_fpUpdate(dTimeStep, this);
}

static eMenuOptionType prevHov = NUM_MENUOPTION_TYPES;
bool CMenu::Input(double elapsedTime, const POINT& mousePt)
{
	// keyboard input
	if (Globals::g_pDI->KeyPressed(DIK_ESCAPE))
	{
		if (m_eType == MOT_MAIN)
			OptionAction_Exit(NULL);
		else
		{
			OptionAction_MainMenu(NULL);
			return true;
		}
	}

	// mouse input
	m_pCurrHover = NULL;
	m_ptMouse = mousePt;
	m_fpInput(elapsedTime, mousePt, this);

	MenuOptIter iter = m_vMenuOptions.begin(), end = m_vMenuOptions.end();
	for (; iter != end; ++iter)
	{
		if ( (*iter).Rect.IsPointInRect(mousePt))
		{
			m_pCurrHover = &(*iter);

			// play hover sound, only once per option entrance
			if ((*iter).HoverSnd > -1 && !Globals::g_pFMOD->IsSoundPlaying((*iter).HoverSnd) && prevHov != m_pCurrHover->Type)
			{
				// stop any others being played
				for (MenuOptIter i = m_vMenuOptions.begin(); i != m_vMenuOptions.end(); ++i)
				{
					if ((*i).HoverSnd > -1)
						Globals::g_pFMOD->StopSound((*i).HoverSnd);
				}
				Globals::g_pFMOD->PlaySound((*iter).HoverSnd);
			}

			prevHov = m_pCurrHover->Type;

			if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT))
			{
				// stop hover sound (if playing) and play click
				if ((*iter).HoverSnd > -1 && Globals::g_pFMOD->IsSoundPlaying((*iter).HoverSnd))
					Globals::g_pFMOD->StopSound((*iter).HoverSnd);
				Globals::g_pFMOD->PlaySound((*iter).ClickSnd);

				(*iter).Action(NULL);

				return true;
			}
			return false;
		}
	}
	prevHov = NUM_MENUOPTION_TYPES; // no hover, set to NULL (NUM_MENUOPTION_TYPES == invalid/NULL)
	return false;
}

void CMenu::Enter()
{
	if (m_nMusicID > -1 && !Globals::g_pFMOD->IsSoundPlaying(m_nMusicID))
		Globals::g_pFMOD->PlaySound(m_nMusicID);
}
void CMenu::Exit(eMenuOptionType changeTo)
{
	if (m_nMusicID > -1 && changeTo == NUM_MENUOPTION_TYPES)	// only stop sound if we're changing to a non-menu (going into game for instance)
	{
		Globals::g_pFMOD->StopSound(m_nMusicID);
		Globals::g_pFMOD->ResetSound(m_nMusicID);
	}
}

//////////////////////////////////////////////////////////////////////////
// protected
//////////////////////////////////////////////////////////////////////////
void CMenu::CenterBGImage()
{
	m_ptBGPos = point((Globals::g_ptScreenSize.width >> 1) - (m_rBGSrc.width() >> 1), 
					  (Globals::g_ptScreenSize.height >> 1) - (m_rBGSrc.height() >> 1));
}
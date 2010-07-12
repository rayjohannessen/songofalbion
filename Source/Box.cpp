//////////////////////////////////////////////////////////////////////////
//	Filename	:	Box.h
//
//	Author		:	Ramon Johannessen (RJ)
//
//	Purpose		:	To dynamically create menu boxes and populate them with
//					specific information.
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "Box.h"
#include "Game.h"
#include "BitmapFont.h"
#include "Globals.h"

#define DEFAULT_SIZE	256.0f
#define MAX_INPUT_SIZE	11

CBox::CBox(int numItems, string* sItems, 
		   int posX, int posY, float posZ /* = 0.1f */, bool bHasTitle /* = false */, 
		   int spacing /* = 35 */, int startX/* = 35 */, int startY/* = 25 */, 
		   int imageID /* = -1 */, float fTextScale /* = 1.0f */, 
		   int red /* = 0 */, int green /* = 0 */, int blue /* = 0 */)
{
	m_sItems = new string[numItems];
	m_sOriginal = new string[numItems];

	//////////////////////////////////////////////////////////////////////////
	// box properties
	m_bHasTitle = bHasTitle;
	if (bHasTitle)
		m_nTitleWidth = (int)(sItems[0].size() * (34.0f * fTextScale));
	m_bIsActive = m_bIsMsgBox = m_bAcceptInput = m_bEnterText = m_bMadeNew = m_bCenterText = m_bCenterBox = m_bOverwrote = false;
	m_nBackType = BOX_NO_BACK;
	m_nPosX = posX;
	m_nPosY = posY;
	m_fPosZ = posZ;

	m_nLongestString = 8;	// 8 is the length of the BACK ESC button string, so that's default
	for (int i = 0; i < numItems; ++i)
	{
		m_sItems[i] = sItems[i];
		if (i > 0 && i < numItems)
			m_sOriginal[i] = sItems[i];	// hang on to the original in case they cancel with ESC
		if ((int)sItems[i].size() > m_nLongestString)
			m_nLongestString = sItems[i].size();
	}
	m_nLongestString *= (int)(44.0f * fTextScale); 
	m_fScaleX = (float)(m_nLongestString + (34.0f * fTextScale) + startX*2/* * 4.0f*/) / DEFAULT_SIZE; 
	int yAdds;
	if (!bHasTitle)
		yAdds = (int)(34.0f * (fTextScale*0.7f) + spacing);	// just add for the buttons on bottom (back/OK)
	else
		yAdds = (int)(34.0f * (fTextScale*0.7f) + (40 * fTextScale) + spacing);	// add for the bottom buttons AND the title
	m_fScaleY = ((float)((numItems * (34.0f * fTextScale)) + ((float)spacing * 2.0f) + startY*2) + yAdds) / DEFAULT_SIZE;
	
	m_nBoxWidth  = (int)(DEFAULT_SIZE * m_fScaleX);
	m_nBoxHeight = (int)(DEFAULT_SIZE * m_fScaleY);

	m_nBoxRight  = m_nPosX + m_nBoxWidth;
	m_nBoxBottom = m_nPosY + m_nBoxHeight;

	//////////////////////////////////////////////////////////////////////////
	// text properties
	m_fTextZ = posZ - 0.01f;
	m_nStartXOriginal = startX; m_nStartYOriginal = startY;
	m_nStartTextX = m_nPosX + startX;
	m_nStartTextY = m_nPosY + startY;
	m_nSpacing = spacing;
	m_fTextScale = fTextScale;

	//////////////////////////////////////////////////////////////////////////
	// items
	if (m_bHasTitle)
		m_nCurrSelectedIndex = 1;
	else
		m_nCurrSelectedIndex = 0;
	 m_nCurrInputIndex = -1;
	m_nNumItems = numItems;
	m_nAlpha = 255;
	r = red; g = green; b = blue;

	//////////////////////////////////////////////////////////////////////////
	if (imageID > -1)
		m_nCurrImage = imageID;	// specify set to an image that has already been loaded
	//else
	//	m_nCurrImage = CAssets::GetInstance()->aBMactionBoxID;	// default BG image

	//m_Timer.StartTimer(0.05f);	// flicker buttons!
}

CBox::~CBox()
{
	delete[] m_sItems; delete[] m_sOriginal;
	m_sItems = NULL; m_sOriginal = NULL;
}

void CBox::CheckMouse(POINT mousePt)
{
	bool changeInMouse = false;
	if (m_ptOldMouse != mousePt)
		changeInMouse = true;
		// is the mouse in the box?
	if (mousePt.x >= m_nPosX && mousePt.x <= m_nBoxRight && mousePt.y >= m_nPosY && mousePt.y <= m_nBoxBottom && !m_bEnterText)
	{
		m_nAlpha = 255; 
		m_bIsMouseInBox = true;
		// it has a back button?
		if (changeInMouse)
		{
			if (mousePt.x >= (m_nBoxRight-(25+(int)(300.0f*m_fTextScale))) && mousePt.x <= m_nBoxRight && 
				mousePt.y >= m_nBoxBottom - (45.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
			{
				m_nCurrSelectedIndex = BTN_BACK;
				m_ptOldMouse = mousePt;
				return;
			}
			if (!m_bIsMsgBox)
				m_nCurrSelectedIndex = (mousePt.y - m_nStartTextY) / (int)((float)m_nSpacing*1.3f);
			// it has an enter button?
			if (mousePt.x >= m_nPosX && mousePt.x <= (m_nPosX+(int)(200.0f*m_fTextScale)) && 
				mousePt.y >= m_nBoxBottom - (45.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
			{
				m_nCurrSelectedIndex = BTN_ENTER;
				m_ptOldMouse = mousePt;
				return;
			}
			if (m_bHasTitle)
			{
	 			if (m_nCurrSelectedIndex == 0)
				{m_nCurrSelectedIndex = -1;}
			}
	
			if (m_nCurrSelectedIndex > m_nNumItems)
			{m_nCurrSelectedIndex = -1;}
		}
		// making a selection
		if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT) && m_bAcceptInput && m_nCurrSelectedIndex > 0 && m_nCurrSelectedIndex < 5 && m_sItems[m_nCurrSelectedIndex] != "CREATE NEW")
		{
			m_nCurrInputIndex = m_nCurrSelectedIndex;
			m_nCurrSelectedIndex = BTN_ENTER;
			if (m_sItems[m_nCurrInputIndex] != m_sOriginal[m_nCurrInputIndex])
				m_bMadeNew = true;
			PlaySnd();
			return;
		}
		// changing a selection
		else if ((Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT) || Globals::g_pDI->MouseButtonPressed(MOUSE_RIGHT)) && m_bAcceptInput && m_nCurrSelectedIndex > 0 && m_nCurrSelectedIndex < 5 )
		{
			if (Globals::g_pDI->MouseButtonPressed(MOUSE_RIGHT))
				m_bOverwrote = true;
			m_sItems[m_nCurrSelectedIndex].clear();
			m_nCurrInputIndex = m_nCurrSelectedIndex;
			m_bEnterText = true;
			PlaySnd();
		}
	}
	else if (m_bEnterText && m_bAcceptInput && changeInMouse)
	{
		// it has an enter button?
		if (mousePt.x >= m_nPosX && mousePt.x <= (m_nPosX+(int)(200.0f*m_fTextScale)) && 
			mousePt.y >= m_nBoxBottom - (40.0f*m_fTextScale) && mousePt.y <= m_nBoxBottom)
		{			
			m_nCurrSelectedIndex = BTN_ENTER;
		}
		else
			m_nCurrSelectedIndex = m_nCurrInputIndex;
	}
	else if (mousePt.x < m_nPosX || mousePt.x > m_nBoxRight || mousePt.y < m_nPosY || mousePt.y > m_nBoxBottom)
	{ 
		if (m_nBackType != BOX_WITH_BACK)
			m_nAlpha = 100; 
		m_bIsMouseInBox = false;
		if(changeInMouse)
			m_nCurrSelectedIndex = -1;
	}
	m_ptOldMouse = mousePt;
}

void CBox::Render()
{
	Globals::g_pTM->DrawWithZSort(CurrImage(), PosX(), PosY(), PosZ(), m_fScaleX, m_fScaleY, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255));
	
	for (int i = 0; i < m_nNumItems; ++i)
	{
// 		if(m_Timer.GetElapsed() > 0.0475f && (i == m_nCurrSelectedIndex || i == m_nCurrInputIndex))
// 			continue;
		if (i == m_nCurrSelectedIndex || i == m_nCurrInputIndex)	// color the currently selected item
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		if (!m_bHasTitle || i > 0)
		{
			if (!m_bCenterText)
				Globals::g_pBitMapFont->DrawString(m_sItems[i].c_str(), m_nStartTextX, m_nStartTextY+(int)((float)i*((float)m_nSpacing*1.5f)), m_fTextZ, m_fTextScale, m_dwColor);
			else
				Globals::g_pBitMapFont->DrawStringAutoCenterBox(m_sItems[i].c_str(), m_nBoxWidth, m_nPosX, m_nStartTextY+(int)((float)i*((float)m_nSpacing*1.5f)), m_fTextZ, m_fTextScale, m_dwColor);
		}
		else // drawing the Title text, centered, and underlined
		{
			int centerBox = m_nBoxRight - (m_nBoxWidth >> 1);
			int centerStr = (m_nTitleWidth >> 1);
			Globals::g_pBitMapFont->DrawString(m_sItems[i].c_str(), centerBox-centerStr, m_nStartTextY+(int)((float)i*((float)m_nSpacing*1.5f)), m_fTextZ, m_fTextScale, m_dwColor);
			Globals::g_pD3D->DrawLine(centerBox-centerStr+5, m_nStartTextY + (int)((float)m_nSpacing*1.2f), centerBox-centerStr + m_nTitleWidth, m_nStartTextY + (int)((float)m_nSpacing*1.2f),
								0, 0, 0);
		}
	}
// 	else
// 	{
// 		m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
// 		int centerBox = m_nBoxRight - (m_nBoxWidth >> 1);
// 		int centerStr = (m_nTitleWidth >> 1);
// 		m_pBM->DrawString(m_sInput.c_str(), centerBox-centerStr, m_nStartTextY, m_fTextZ, m_fTextScale, m_dwColor);
// 
// 	}
	if (m_nBackType == BOX_WITH_BACK)
	{
		if (m_nCurrSelectedIndex == BTN_BACK)
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		if (!m_bIsMsgBox)
			Globals::g_pBitMapFont->DrawString("BACK-ESC", (m_nBoxRight-(25+(int)(300.0f*m_fTextScale*0.8f))), m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale * 0.7f, m_dwColor);
		else
			Globals::g_pBitMapFont->DrawString("OK", (m_nBoxRight-(25+(int)(300.0f*m_fTextScale*0.8f))), m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale * 0.7f, m_dwColor);
	}
	if (m_bAcceptInput)
	{
	    if (m_nCurrSelectedIndex == BTN_ENTER || m_nCurrInputIndex == BTN_ENTER)
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,50,50/*r, g, b*/);
		else
			m_dwColor = D3DCOLOR_ARGB(m_nAlpha, 255,255,255/*r, g, b*/);
		if (m_bAcceptInput)
			Globals::g_pBitMapFont->DrawString("ENTER", m_nPosX+25, m_nBoxBottom-(int)(40.0f*m_fTextScale), m_fTextZ, m_fTextScale * 0.7f, m_dwColor );
	}
}

int CBox::Input(POINT mousePt, float fElapsedTime)
{
//	bool timerEvent = m_Timer.Update(fElapsedTime);
	if (m_bIsActive)
		CheckMouse(mousePt);
	if (m_bIsActive && m_bAcceptInput)
		CheckKeysInputBox();
	CheckKeys();

// 	if (timerEvent)
// 	{
// 		m_Timer.StartTimer(0.05f);
// 	}

	return m_nCurrSelectedIndex;
}

void CBox::SetActive(bool bIsActive)
{
	m_bIsActive = bIsActive;
	if (!bIsActive)
	{
		m_nAlpha = 150;
		m_bIsMouseInBox = false;
	}
	else
		m_nAlpha = 255;
}

void CBox::CheckKeysInputBox()
{
	char key = Globals::g_pDI->CheckBufferedKeysEx();
	if (key >= 97 && key <= 122 || key == 8)
	{
		if (Globals::g_pDI->KeyPressed(DIK_SPACE))
			return;
		if (Globals::g_pDI->KeyPressed(DIK_BACKSPACE) && m_sItems[m_nCurrInputIndex].size() > 0)
		{
			m_sItems[m_nCurrInputIndex].erase(m_sItems[m_nCurrInputIndex].size()-1, 1);
		}
		else if (m_sItems[m_nCurrInputIndex].size() < MAX_INPUT_SIZE && !Globals::g_pDI->KeyPressed(DIK_ESCAPE) && key != 8)
		{
			char key = toupper(Globals::g_pDI->CheckKeys());
			m_sItems[m_nCurrInputIndex] += key;
		}
	}
	if (Globals::g_pDI->KeyPressed(DIK_ESCAPE) && m_nCurrInputIndex > -1)
	{
		// if they've started to enter text, don't close the box...just reset it to its original value
		m_sItems[m_nCurrInputIndex] = m_sOriginal[m_nCurrInputIndex];
		m_nCurrSelectedIndex = m_nCurrInputIndex = -1;
		if(m_bHasTitle)
			m_nCurrSelectedIndex = 1;
		m_bEnterText = false;
		m_bMadeNew = false;
	}
	else if (Globals::g_pDI->KeyPressed(DIK_ESCAPE))	// close box
	{
		m_nCurrSelectedIndex = BTN_BACK;
		m_nCurrInputIndex	 = -1;
	}
}

void CBox::CheckKeys()
{
	if (Globals::g_pDI->KeyPressed(DIK_UPARROW) && !m_bEnterText)
	{
		if (m_nCurrSelectedIndex > 90)
			m_nCurrSelectedIndex = 0;
		--m_nCurrSelectedIndex;
		if (m_bHasTitle)
		{
			if(m_nCurrSelectedIndex < 1) m_nCurrSelectedIndex = 1;
		}
		else
		{
			if(m_nCurrSelectedIndex < 0) m_nCurrSelectedIndex = 0;
		}
		PlaySnd();
	}
	else if (Globals::g_pDI->KeyPressed(DIK_DOWNARROW) && !m_bEnterText)
	{
		if (m_nCurrSelectedIndex > 90)
			m_nCurrSelectedIndex = -1;
		++m_nCurrSelectedIndex;
		if(m_nCurrSelectedIndex == m_nNumItems) m_nCurrSelectedIndex = m_nNumItems-1;
		if (m_bHasTitle && m_nCurrSelectedIndex < 1) m_nCurrSelectedIndex = 1;
		PlaySnd();
	}
	if ((Globals::g_pDI->KeyPressed(DIK_RETURN) || Globals::g_pDI->KeyPressed(DIK_NUMPADENTER)) && m_bAcceptInput && 
			m_nCurrSelectedIndex > 0 && m_sItems[m_nCurrSelectedIndex] != "CREATE NEW")
	{
		// accept the input...store it, and close the box
		m_nCurrInputIndex = m_nCurrSelectedIndex;
		m_nCurrSelectedIndex = BTN_ENTER;
		if (m_sItems[m_nCurrInputIndex] != m_sOriginal[m_nCurrInputIndex])
			m_bMadeNew = true;
		PlaySnd();
	}
	else if ((Globals::g_pDI->KeyPressed(DIK_RETURN) || Globals::g_pDI->KeyPressed(DIK_DELETE)) && m_nCurrSelectedIndex > 0 && m_bAcceptInput)
	{
		if (Globals::g_pDI->KeyPressed(DIK_DELETE))
			m_bOverwrote = true;
		m_sItems[m_nCurrSelectedIndex].clear();
		m_nCurrInputIndex = m_nCurrSelectedIndex;
		m_bEnterText = true;
		PlaySnd();
	}
}

void CBox::CenterBox()
{
	m_nPosX = ((Globals::g_pGame->GetScreenWidth()>>1) - (m_nBoxWidth>>1));
	m_nPosY = ((Globals::g_pGame->GetScreenHeight()>>1) - (m_nBoxHeight>>1));
	m_nBoxBottom= m_nPosY + m_nBoxHeight; m_nBoxRight = m_nPosX + m_nBoxWidth;
	m_nStartTextX = m_nPosX + m_nStartXOriginal; m_nStartTextY = m_nPosY + m_nStartYOriginal;
}

void CBox::PlaySnd()
{
	// 		g_pFMOD->PlaySound(g_pAssets->aMMmenuClickSnd);
	// 		if(!g_pFMOD->SetVolume(g_pAssets->aMMmenuClickSnd, Globals::g_pGame->GetSFXVolume()))
	//			MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
}
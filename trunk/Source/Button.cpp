#include "stdafx.h"

#include "Button.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Map.h"
#include "UIWindowBase.h"

void CButton::Update(double fElapsedTime)
{
	if(m_Timer.Update(fElapsedTime))
	{
		m_nState = m_nImageIDup;
		m_Timer.ResetTimer(true);
	}
}

void CButton::Input(POINT& mouse, CUIWindowBase*& window)
{
	if (m_Rect.IsPointInRect(mouse))
	{
		if (m_nState != m_nImageIDdown)
			m_nState = m_nImageIDhover;
		if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT))
		{
			m_nState = m_nImageIDdown;
			window = m_ActionFunc(Globals::g_pMap->GetSelectedObject(), *this);
			m_Timer.StartTimer(0.3);
		}
	}
	else
		m_nState = m_nImageIDup;
}

CUIWindowBase* CButton::SimulatePressed()
{
	CUIWindowBase* window = NULL;
	m_nState = m_nImageIDdown;
	window = m_ActionFunc(Globals::g_pMap->GetSelectedObject(), *this);
	m_Timer.StartTimer(0.3);

	return (CUIWindowBase*)window;
}

void CButton::Render()
{
	Globals::g_pTM->DrawWithZSort(m_nState, m_Rect.left, m_Rect.top, DEPTH_BUTTON);
	if (m_nState == m_nImageIDdown)
		Globals::g_pBitMapFont->DrawStringAutoCenterBox(m_strText.c_str(), 
														m_Rect.right - m_Rect.left , m_Rect.left, m_Rect.top + 5, DEPTH_BUTTONTXT, 
														TEXT_SCALE, GREY);
	else if (m_nState == m_nImageIDhover)
		Globals::g_pBitMapFont->DrawStringAutoCenterBox(m_strText.c_str(), 
														m_Rect.right - m_Rect.left , m_Rect.left, m_Rect.top + 5, DEPTH_BUTTONTXT,
														TEXT_SCALE, WHITE);
	else
		Globals::g_pBitMapFont->DrawStringAutoCenterBox(m_strText.c_str(), 
														m_Rect.right - m_Rect.left , m_Rect.left, m_Rect.top + 5, DEPTH_BUTTONTXT,
														TEXT_SCALE, WHITE);
}
#include "stdafx.h"

#include "Button.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Map.h"
#include "UIWindowBase.h"
#include "Assets.h"

void CButton::Update(double dTimeStep)
{
	if(m_Timer.Update(dTimeStep))
	{
		m_nState = m_nImageIDup;
		m_Timer.ResetTimer(true);
	}
}
bool CButton::Input(const POINT& mouse, CUIWindowBase*& window)
{
	if (m_Rect.IsPointInRect(mouse))
	{
		Globals::g_pMap->ToggleMapFlagOn(MF_MOUSE_IN_QUICK_BAR);
		if (m_nState != m_nImageIDdown)
			m_nState = m_nImageIDhover;
		if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT))
		{
			m_nState = m_nImageIDdown;
			window = m_ActionFunc(Globals::g_pMap->GetSelectedObject(), *this);
			if (window)
				Globals::g_pFMOD->PlaySound(Globals::g_pAssets->GetMenuSnds()->BtnInGameSnd());
			else
				Globals::g_pFMOD->PlaySound(Globals::g_pAssets->GetMenuSnds()->MenuClickSnd());
			m_Timer.StartTimer(0.3);
		}
		return true;
	}
	else
	{
		m_nState = m_nImageIDup;
		Globals::g_pMap->ToggleMapFlagOff(MF_MOUSE_IN_QUICK_BAR);
	}
	return false;
}

CUIWindowBase* CButton::SimulatePressed()
{
	CUIWindowBase* window = m_ActionFunc(Globals::g_pMap->GetSelectedObject(), *this);
	m_nState = m_nImageIDdown;
	m_Timer.StartTimer(0.3);

	return (CUIWindowBase*)window;
}

void CButton::Render()
{
	Globals::g_pTM->Render(m_nState, m_Rect.left, m_Rect.top, DEPTH_BUTTON);

	if (m_nState == m_nImageIDdown)
		Globals::g_pBitMapFont->DrawStringAutoCenter(m_strText.c_str(), m_Rect, DEPTH_BUTTONTXT, TEXT_SCALE, GREY);
	else if (m_nState == m_nImageIDhover)
		Globals::g_pBitMapFont->DrawStringAutoCenter(m_strText.c_str(), m_Rect, DEPTH_BUTTONTXT, TEXT_SCALE, WHITE);
	else
		Globals::g_pBitMapFont->DrawStringAutoCenter(m_strText.c_str(), m_Rect, DEPTH_BUTTONTXT, TEXT_SCALE, YELLOW_WHITE);
}
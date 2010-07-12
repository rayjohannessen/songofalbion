#include "stdafx.h"

#include "BaseMenuState.h"
#include "Game.h"
#include "BitmapFont.h"
#include "Globals.h"

CBaseMenuState::CBaseMenuState()
{

}
CBaseMenuState::~CBaseMenuState()
{

}
void CBaseMenuState::Render()
{
	Globals::g_pTM->DrawWithZSort(m_nBGImageID, m_nBGX, m_nBGY, 1.0f, 1.0f, 1.0f, NULL, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(150, 150, 150));
}

void CBaseMenuState::Update(double fElapsedTime)
{

}

void CBaseMenuState::Enter()
{
	//m_nMenuClick	= g_pAssets->aMMmenuMoveSnd;
	m_nMenuItemSpacing = 45;
}

bool CBaseMenuState::Input(double elapsedTime, POINT mousePt)
{
	if (Globals::g_pDI->KeyPressed(DIK_DOWN) || Globals::g_pDI->KeyPressed(DIK_UP))
	{
		if (Globals::g_pFMOD->IsSoundPlaying(m_nMenuClick))
		{
			Globals::g_pFMOD->StopSound(m_nMenuClick);
			Globals::g_pFMOD->ResetSound(m_nMenuClick);
		}
		Globals::g_pFMOD->PlaySound(m_nMenuClick);
		if(!Globals::g_pFMOD->SetVolume(m_nMenuClick, Globals::g_pGame->GetSFXVolume()))
			MessageBox(0, "VOLUME NOT SET", "ERROR", MB_OK);
	}
	return true;
}

void CBaseMenuState::Exit()
{
}

void CBaseMenuState::CenterBGImage()
{
	m_nBGX = (m_nScreenWidth >> 1) - (m_nImageWidth >> 1);
	m_nBGY = (m_nScreenHeight >> 1) - (m_nImageHeight >> 1);
}
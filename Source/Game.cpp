#include "stdafx.h"

#include "Map.h"
#include "Game.h"
#include "Player.h"
#include "Assets.h"
#include "GamePlayState.h"
#include "BitmapFont.h"
#include "Globals.h"
#include "Timer.h"
#include <ctime>

//#include "MessageSystem.h"
//#include "Factory.h"

CGame::CGame()
{
	m_pCurrentState = NULL;

	// variables
	m_bIsRunning = false;
	m_fSFXVolume = 0.5f;
	m_fMusicVolume = 0.5f;
	m_sCurrProfName = "NONE";

	srand((unsigned int)(time(0)));
	m_dTimeStep = 0.0;
}

CGame::~CGame()
{
}

CGame* CGame::GetInstance(void)
{
	static CGame instance;
	return &instance;
}

void CGame::Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed)
{
	m_pTimer = new CTimer();

	m_nScreenWidth = nScreenWidth;
	m_nScreenHeight = nScreenHeight;

	// initialize all the globals (d3d, directInput, fmod, assets, etc...)
	Globals::InitGlobals(hWnd, hInstance, nScreenWidth, nScreenHeight, bIsWindowed);

	SetIsRunning(true);

	//ChangeState(CMainMenuState::GetInstance());
	ChangeState(CGamePlayState::GetInstance());
}

void CGame::Shutdown()
{
	delete m_pTimer;
	ChangeState(NULL);

	Globals::ShutdownGlobals();
}

bool CGame::Main(POINT mouse)
{
	m_dTimeStep = m_pTimer->Update();
	
	// input, update, render
	Globals::g_pDI->ReadDevices();
	
	if ((Globals::g_pDI->KeyDown(DIK_RMENU) && Globals::g_pDI->KeyDown(DIK_RETURN))  || (Globals::g_pDI->KeyDown(DIK_LMENU) && Globals::g_pDI->KeyDown(DIK_RETURN)) )
	{
		Globals::g_pD3D->ChangeDisplayParam(Globals::g_pD3D->GetPresentParams()->BackBufferWidth, Globals::g_pD3D->GetPresentParams()->BackBufferHeight, !Globals::g_pD3D->GetPresentParams()->Windowed);
		Globals::g_pDI->ReadDevices();
	}

	if( !m_pCurrentState->Input(m_dTimeStep, mouse) )
		return false;

	m_pCurrentState->Update(m_dTimeStep);
	Globals::g_pFMOD->Update();

	//m_pMS->ProcessMsgs();

	Globals::g_pD3D->Clear(0,0,0);
	Globals::g_pD3D->DeviceBegin();
	Globals::g_pD3D->SpriteBegin();
	m_pCurrentState->Render();
	Globals::g_pD3D->SpriteEnd();
	Globals::g_pD3D->DeviceEnd();

	//CMap* Globals::g_pMap = CMap::GetInstance();
	//Globals::g_pMap->DrawDebug();

	Globals::g_pD3D->Present();

	return true;
}

void CGame::ChangeState(IGameState *pGameState)
{
	if(m_pCurrentState) { m_pCurrentState->Exit(); }

	m_pCurrentState = pGameState;

	if(m_pCurrentState) { m_pCurrentState->Enter(); }
}

void CGame::LoadSettings(void)
{
	// get settings
	m_fSFXVolume = 0.2f;
	m_fMusicVolume = 0.2f;
}

void CGame::SetMusicVolume(float _nMusicVolume)
{
	m_fMusicVolume = _nMusicVolume;
}

void CGame::SetSFXVolume(float _nSFXVolume)
{
	m_fSFXVolume = _nSFXVolume;
}

// void MessageProc(CBaseMessage* pMsg)
// {
// 	switch(pMsg->GetMsgID())
// 	{
// 	case MSG_CREATE_ITEM:
// 		{
// 			int type = rand()% 3;
// 			CCreateItem * pCP = (CCreateItem*)pMsg;
// 			Factory::GetInstance()->CreateBattleItem(type,pCP->GetNinja()->GetMapCoord() );
// 			ObjectManager::GetInstance()->Remove(pCP->GetNinja());
// 		}
// 		break;
// 	case MSG_DESTROY_ITEM:
// 		{
// 			CDestroyItem * pDP = (CDestroyItem*)pMsg;
// 			CBattleItem p = *(pDP->GetItem());
// 			CPlayer::GetInstance()->AddItem(p);
// 			ObjectManager::GetInstance()->Remove(pDP->GetItem());
// 		}
// 		break;
// 	case MSG_CREATE_WEAPON:
// 		{
// 			int type = rand()% 12;
// 			CCreateWeapon * pCP = (CCreateWeapon*)pMsg;
// 			Factory::GetInstance()->CreateWeapon(type,pCP->GetNinja()->GetMapCoord() );
// 			ObjectManager::GetInstance()->Remove(pCP->GetNinja());
// 			
// 		}
// 		break;
// 	case MSG_DESTROY_WEAPON:
// 		{
// 			CDestroyWeapon * pDW = (CDestroyWeapon*)pMsg;
// 			CBase p = *(pDW->GetWeapon());
// 
// 			if(p.GetNumType() <= 3)//leo
// 				CPlayer::GetInstance()->GetTurtles()[0]->AddWeapon(p);
// 			if(p.GetNumType() >3 && p.GetNumType() <= 6) //mikey
// 				CPlayer::GetInstance()->GetTurtles()[3]->AddWeapon(p);
// 			if(p.GetNumType() >6 && p.GetNumType() <= 9) //don
// 				CPlayer::GetInstance()->GetTurtles()[1]->AddWeapon(p);
// 			if(p.GetNumType() >9 && p.GetNumType() <= 11) //raph
// 				CPlayer::GetInstance()->GetTurtles()[2]->AddWeapon(p);
// 
// 			ObjectManager::GetInstance()->Remove(pDW->GetWeapon());
// 
// 		}
// 		break;
// 	}
// }


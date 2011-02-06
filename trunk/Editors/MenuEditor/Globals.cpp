#include "StdAfx.h"
#include "Globals.h"
#include "HUD.h"
#include "Project.h"
#include "Win32Window.h"
#include "EditorRenderer.h"

HINSTANCE			 Globals::g_hInstance	 = NULL;
CSGD_Direct3D*		 Globals::g_pD3D = NULL;
CSGD_DirectInput*	 Globals::g_pDI	 = NULL;
CSGD_TextureManager* Globals::g_pTM  = NULL;
CHUD*				 Globals::g_pHUD = NULL;
CProject*			 Globals::g_pCurrProject = NULL;
size				 Globals::g_ptScreenSize = size(0,0);
CWin32Window*		 Globals::g_pWin32Windows[NUM_EDITOR_WINDOWS] = {0};
EditorRenderer*		 Globals::g_pRenderer = NULL;
HWND				 Globals::g_hWndDlg = NULL;

Globals::Globals(void)
{
}

Globals::~Globals(void)
{
}

void Globals::InitStandalone(HWND hwnd, HINSTANCE hInstance, size& screenSize, bool windowed, bool vSync)
{
	g_pD3D = CSGD_Direct3D::GetInstance();
	g_pDI  = CSGD_DirectInput::GetInstance();
	g_pTM  = CSGD_TextureManager::GetInstance();
	g_pHUD = CHUD::GetInstance();

	g_ptScreenSize = screenSize;
	
	g_pD3D->InitDirect3D(hwnd, screenSize.width, screenSize.height, windowed, vSync);
	g_pDI->InitDirectInput(hwnd, hInstance, DI_KEYBOARD | DI_MOUSE, 0);

	// TEMP
//	g_pCurrProject = new CProject();

	InitCommon();
}

void Globals::InitInGame(LPDIRECT3DDEVICE9 d3d, D3DPRESENT_PARAMETERS& d3dpp, HINSTANCE hInstance, 
						 LPD3DXSPRITE d3dxSprite /*= NULL*/, LPD3DXFONT d3dxFont /*= NULL*/, LPD3DXLINE d3dxLine /*= NULL*/)
{
	g_ptScreenSize = size(d3dpp.BackBufferWidth, d3dpp.BackBufferHeight);

	// TODO:: need to create side windows & add menu to main window
	//		for this in-game version.. (need to resize main window height-wise
	//		to account for menu bar, make sure we don't need to account for menu bar)

	Globals::g_pD3D = CSGD_Direct3D::GetInstance();
	Globals::g_pDI  = CSGD_DirectInput::GetInstance();
	Globals::g_pTM  = CSGD_TextureManager::GetInstance();

	// setup the D3D wrapper
	LPDIRECT3D9 d3dObj; d3d->GetDirect3D(&d3dObj);
	Globals::g_pD3D->SetLPD3D9(d3dObj);					
	Globals::g_pD3D->SetLPD3DDevice9(d3d);
	Globals::g_pD3D->SetLPD3DXSprite(d3dxSprite);
	Globals::g_pD3D->SetLPD3DXFont(d3dxFont);
	Globals::g_pD3D->SetLPD3DXLine(d3dxLine);
	Globals::g_pD3D->SetD3DPP(d3dpp);
	Globals::g_pD3D->SetHWND(d3dpp.hDeviceWindow);
	g_pD3D->InitDirect3D(d3dpp.hDeviceWindow, g_ptScreenSize.width, g_ptScreenSize.height, d3dpp.Windowed, 
							d3dpp.PresentationInterval == D3DPRESENT_INTERVAL_DEFAULT);

	// input
	g_pDI->InitDirectInput(d3dpp.hDeviceWindow, hInstance, DI_KEYBOARD | DI_MOUSE, 0);

	// on with higher-level initialization..
	InitCommon();
}
void Globals::InitCommon()
{
	// under the hood inits, etc...
	g_pDI->AcquireAll();
	g_pTM->InitTextureManager(g_pD3D->GetDirect3DDevice(), g_pD3D->GetSprite());

	// this HUD displays editor-related stuff
	g_pHUD->Init();

	g_pRenderer = new EditorRenderer();
}

void Globals::Shutdown()
{
	SAFE_DELETE(g_pCurrProject);
	SAFE_DELETE(g_pRenderer);

	if (g_pHUD)
	{
		g_pHUD->Shutdown();
		g_pHUD = NULL;
	}
	if (g_pTM)
	{
		g_pTM->Shutdown();	
		g_pTM = NULL;
	}
	if (g_pDI)
	{
		g_pDI->Shutdown();
		g_pDI = NULL;
	}
	if (g_pD3D)
	{
		g_pD3D->Shutdown();	
		g_pD3D = NULL;
	}
}

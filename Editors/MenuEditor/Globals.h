#pragma once

#include "EditorWindowDetails.h"

class CSGD_TextureManager;
class CSGD_Direct3D;
class CSGD_DirectInput;
class CProject;
class CHUD;
class CWin32Window;
class EditorRenderer;

class Globals
{
	static void InitCommon();
public:
	static HINSTANCE	g_hInstance;
	static HWND			g_hWndDlg;

	static CSGD_TextureManager*	g_pTM;
	static CSGD_Direct3D*		g_pD3D;
	static CSGD_DirectInput*	g_pDI;

	static CProject* g_pCurrProject;
	static CHUD*	 g_pHUD;

	static EditorRenderer*	g_pRenderer;

	static size	g_ptScreenSize;

	static CWin32Window* g_pWin32Windows[NUM_EDITOR_WINDOWS];

	Globals(void);
	~Globals(void);

	static void InitStandalone(HWND hwnd, HINSTANCE hInstance, size& screenSize, bool windowed, bool vSync);
	static void InitInGame(LPDIRECT3DDEVICE9 d3d, D3DPRESENT_PARAMETERS& d3dpp, HINSTANCE hInstance, 
							LPD3DXSPRITE d3dxSprite = NULL, LPD3DXFONT d3dxFont = NULL, LPD3DXLINE d3dxLine = NULL);
	static void Shutdown();
};

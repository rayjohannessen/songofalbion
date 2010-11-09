#pragma once

//////////////////////////////////////////////////////////////////////////
//	MenuEditorInterface
//	- allows the MenuEditor to be used as an in-game menu editor, everything 
//		that needs to be done to initialize it is here along with any
//		optional functionality
//
//	Ray Johannessen -- 09/16/10
//////////////////////////////////////////////////////////////////////////

class CApplication;

class MenuEditorInterface
{
	CApplication*	m_pApp;

public:
	MenuEditorInterface(void);
	~MenuEditorInterface(void);

	bool Init(LPDIRECT3DDEVICE9 d3d, D3DPRESENT_PARAMETERS& d3dpp, HINSTANCE hInstance, 
			  LPD3DXSPRITE d3dxSprite = NULL, LPD3DXFONT d3dxFont = NULL, LPD3DXLINE d3dxLine = NULL);

	void Shutdown();

	void Update(float fDeltaTime);
};

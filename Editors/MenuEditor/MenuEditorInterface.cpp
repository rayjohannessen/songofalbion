#include "StdAfx.h"
#include "MenuEditorInterface.h"
#include "Application.h"

MenuEditorInterface::MenuEditorInterface(void) :
	m_pApp(NULL)
{
}

MenuEditorInterface::~MenuEditorInterface(void)
{
}

bool MenuEditorInterface::Init(LPDIRECT3DDEVICE9 d3d, D3DPRESENT_PARAMETERS& d3dpp, HINSTANCE hInstance, 
							   LPD3DXSPRITE d3dxSprite /*= NULL*/, LPD3DXFONT d3dxFont /*= NULL*/, LPD3DXLINE d3dxLine /*= NULL*/)
{
	Globals::InitInGame(d3d, d3dpp, hInstance, d3dxSprite, d3dxFont, d3dxLine);
	m_pApp = CApplication::GetInstance();

	return true;	// TODO: make sure it init's fine
}

void MenuEditorInterface::Update(float fDeltaTime)
{
	m_pApp->_MainInGame(fDeltaTime);
}

void MenuEditorInterface::Shutdown()
{
	m_pApp->Shutdown();
}
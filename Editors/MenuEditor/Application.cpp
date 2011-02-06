#include "StdAfx.h"
#include "Application.h"
#include "Project.h"
#include "Timer.h"
#include "Win32Window.h"
#include "EditorRenderer.h"

CApplication* CApplication::GetInstance()
{
	static CApplication app;
	return &app;
}

//////////////////////////////////////////////////////////////////////////
// standalone
//////////////////////////////////////////////////////////////////////////
bool CApplication::Main()
{
	static CTimer timer;
	timer.Update();
	static float timeStep;
	timeStep = (float)timer.GetTick();

	if (!_UpdateAll(timeStep))
		return false;
	_RenderAll();

	return true;
}

void CApplication::Init(HWND hwnd, HINSTANCE hInstance, size& screenSize, bool windowed, bool vSync)
{
	Globals::InitStandalone(hwnd, hInstance, screenSize, windowed, vSync);
}

void CApplication::Shutdown()
{
	Globals::Shutdown();
}

//////////////////////////////////////////////////////////////////////////
// PRIVATE
bool CApplication::_MainInGame(float fDeltaTime)
{
	if (!_UpdateAll(fDeltaTime))
		return false;

	_RenderAll();

	return true;
}
void CApplication::_RenderAll()
{
	// if we have a project
	if (Globals::g_pCurrProject)
	{
		Globals::g_pRenderer->Render();

		Globals::g_pCurrProject->Render(); // TODO:: NOT SURE WHAT THIS WOULD RENDER...
	}
	// if we do not have a project
	else
	{

	}

	// render HUD and any other elements not rendered with the project
	//////////////////////////////////////////////////////////////////////////
}

bool CApplication::_UpdateAll(float _fDeltaTime)
{
	Globals::g_pDI->ReadDevices();
	if (Globals::g_pDI->KeyPressed(DIK_ESCAPE))
		return false;

	// update
	if (Globals::g_pCurrProject)
		Globals::g_pCurrProject->Update(_fDeltaTime);

	// update all windows

	return true;
}

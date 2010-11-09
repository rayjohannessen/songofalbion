#include "StdAfx.h"
#include "Application.h"
#include "Project.h"
#include "Timer.h"
#include "Win32Window.h"

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

	return UpdateAll(timeStep);
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
#define CLR_CLR(clr) (clr.R, clr.G, clr.B)
bool CApplication::MainInGame(float fDeltaTime)
{
	return UpdateAll(fDeltaTime);
}
bool CApplication::UpdateAll( float fDeltaTime )
{
	Globals::g_pDI->ReadDevices();
	if (Globals::g_pDI->KeyPressed(DIK_ESCAPE))
		return false;

	// if we have a project
	if (Globals::g_pCurrProject)
	{
		//////////////////////////////////////////////////////////////////////////
		// Render for main window
		Globals::g_pD3D->SetBackBuffer(SC_MAIN);
		Globals::g_pD3D->Clear(CLR_CLR(Globals::g_pCurrProject->GetClearClr()));
		Globals::g_pD3D->DeviceBegin();
		Globals::g_pD3D->SpriteBegin();

		// TODO:: update/render each window separately
		// update
		Globals::g_pCurrProject->Update(fDeltaTime);
		// render
		Globals::g_pCurrProject->Render();

		Globals::g_pD3D->SpriteEnd();
		Globals::g_pD3D->DeviceEnd();
		Globals::g_pD3D->Present(Globals::g_pWin32Windows[EW_MAIN]->GetHWND(), SC_MAIN);

		//////////////////////////////////////////////////////////////////////////
		// Render for objects window
		// NOTE: only IF the window actually should be rendered...
		Globals::g_pD3D->SetBackBuffer(SC_OBJS);
		Globals::g_pD3D->Clear(CLR_CLR(Globals::g_pCurrProject->GetClearClr()));
		Globals::g_pD3D->DeviceBegin();
		Globals::g_pD3D->SpriteBegin();

		Globals::g_pD3D->SpriteEnd();
		Globals::g_pD3D->DeviceEnd();
		Globals::g_pD3D->Present(Globals::g_pWin32Windows[EW_OBJ]->GetHWND(), SC_OBJS);

		//////////////////////////////////////////////////////////////////////////
		// Render for object creation window
		// NOTE: only IF the window actually should be rendered...
		Globals::g_pD3D->SetBackBuffer(SC_OBJ_CREATE);
		Globals::g_pD3D->Clear(CLR_CLR(Globals::g_pCurrProject->GetClearClr()));
		Globals::g_pD3D->DeviceBegin();
		Globals::g_pD3D->SpriteBegin();

		Globals::g_pD3D->SpriteEnd();
		Globals::g_pD3D->DeviceEnd();
		Globals::g_pD3D->Present(Globals::g_pWin32Windows[EW_OBJ_CREATION]->GetHWND(), SC_OBJ_CREATE);
	}
	// if we do not have a project
	else
	{
		// TODO:: any sense in having any other windows open here?
		//			should do just the main window
		Globals::g_pD3D->SetBackBuffer(SC_MAIN);
		Globals::g_pD3D->Clear(100, 100, 100);
		Globals::g_pD3D->DeviceBegin();
		Globals::g_pD3D->SpriteBegin();

		Globals::g_pD3D->SpriteEnd();
		Globals::g_pD3D->DeviceEnd();
		Globals::g_pD3D->Present(Globals::g_pWin32Windows[EW_MAIN]->GetHWND(), SC_MAIN);
	}

	// render HUD and any other elements not rendered with the project

	//////////////////////////////////////////////////////////////////////////

	return true;
}
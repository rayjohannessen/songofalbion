//////////////////////////////////////////////////////////////////////////////////////////////
//	File			:	"WinMain.cpp"
//
//	Author			:	David Brown (DB), modified by Ramon Johannessen(RJ)
//	Based in part on:
//		-Window code from the book: "Physics for Game Developers" by David M. Bourg.
//		-The previous WinMain.cpp by Jensen Rivera.
//
//	Last Modified	:	3/31/2009
//
//	Purpose			:	To provide a basic window framework for student games.
//
//////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Application.h"
#include "Globals.h"
#include "Resource.h"
#include "EditorWindowDetails.h"
#include "ProcDefinitions.h"
#include "Win32Window.h"

#include <vld.h>

void ToggleFullscreenMode(HWND hWnd, 
						  int iWidth, int iHeight, int iBpp, int iRefreshRate);
void SetDisplayMode(int iWidth, int iHeight, int iBpp, int iRefreshRate);

// the app ptr
CApplication* app;
const char* g_szWINDOW_CLASS_NAME = "MENUEDITORWINDOWCLASS";	//	Window Class Name.
const char* g_szWINDOW_TITLE	  = "Menu Editor";			//	Window Title.
const int	g_nWINDOW_WIDTH		= 1024;						//	Window Width.
const int	g_nWINDOW_HEIGHT	= 768;						//	Window Height.

//	Checks to see if the game was already running in another window.
//
//	NOTE:	Don't call this function if your game needs to have more
//			than one instance running on the same computer (i.e. client/server)
BOOL CheckIfAlreadyRunning(void)
{
	//	Find a window of the same window class name and window title
	HWND hWnd = FindWindow(g_szWINDOW_CLASS_NAME, g_szWINDOW_TITLE);

	//	If one was found
	if (hWnd)
	{
		//	If it was minimized
		if (IsIconic(hWnd))
			//	restore it
			ShowWindow(hWnd, SW_RESTORE);

		//	Bring it to the front
		SetForegroundWindow(hWnd);

		return TRUE;
	}

	//	No other copies found running
	return FALSE;
}

//////////////////////////
//		WinMain			//
//////////////////////////
// THE APP
//////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;	//	Generic message.
	Globals::g_hInstance = hInstance;

	//////////////////////////////////////////////////////////////////////////
	// Don't let more than one instance of the application exist
	//
	// NOTE:	Comment out the following section of code if your game needs to have more
	//			than one instance running on the same computer (i.e. client/server)
	////////////////////////////////////////////////////////////////////////
	if (!hPrevInstance)
	{
		if (CheckIfAlreadyRunning())
			return FALSE;
	}
	////////////////////////////////////////////////////////////////////////

	// TODO : create profiles in win32window class, so that an enum can be passed into create,
	//			and it will use that profile's creation parameters, etc..

	// TODO : get values from some file, width/height...

	// TODO : this WinMain will not be used in-game...
	//			may have to add a function that can be inserted into the game's winproc
	//			to catch events and be able to use EW_MAIN's original winproc

	// ???Could also create a stand-alone menu dialogue-like window instead of using a menu?????
	Globals::g_pWin32Windows[EW_MAIN]		 = new CWin32Window( WS_OVERLAPPEDWINDOW,
														WS_EX_APPWINDOW,
														LoadMenu(hInstance, MAKEINTRESOURCE(ID_Menu)),
														true,
														"Menu Editor");
	Globals::g_pWin32Windows[EW_OBJ]		 = new CWin32Window( WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
														WS_EX_APPWINDOW,
														NULL,
														false,
														"Objects");
	Globals::g_pWin32Windows[EW_OBJ_CREATION] = new CWin32Window( WS_OVERLAPPED | WS_BORDER | WS_SYSMENU,
														WS_EX_APPWINDOW,
														NULL,
														false,
														"Create Objects");
	string classNameMain = "WndMain";
	string classNameObj  = "WndObj";
	string classNameObjCreation = "WndObjCreation";

	// setup properties, anything that's not yet set
	Globals::g_pWin32Windows[EW_MAIN]->Win32SetWndClassProps( hInstance, WindowProc, 0, classNameMain.c_str());
	Globals::g_pWin32Windows[EW_OBJ]->Win32SetWndClassProps( hInstance, ObjProc, 0, classNameObj.c_str());
	Globals::g_pWin32Windows[EW_OBJ_CREATION]->Win32SetWndClassProps( hInstance, ObjCreatorProc, 0, classNameObjCreation.c_str());

// #ifdef _DEBUG
// 	DebugWnd->Init();
// #endif

	//	Create the main window
	if ( !Globals::g_pWin32Windows[EW_MAIN]->Create(hInstance, nCmdShow) )
		return 0;

	// this window should start to the right of the main window, so wait until after 
	// the main window has been adjusted to set its rect
	rect* r = &Globals::g_pWin32Windows[EW_MAIN]->GetRect();

	Globals::g_pWin32Windows[EW_OBJ_CREATION]->SetRect( rect(r->top+WND_TOP_PXLS, r->bottom, r->right+WND_BORDER_PXLS, r->right + 200) );
	Globals::g_pWin32Windows[EW_OBJ]->SetRect( rect(r->top+WND_TOP_PXLS, r->bottom, r->left-250, r->left-4) );
	Globals::g_pWin32Windows[EW_OBJ]->SetParentWnd(Globals::g_pWin32Windows[EW_MAIN]->GetHWND());
	Globals::g_pWin32Windows[EW_OBJ_CREATION]->SetParentWnd(Globals::g_pWin32Windows[EW_MAIN]->GetHWND());

	//////////////////////////////////////////////////////////////////////////
	// create side windows
	for (unsigned i = 1; i < NUM_EDITOR_WINDOWS; ++i)
		if( !Globals::g_pWin32Windows[i]->Create(hInstance, nCmdShow) )
			return 0;

// 	Globals::g_pWin32Windows[EW_MAIN]->Win32ShowWnd(nCmdShow);
// 	Globals::g_pWin32Windows[EW_MAIN]->Win32UpdateWnd();

//	DialogBox(Globals::g_hInstance, MAKEINTRESOURCE(IDD_FORMVIEW), Globals::g_pWin32Windows[EW_MAIN]->GetHWND(), reinterpret_cast<DLGPROC>(NewProjDlgProc));
// 	DWORD err = GetLastError();

	//////////////////////////////////////////
	//	Initialize Game here
	//////////////////////////////////////////
	app = CApplication::GetInstance();
	app->Init(Globals::g_pWin32Windows[EW_MAIN]->GetHWND(), hInstance, size(g_nWINDOW_WIDTH, g_nWINDOW_HEIGHT), g_bIS_WINDOWED, true);
	//////////////////////////////////////////

	//	Enter main event loop
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			//	Test if this is a quit
			if (msg.message == WM_QUIT)
				break;
		
			//	Translate any accelerator keys
			TranslateMessage(&msg);

			//	Send the message to the window proc
			DispatchMessage(&msg);
		}

		//////////////////////////////////
		//	Put Game Logic Here
		//////////////////////////////////

 		if(app->Main() == false)
 			break;
		//////////////////////////////////
	}

// #ifdef _DEBUG
// 	DebugWnd->Shutdown();
// #endif

	/////////////////////////////////////////
	//	Shutdown Game Here
	/////////////////////////////////////////
	app->Shutdown();

	/////////////////////////////////////////
	
	//	Unregister the window class
	for (int i = 0; i < NUM_EDITOR_WINDOWS; ++i)
	{
		Globals::g_pWin32Windows[i]->Destroy(hInstance);
		delete Globals::g_pWin32Windows[i];
	}

	//	Return to Windows like this.
	return (int)(msg.wParam);
}

void ToggleFullscreenMode(HWND hWnd, 
						  int iWidth, int iHeight, int iBpp, int iRefreshRate)
{
	static bool bFullScreen = true;
	static RECT rWindow = {0,0,g_nWINDOW_WIDTH,g_nWINDOW_HEIGHT};
	static HMENU hMenu = NULL;


	if(bFullScreen)
	{
		// Remember the window position.
		GetWindowRect(hWnd, &rWindow);
		// Switch to the requested display mode.
		SetDisplayMode(iWidth, iHeight, iBpp, iRefreshRate);
		// Remember the menu, then remove it.
		hMenu = GetMenu(hWnd);
		SetMenu(hWnd, NULL);
		// Remove the window's title bar.
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
		// Put the changes to the window into effect.
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		// Size the window to cover the entire screen.
		SetWindowPos(hWnd, 0,
			0, 0,
			GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_NOZORDER);
		// Remove the cursor.
		//ShowCursor(FALSE);
	}
	else
	{
		// Restore the display mode.
		SetDisplayMode(0, 0, 0, 0);
		// Restore the window's menu.
		SetMenu(hWnd, hMenu);
		// Restore the window's title bar.
		SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		// Put the changes to the window into effect.
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
		// Size the window to its original position.
		int iWindowWidth = rWindow.right - rWindow.left;
		int iWindowHeight = rWindow.bottom - rWindow.top;
		SetWindowPos(hWnd, 0,
			rWindow.left,
			rWindow.top,
			iWindowWidth, iWindowHeight,
			SWP_NOZORDER);
		// Restore the cursor.
		ShowCursor(TRUE);
	}
	bFullScreen = !bFullScreen;
}
void SetDisplayMode(int iWidth, int iHeight, int iBpp, int iRefreshRate)
{
	if(iWidth == 0 && iHeight == 0 && iBpp == 0 && iRefreshRate == 0)
	{
		// Restore display settings to those stored in the registry.
		ChangeDisplaySettings(NULL, 0);
		return;
	}

	DEVMODE dm;
	dm.dmSize = sizeof(DEVMODE);

	int i = 0;
	while(EnumDisplaySettings(NULL, i++, &dm))
	{
		// Iterate through the display settings until a match is found.
		if(dm.dmPelsWidth == iWidth && dm.dmPelsHeight == iHeight &&
			dm.dmBitsPerPel == iBpp && dm.dmDisplayFrequency == iRefreshRate)
		{
			if(ChangeDisplaySettings(&dm, CDS_TEST) == DISP_CHANGE_SUCCESSFUL)
			{
				// Put the new settings into effect.
				ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
				return;
			}
		}
	}
}
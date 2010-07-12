//////////////////////////////////////////////////////////////////////////////////////////////
//	File			:	"WinMain.cpp"
//
//	Author			:	David Brown (DB), modified slightly by Ramon Johannessen(RJ)
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

//#define VLD_AGGREGATE_DUPLICATES
//#define VLD_MAX_DATA_DUMP 0
#include <vld.h>
#include "Game.h"
#include "Globals.h"
#include "Timer.h"
//#include "../resource.h"

void ToggleFullscreenMode(HWND hWnd, 
						  int iWidth, int iHeight, int iBpp, int iRefreshRate);
void SetDisplayMode(int iWidth, int iHeight, int iBpp, int iRefreshRate);

const char* g_szWINDOW_CLASS_NAME	= "TMNTWINDOWCLASS";		//	Window Class Name.

const char* g_szWINDOW_TITLE		= "Song of Albion";			//	Window Title.
const int	g_nWINDOW_WIDTH			= 1024;						//	Window Width.
const int	g_nWINDOW_HEIGHT		= 768;						//	Window Height.
POINT mouse;
int count;

//	Windowed or Full screen depending on project setting
#ifdef _DEBUG
	const BOOL	g_bIS_WINDOWED			= TRUE;						
#else
	const BOOL	g_bIS_WINDOWED			= FALSE;
#endif

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//	This is the main message handler of the system.

	//	What is the message 
	switch(msg)
	{
		//	To skip ALT pop up menu (system menu)
		case WM_SYSKEYUP:
		case WM_SYSCHAR:
			return(0);
		break;
		
		//	Handle ALT+F4
		case WM_CLOSE:
		{
			// Sends us a WM_DESTROY
			DestroyWindow(hWnd);			
		}
		break;
		case WM_MOUSEMOVE:
			{
				//GetCursorPos()
				mouse.x = LOWORD(lParam);
				mouse.y = HIWORD(lParam);
			}
			break;
		//	and lose/gain focus
		case WM_ACTIVATE:
		{
			//	gaining focus
			if (LOWORD(wParam) != WA_INACTIVE)
			{
				// unpause game code here
				//CGamePlayState::GetInstance()->SetPaused(false);
				//CGamePlayState::GetInstance()->SetPaused(false);
			}
			else // losing focus
			{
				// pause game code here
				//CGamePlayState::GetInstance()->SetPaused(true);
			}
			break;
		}

		case WM_SETCURSOR:
			{
				if (LOWORD(lParam) == HTCLIENT && count > -2)
				{
					count = ShowCursor(FALSE);
				}
				else if (LOWORD(lParam) == HTCAPTION && count < 0)
				{
					count = ShowCursor(TRUE);
				}
			}
		break;

		case WM_CREATE: 
		{
			//	Do initialization here
			return(0);
		}
		break;

		case WM_PAINT:
		{
			//	Start painting

// 			++nFrames;
// 			if (nFrames == 50)
// 			{
// 				float fps = 50.0f / fpsTimer.GetElapsed();
// 
// 				char szBuff[32];
// 				sprintf_s(szBuff, "FPS: %.1f", fps);
// 				SetWindowText(hWnd, szBuff);
// 
// 				nFrames = 0;
// 				fpsTimer.ResetTimer();
// 			}

			return(0);
		}
		break;

		case WM_DESTROY: 
		{
			//	Kill the application			
			PostQuitMessage(0);
			return(0);
		}
		break;

		default:
		break;
	}

	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

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

BOOL RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX	winClassEx;	//	This will describe the window class we will create.

	//	First fill in the window class structure
	winClassEx.cbSize			= sizeof(winClassEx);
	winClassEx.style			= CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winClassEx.lpfnWndProc		= WindowProc;
	winClassEx.cbClsExtra		= 0;
	winClassEx.cbWndExtra		= 0;
	winClassEx.hInstance		= hInstance;
	//winClassEx.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	winClassEx.hIcon			= LoadIcon(NULL, IDC_ARROW);
	winClassEx.hIconSm			= NULL;
	winClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	winClassEx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClassEx.lpszMenuName		= NULL; 
	winClassEx.lpszClassName	= g_szWINDOW_CLASS_NAME;

	//ShowCursor(FALSE);

	mouse.x = 0;
	mouse.y = 0;
	//	Register the window class
	return RegisterClassEx(&winClassEx);
}

//	Creates and sizes the window appropriately depending on if 
//	the application is windowed or full screen.
HWND MakeWindow(HINSTANCE hInstance)
{
	//fpsTimer.StartTimer();
	count = 0;
	// Setup window style flags
	DWORD dwWindowStyleFlags = WS_VISIBLE;

	if (g_bIS_WINDOWED)
	{
		dwWindowStyleFlags |= WS_OVERLAPPEDWINDOW;
	}
	else
	{
		dwWindowStyleFlags |= WS_POPUP;
		//ShowCursor(FALSE);	// Stop showing the mouse cursor
	}

	// Setup the desired client area size
	RECT rWindow;
	rWindow.left	= 0;
	rWindow.top		= 0;
	rWindow.right	= g_nWINDOW_WIDTH;
	rWindow.bottom	= g_nWINDOW_HEIGHT;

	// Get the dimensions of a window that will have a client rect that
	// will really be the resolution we're looking for.
	AdjustWindowRectEx(&rWindow, 
						dwWindowStyleFlags,
						FALSE, 
						WS_EX_APPWINDOW);
	
	// Calculate the width/height of that window's dimensions
	int nWindowWidth	= rWindow.right - rWindow.left;
	int nWindowHeight	= rWindow.bottom - rWindow.top;

	//	Create the window
	return CreateWindowEx(WS_EX_APPWINDOW,											//	Extended Style flags.
						  g_szWINDOW_CLASS_NAME,									//	Window Class Name.
						  g_szWINDOW_TITLE,											//	Title of the Window.
						  dwWindowStyleFlags,										//	Window Style Flags.
						  (GetSystemMetrics(SM_CXSCREEN)>>1) - (nWindowWidth>>1),	//	Window Start Point (x, y). 
						  (GetSystemMetrics(SM_CYSCREEN)>>1) - (nWindowHeight>>1),	//		-Does the math to center the window over the desktop.
						  nWindowWidth,												//	Width of Window.
						  nWindowHeight,											//	Height of Window.
						  NULL,														//	Handle to parent window.
						  NULL,														//	Handle to menu.
						  hInstance,												//	Application Instance.
						  NULL);													//	Creation parameters.
}

//////////////////////////
//		WinMain			//
//////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;	//	Generic message.
	HWND	hWnd;	//	Main Window Handle.

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

	//	Register the window class
	if (!RegisterWindowClass(hInstance))
		return 0;

#ifdef _DEBUG
	DebugWnd->Init();
#endif

	//	Create the window
	hWnd = MakeWindow(hInstance);

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//////////////////////////////////////////
	//	Initialize Game here
	//////////////////////////////////////////
	Globals::g_pGame = CGame::GetInstance();
	Globals::g_pGame->Initialize(hWnd, hInstance, g_nWINDOW_WIDTH, g_nWINDOW_HEIGHT, g_bIS_WINDOWED);

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
 		if(Globals::g_pGame->Main(mouse) == false)
 			break;
		//////////////////////////////////
	}

	/////////////////////////////////////////
	//	Shutdown Game Here
	/////////////////////////////////////////
	Globals::g_pGame->Shutdown();

#ifdef _DEBUG
	DebugWnd->Shutdown();
#endif
	/////////////////////////////////////////
	
	//	Unregister the window class
	UnregisterClass(g_szWINDOW_CLASS_NAME, hInstance);

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
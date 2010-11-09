#include "StdAfx.h"
#include "Win32Window.h"

CWin32Window::CWin32Window(
						 const WNDCLASSEX& wndClass,
						 DWORD style /*= WS_VISIBLE | WS_OVERLAPPEDWINDOW*/, 
						 DWORD extStyle /*= WS_EX_APPWINDOW*/, 
						 HMENU menu /*= NULL*/, 
						 bool center /*= true*/,
						 char* wndTitle /*= "WINDOW"*/, 
						 HWND parentWnd /*= NULL*/, 
						 const rect& r /*= rect(0, 768, 0, 1024)*/)
						 :
m_bCenter(center),
m_dwStyle(style),
m_dwExtendedStyle(extStyle),
m_szWindowTitle(wndTitle),
m_rRect(r),
m_hParentWnd(parentWnd),
m_hMenu(menu),
m_hHwnd(NULL)
{
	m_WndClass = wndClass;
}

CWin32Window::CWin32Window(
						 DWORD style /*= WS_VISIBLE | WS_OVERLAPPEDWINDOW*/, 
						 DWORD extStyle /*= WS_EX_APPWINDOW*/, 
						 HMENU menu /*= NULL*/, 
						 bool center /*= true*/,
						 char* wndTitle /*= "WINDOW"*/, 
						 HWND parentWnd /*= NULL*/, 
						 const rect& r /*= rect(0, 768, 0, 1024)*/)
						 :
m_bCenter(center),
m_dwStyle(style),
m_dwExtendedStyle(extStyle),
m_szWindowTitle(wndTitle),
m_rRect(r),
m_hParentWnd(parentWnd),
m_hMenu(menu)
{
}

CWin32Window::~CWin32Window()
{

}

bool CWin32Window::Create(HINSTANCE hInstance)
{
	// Register the window class
	if ( !RegisterClassEx(&m_WndClass) )
		return false;

	// Setup the desired client area size
	RECT rWindow;
	rWindow.left	= m_rRect.left;
	rWindow.top		= m_rRect.top;
	rWindow.right	= m_rRect.right;
	rWindow.bottom	= m_rRect.bottom;

	// Get the dimensions of a window that will have a client rect that
	// will really be the resolution we're looking for.
	AdjustWindowRectEx(&rWindow, 
		m_dwStyle,
		FALSE, 
		m_dwExtendedStyle);

	m_rRect = rWindow;

	if (m_bCenter)
		CenterWndInScreen();

	//////////////////////////////////////////////////////////////////////////
	//	Create the window
	m_hHwnd = CreateWindowEx(  m_dwExtendedStyle,	//	Extended Style flags.
		m_WndClass.lpszClassName,//	Window Class Name.
		m_szWindowTitle,	//	Title of the Window.
		m_dwStyle,			//	Window Style Flags.
		m_rRect.left,		//	Window Start Point (x, y). 
		m_rRect.top,		//	
		m_rRect.width(),	//	Width of Window.
		m_rRect.height(),	//	Height of Window.
		m_hParentWnd,		//	Handle to parent window.
		m_hMenu,			//	Handle to menu.
		hInstance,			//	Application Instance.
		NULL);				//	Creation parameters.

	return (m_hHwnd != NULL);
}

void CWin32Window::Show(int showCmd)
{
	ShowWindow(m_hHwnd, showCmd);
	DWORD err = GetLastError();
	if (err != 0)
	{
		string s = "Show Error: "; s += int(err);
		MessageBox(NULL, s.c_str(), "Window Error", MB_OK);
	}
}
void CWin32Window::Update()
{
	UpdateWindow(m_hHwnd);
}

void CWin32Window::SetWndClassProps(HINSTANCE hInstance, WNDPROC wndProc, int wndExtra /*= 0*/, const char* className /*= 0*/)
{
	m_WndClass.cbSize        = sizeof(m_WndClass);
	m_WndClass.style         = CS_HREDRAW | CS_VREDRAW;
	m_WndClass.lpfnWndProc   = wndProc;
	m_WndClass.cbClsExtra	 = 0;
	m_WndClass.cbWndExtra    = wndExtra;
	m_WndClass.hInstance     = hInstance;
	m_WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	m_WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	m_WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_WndClass.lpszMenuName  = "MainMenu";
	m_WndClass.lpszClassName = className;
	m_WndClass.hIconSm       = (HICON)LoadImage(hInstance, // small class icon
		MAKEINTRESOURCE(5),
		IMAGE_ICON,
		GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);
}

void CWin32Window::UnregClass(HINSTANCE hInstance)
{
	UnregisterClass(m_WndClass.lpszClassName, hInstance);
}

//////////////////////////////////////////////////////////////////////////
// PRIVATE
void CWin32Window::CenterWndInScreen()
{
	// TODO : right & bottom used as width/height, should be changed
	m_rRect.left = (GetSystemMetrics(SM_CXSCREEN)>>1) - (m_rRect.right>>1);	
	m_rRect.top  = (GetSystemMetrics(SM_CYSCREEN)>>1) - (m_rRect.bottom>>1);
	m_rRect.right = m_rRect.left + m_rRect.right;
	m_rRect.bottom= m_rRect.top  + m_rRect.bottom;
}
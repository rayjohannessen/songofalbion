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
						 const rect& r /*= rect(0, 768, 0, 1024)*/,
						 const ClearClr& _clrClr /*= ClearClr(200, 150, 100)*/)
						 :
m_bCenter(center),
m_dwStyle(style),
m_dwExtendedStyle(extStyle),
m_szWindowTitle(wndTitle),
m_rRect(r),
m_hParentWnd(parentWnd),
m_hMenu(menu),
m_hHwnd(NULL),
m_bVisible(true),
m_ClearClr(_clrClr),
m_pMenu(NULL)
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
						 const rect& r /*= rect(0, 768, 0, 1024)*/,
						 const ClearClr& _clrClr /*= ClearClr(200, 150, 100)*/)
						 :
m_bCenter(center),
m_dwStyle(style),
m_dwExtendedStyle(extStyle),
m_szWindowTitle(wndTitle),
m_rRect(r),
m_hParentWnd(parentWnd),
m_hMenu(menu),
m_bVisible(true),
m_ClearClr(_clrClr),
m_pMenu(NULL)
{
	// TODO:: do MENUS this way
//	m_pMenu = new CMenu();
}

CWin32Window::~CWin32Window()
{

}

bool CWin32Window::Create(HINSTANCE hInstance, int showCmd)
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

	m_OrigSize = size(m_rRect.width(), m_rRect.height());
	// Get the dimensions of a window that will have a client rect that
	// will really be the resolution we're looking for.
	AdjustWindowRectEx(&rWindow, 
		m_dwStyle,
		FALSE, 
		m_dwExtendedStyle);

	m_rRect = rWindow;

	if (m_bCenter)
		_CenterWndInScreen();

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

	if (m_hHwnd)
	{
		Win32ShowWnd(showCmd);
		Win32UpdateWnd();
	}

	return (m_hHwnd != NULL);
}
void CWin32Window::Destroy(HINSTANCE hInstance)
{
	_UnregClass(hInstance);
}

void CWin32Window::Win32ShowWnd(int showCmd)
{
	ShowWindow(m_hHwnd, showCmd);
	if (showCmd == SW_MINIMIZE)
		m_bVisible = false;
	if (showCmd == SW_RESTORE || showCmd == SW_SHOW)
		m_bVisible = true;
}
void CWin32Window::Win32UpdateWnd()
{
	UpdateWindow(m_hHwnd);
}

void CWin32Window::Win32SetWndClassProps(HINSTANCE hInstance, WNDPROC wndProc, int wndExtra /*= 0*/, const char* className /*= 0*/)
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


//////////////////////////////////////////////////////////////////////////
// PRIVATE
void CWin32Window::_CenterWndInScreen()
{
	m_rRect.left = (GetSystemMetrics(SM_CXSCREEN)>>1) - (m_OrigSize.width>>1);	
	m_rRect.top  = (GetSystemMetrics(SM_CYSCREEN)>>1) - (m_OrigSize.height>>1);
	m_rRect.right = m_rRect.left + m_OrigSize.width;
	m_rRect.bottom= m_rRect.top  + m_OrigSize.height;

	SetWindowPos(m_hHwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);

	SetWindowPos(m_hHwnd, 0,
				m_rRect.left,
				m_rRect.top,
				m_rRect.width(),
				m_rRect.height(),
				SWP_NOZORDER);
}

void CWin32Window::_UnregClass(HINSTANCE hInstance)
{
	UnregisterClass(m_WndClass.lpszClassName, hInstance);
}
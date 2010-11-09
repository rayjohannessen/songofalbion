#pragma once

#include "EditorWindowDetails.h"

class CWin32Window
{
	bool	m_bCenter;
	DWORD	m_dwStyle;
	DWORD	m_dwExtendedStyle;
	char*	m_szWindowTitle;
	rect	m_rRect;
	HWND	m_hParentWnd;
	HMENU	m_hMenu;
	// TODO : creation parameters

	HWND		m_hHwnd;
	WNDCLASSEX	m_WndClass;

	void CenterWndInScreen();

public:
	CWin32Window(const WNDCLASSEX& wndClass,
		DWORD style = WS_VISIBLE | WS_OVERLAPPEDWINDOW, 
		DWORD extStyle = WS_EX_APPWINDOW, 
		HMENU menu = NULL, 
		bool center = true,
		char* wndTitle = "WINDOW", 
		HWND parentWnd = NULL, 
		const rect& r = rect(0, 768, 0, 1024));

	// must call SetWndClassProps separately
	CWin32Window(DWORD style = WS_VISIBLE | WS_OVERLAPPEDWINDOW, 
		DWORD extStyle = WS_EX_APPWINDOW,
		HMENU menu = NULL,  
		bool center = true,
		char* wndTitle = "WINDOW", 
		HWND parentWnd = NULL, 
		const rect& r = rect(0, 768, 0, 1024));

	~CWin32Window();

	bool Create(HINSTANCE hInstance);
	void UnregClass(HINSTANCE hInstance);

	void Show(int showCmd);
	void Update();

	void SetWndClassProps(HINSTANCE hInstance, WNDPROC wndProc, int wndExtra = 0, const char* className = 0);

	//////////////////////////////////////////////////////////////////////////
	// MUTATORS
	inline void SetRect(const rect& r)		{ m_rRect = r;			}
	inline void SetMenu(HMENU menu)			{ m_hMenu = menu;		}
	inline void SetParentWnd(HWND parent)	{ m_hParentWnd = parent;}
	inline void SetHWND(HWND hwnd)			{ m_hHwnd = hwnd;		}
	//////////////////////////////////////////////////////////////////////////
	// ACCESSORS
	inline rect GetRect()		const	{ return m_rRect;		}
	inline HWND GetParentWnd()	const	{ return m_hParentWnd;	}
	inline HWND GetHWND()		const	{ return m_hHwnd;		}
};


//////////////////////////////////////////////////////////////////////////
//	WINDOW PARAMS
//////////////////////////////////////////////////////////////////////////

/*
WS_OVERLAPPEDWINDOW	-	title bar (all 3 buttons)
WS_OVERLAPPED		-	title bar (no buttons)
WS_SYSMENU			-	'X'
WS_VISIBLE			-	caused not to have focus?

WS_BORDER 
	Creates a window that has a thin-line border.

WS_CAPTION 
	Creates a window that has a title bar (includes the WS_BORDER style).

WS_CHILD 
	Creates a child window. A window with this style cannot have a menu bar. This style cannot be used with the WS_POPUP style.

WS_CHILDWINDOW 
	Same as the WS_CHILD style.

WS_CLIPCHILDREN 
	Excludes the area occupied by child windows when drawing occurs within the parent window. This style is used when creating the parent window.

WS_CLIPSIBLINGS 
	Clips child windows relative to each other; that is, when a particular child window receives a WM_PAINT message, the WS_CLIPSIBLINGS style clips all other overlapping child windows out of the region of the child window to be updated. 
	If WS_CLIPSIBLINGS is not specified and child windows overlap, it is possible, when drawing within the client area of a child window, to draw within the client area of a neighboring child window.

WS_DISABLED 
	Creates a window that is initially disabled. A disabled window cannot receive input from the user. To change this after a window has been created, use EnableWindow. 

WS_DLGFRAME 
	Creates a window that has a border of a style typically used with dialog boxes. A window with this style cannot have a title bar.

WS_GROUP 
	Specifies the first control of a group of controls. The group consists of this first control and all controls defined after it, up to the next control with the WS_GROUP style. 
	The first control in each group usually has the WS_TABSTOP style so that the user can move from group to group. The user can subsequently change the keyboard focus from one control in the group to the next control in the group by using the direction keys.
	You can turn this style on and off to change dialog box navigation. To change this style after a window has been created, use SetWindowLong.

WS_HSCROLL 
	Creates a window that has a horizontal scroll bar.

WS_ICONIC 
	Creates a window that is initially minimized. Same as the WS_MINIMIZE style.

WS_MAXIMIZE 
	Creates a window that is initially maximized.

WS_MAXIMIZEBOX 
	Creates a window that has a maximize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified. 

WS_MINIMIZE 
	Creates a window that is initially minimized. Same as the WS_ICONIC style.

WS_MINIMIZEBOX 
	creates a window that has a minimize button. Cannot be combined with the WS_EX_CONTEXTHELP style. The WS_SYSMENU style must also be specified. 

WS_OVERLAPPED 
	creates an overlapped window. An overlapped window has a title bar and a border. Same as the WS_TILED style.

WS_OVERLAPPEDWINDOW 
	creates an overlapped window with the WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, and WS_MAXIMIZEBOX styles. Same as the WS_TILEDWINDOW style. 

WS_POPUP 
	creates a pop-up window. This style cannot be used with the WS_CHILD style.

WS_POPUPWINDOW 
	creates a pop-up window with WS_BORDER, WS_POPUP, and WS_SYSMENU styles. The WS_CAPTION and WS_POPUPWINDOW styles must be combined to make the window menu visible.

WS_SIZEBOX 
	creates a window that has a sizing border. Same as the WS_THICKFRAME style.

WS_SYSMENU 
	creates a window that has a window menu on its title bar. The WS_CAPTION style must also be specified.

WS_TABSTOP 
	Specifies a control that can receive the keyboard focus when the user presses the TAB key. Pressing the TAB key changes the keyboard focus to the next control with the WS_TABSTOP style. 
	You can turn this style on and off to change dialog box navigation. To change this style after a window has been created, use SetWindowLong.


WS_THICKFRAME 
	Creates a window that has a sizing border. Same as the WS_SIZEBOX style.

WS_TILED 
	Creates an overlapped window. An overlapped window has a title bar and a border. Same as the WS_OVERLAPPED style. 

WS_TILEDWINDOW 
	Creates an overlapped window with the WS_OVERLAPPED, WS_CAPTION, WS_SYSMENU, WS_THICKFRAME, WS_MINIMIZEBOX, and WS_MAXIMIZEBOX styles. Same as the WS_OVERLAPPEDWINDOW style. 

WS_VISIBLE 
	Creates a window that is initially visible.
	This style can be turned on and off by using ShowWindow or SetWindowPos.


WS_VSCROLL 
	Creates a window that has a vertical scroll bar.
*/

//////////////////////////////////////////////////////////////////////////
// EXTENDED PARAMS
//////////////////////////////////////////////////////////////////////////

/*
WS_EX_ACCEPTFILES
	Specifies that a window created with this style accepts drag-drop files.
WS_EX_APPWINDOW
	Forces a top-level window onto the taskbar when the window is visible. 
WS_EX_CLIENTEDGE
	Specifies that a window has a border with a sunken edge.
WS_EX_COMPOSITED
	Windows XP: Paints all descendants of a window in bottom-to-top painting order using double-buffering. For more information, see Remarks. This cannot be used if the window has a class style of either CS_OWNDC or CS_CLASSDC. 
WS_EX_CONTEXTHELP
	Includes a question mark in the title bar of the window. When the user clicks the question mark, the cursor changes to a question mark with a pointer. If the user then clicks a child window, the child receives a WM_HELP message. 
	The child window should pass the message to the parent window procedure, which should call the WinHelp function using the HELP_WM_HELP command. The Help application displays a pop-up window that typically contains help for the child window.

WS_EX_CONTEXTHELP cannot be used with the WS_MAXIMIZEBOX or WS_MINIMIZEBOX styles.

WS_EX_CONTROLPARENT
	The window itself contains child windows that should take part in dialog box navigation. If this style is specified, the dialog manager recurses into children of this window when performing navigation operations such as handling the TAB key, an arrow key, or a keyboard mnemonic.
WS_EX_DLGMODALFRAME
	Creates a window that has a double border; the window can, optionally, be created with a title bar by specifying the WS_CAPTION style in the dwStyle parameter.
WS_EX_LAYERED
	Windows 2000/XP: Creates a layered window. Note that this cannot be used for child windows. Also, this cannot be used if the window has a class style of either CS_OWNDC or CS_CLASSDC. 
WS_EX_LAYOUTRTL
	Arabic and Hebrew versions of Windows 98/Me, Windows 2000/XP: Creates a window whose horizontal origin is on the right edge. Increasing horizontal values advance to the left. 
WS_EX_LEFT
	Creates a window that has generic left-aligned properties. This is the default.
WS_EX_LEFTSCROLLBAR
	If the shell language is Hebrew, Arabic, or another language that supports reading order alignment, the vertical scroll bar (if present) is to the left of the client area. For other languages, the style is ignored.
WS_EX_LTRREADING
	The window text is displayed using left-to-right reading-order properties. This is the default.
WS_EX_MDICHILD
	Creates a multiple-document interface (MDI) child window.
WS_EX_NOACTIVATE
	Windows 2000/XP: A top-level window created with this style does not become the foreground window when the user clicks it. The system does not bring this window to the foreground when the user minimizes or closes the foreground window. 
	To activate the window, use the SetActiveWindow or SetForegroundWindow function.

	The window does not appear on the taskbar by default. To force the window to appear on the taskbar, use the WS_EX_APPWINDOW style.

WS_EX_NOINHERITLAYOUT
	Windows 2000/XP: A window created with this style does not pass its window layout to its child windows.
WS_EX_NOPARENTNOTIFY
	Specifies that a child window created with this style does not send the WM_PARENTNOTIFY message to its parent window when it is created or destroyed.
WS_EX_OVERLAPPEDWINDOW
	Combines the WS_EX_CLIENTEDGE and WS_EX_WINDOWEDGE styles.
WS_EX_PALETTEWINDOW
	Combines the WS_EX_WINDOWEDGE, WS_EX_TOOLWINDOW, and WS_EX_TOPMOST styles.
WS_EX_RIGHT
	The window has generic "right-aligned" properties. This depends on the window class. This style has an effect only if the shell language is Hebrew, Arabic, or another language that supports reading-order alignment; otherwise, the style is ignored.
	Using the WS_EX_RIGHT style for static or edit controls has the same effect as using the SS_RIGHT or ES_RIGHT style, respectively. Using this style with button controls has the same effect as using BS_RIGHT and BS_RIGHTBUTTON styles. 

WS_EX_RIGHTSCROLLBAR
	Vertical scroll bar (if present) is to the right of the client area. This is the default.
WS_EX_RTLREADING
	If the shell language is Hebrew, Arabic, or another language that supports reading-order alignment, the window text is displayed using right-to-left reading-order properties. For other languages, the style is ignored.
WS_EX_STATICEDGE
	Creates a window with a three-dimensional border style intended to be used for items that do not accept user input.
WS_EX_TOOLWINDOW
	Creates a tool window; that is, a window intended to be used as a floating toolbar. A tool window has a title bar that is shorter than a normal title bar, and the window title is drawn using a smaller font. A tool window does not appear in the taskbar or in the dialog that appears when the user presses ALT+TAB. 
	If a tool window has a system menu, its icon is not displayed on the title bar. However, you can display the system menu by right-clicking or by typing ALT+SPACE. 
WS_EX_TOPMOST
	Specifies that a window created with this style should be placed above all non-topmost windows and should stay above them, even when the window is deactivated. To add or remove this style, use the SetWindowPos function.
WS_EX_TRANSPARENT
	Specifies that a window created with this style should not be painted until siblings beneath the window (that were created by the same thread) have been painted. The window appears transparent because the bits of underlying sibling windows have already been painted.

	To achieve transparency without these restrictions, use the SetWindowRgn function.

WS_EX_WINDOWEDGE
	Specifies that a window has a border with a raised edge.
*/
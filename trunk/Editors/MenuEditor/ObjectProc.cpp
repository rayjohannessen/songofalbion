#include "StdAfx.h"
#include "ProcDefinitions.h"
#include "Globals.h"
#include "Win32Window.h"
#include "Resource.h"

LRESULT CALLBACK ObjProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Globals::g_hWndDlg)
		return FALSE;

	//	What is the message 
	switch(msg)
	{
	case WM_COMMAND:
		{
// 			switch (LOWORD(wParam))
// 			{
// 
// 			}
		}
	case WM_SYSKEYUP:	//	To skip ALT pop up menu (system menu)
	case WM_SYSCHAR:
		{
			return(0);
		}break;		
	case WM_CLOSE: //	Handle ALT+F4
		{
			Globals::g_pWin32Windows[EW_OBJ]->Win32ShowWnd(SW_MINIMIZE);
			CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_Objects, MF_UNCHECKED);
			return 0;
		}break;
	case WM_MOUSEMOVE:
		{

		}break;
	case WM_ACTIVATE: //	and lose/gain focus
		{
			//	gaining focus
			if (LOWORD(wParam) != WA_INACTIVE)
			{
			}
			else // losing focus
			{
			}
		}break;
	case WM_SETCURSOR:
		{

		}break;
	case WM_CREATE: 
		{
			//	Do initialization here
			return(0);
		}break;
	case WM_PAINT:
		{
			//	Start painting
			return(0);
		}break;
	case WM_DESTROY: 
		{
			//	hide the window				
			//DestroyWindow(hWnd);	
		}break;
	default:
		{

		}break;		
	}
	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
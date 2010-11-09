#include "StdAfx.h"
#include "ProcDefinitions.h"


LRESULT CALLBACK ObjProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
		//	To skip ALT pop up menu (system menu)
	case WM_SYSKEYUP:
	case WM_SYSCHAR:
		{
			return(0);
		}break;		
		//	Handle ALT+F4
	case WM_CLOSE:
		{
			// Sends us a WM_DESTROY
			DestroyWindow(hWnd);	
		}break;
	case WM_MOUSEMOVE:
		{

		}break;
		//	and lose/gain focus
	case WM_ACTIVATE:
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
			DestroyWindow(hWnd);	
		}break;
	default:
		{

		}break;		
	}
	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		{

			return TRUE;
		}
	case WM_SHOWWINDOW:
		{

		}break;

	case WM_CLOSE:
		{
			CloseWindow(hWndDlg);
			return 0;
		}
	}
	DWORD err = GetLastError();
	if (err)
	{
		string e = "Error: " + int(err);
		MessageBox(hWndDlg, e.c_str(), "Error", MB_OK);
	}
	return FALSE;
}
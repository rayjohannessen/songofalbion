#include "StdAfx.h"
#include "ProcDefinitions.h"
#include "Globals.h"
#include "Win32Window.h"
#include "Resource.h"
#include "Project.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Globals::g_hWndDlg) // TODO:: find a way to let it repaint (or get the modal dlg working)
		return FALSE;

	//	This is the main message handler of the system.
	switch(msg)
	{
	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case ID_Exit:
				{			
					PostQuitMessage(0);
				}break;
			case ID_SaveProj:
				{

				}break;
			case ID_LoadProj:
				{

				}break;
			case ID_NewProj:
				{
					Globals::g_hWndDlg = CreateDialog(Globals::g_hInstance, MAKEINTRESOURCE(IDD_DLGFIRST), Globals::g_pWin32Windows[EW_MAIN]->GetHWND(), reinterpret_cast<DLGPROC>(NewProjDlgProc));
					ShowWindow(Globals::g_hWndDlg, SW_SHOW);
					//DialogBox(Globals::g_hInstance, MAKEINTRESOURCE(IDD_DLGFIRST), Globals::g_pWin32Windows[EW_MAIN]->GetHWND(), reinterpret_cast<DLGPROC>(NewProjDlgProc));

				}break;
			case ID_CloseProj:
				{
					// TOO:: if changes, ask if they want to save before closing:
					SAFE_DELETE(Globals::g_pCurrProject);

				}break;
			case ID_Objects:
				{
					if (Globals::g_pWin32Windows[EW_OBJ]->GetVisible())
					{
						Globals::g_pWin32Windows[EW_OBJ]->Win32ShowWnd(SW_MINIMIZE);
						CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_Objects, MF_UNCHECKED);
					} 
					else
					{
						Globals::g_pWin32Windows[EW_OBJ]->Win32ShowWnd(SW_RESTORE);
						Globals::g_pWin32Windows[EW_OBJ]->Win32UpdateWnd();
						CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_Objects, MF_CHECKED);
					}
				}break;
			case ID_ObjectCreator:
				{
					if (Globals::g_pWin32Windows[EW_OBJ_CREATION]->GetVisible())
					{
						Globals::g_pWin32Windows[EW_OBJ_CREATION]->Win32ShowWnd(SW_MINIMIZE);
						CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_ObjectCreator, MF_UNCHECKED);
					} 
					else
					{
						Globals::g_pWin32Windows[EW_OBJ_CREATION]->Win32ShowWnd(SW_RESTORE);
						Globals::g_pWin32Windows[EW_OBJ_CREATION]->Win32UpdateWnd();
						CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_ObjectCreator, MF_CHECKED);
					}
				}break;
			case ID_MainWindow_Center:
				{
					Globals::g_pWin32Windows[EW_MAIN]->Center();
					return 0;
				}break;
			default:
				{

				}
			}
		}break;
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
			//GetCursorPos()
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
			// 				if (LOWORD(lParam) == HTCLIENT && count > -2)
			// 				{
			// 					count = ShowCursor(FALSE);
			// 				}
			// 				else if (LOWORD(lParam) == HTCAPTION && count < 0)
			// 				{
			// 					count = ShowCursor(TRUE);
			// 				}
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
			//	Kill the application			
			PostQuitMessage(0);
			return(0);
		}break;
	default:
		{

		}break;		
	}

	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}


LRESULT CALLBACK NewProjDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_INITDIALOG:
			return TRUE;

		case WM_COMMAND:
			switch(wParam)
			{
			case IDOK:
				EndDialog(hWndDlg, 0);

				SAFE_DELETE(Globals::g_pCurrProject);
				Globals::g_pCurrProject = new CProject();

				Globals::g_hWndDlg = NULL;
				//ModifyMenu(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), NULL, MF_ENABLED, ID_SaveProj, NULL);
				//CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_CloseProj, MF_ENABLED);
				//CheckMenuItem(Globals::g_pWin32Windows[EW_MAIN]->GetMenu(), ID_SaveProj, MF_ENABLED);

				return TRUE;
			}
			break;
	}

	return FALSE;
}
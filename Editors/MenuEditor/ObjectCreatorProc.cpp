#include "StdAfx.h"
#include "ProcDefinitions.h"
#include "Globals.h"
#include "Win32Window.h"

LRESULT CALLBACK ObjCreatorProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
			//CloseWindow(hWnd);
			//Globals::g_pWin32Windows[EW_OBJ_CREATION]->SetHWND(hWnd);
			//DestroyWindow(hWnd);	
			ShowWindow(hWnd, SW_HIDE);
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
			//DestroyWindow(hWnd);	
		}break;
	default:
		{

		}break;		
	}
	//	Process any messages that we didn't take care of 
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}
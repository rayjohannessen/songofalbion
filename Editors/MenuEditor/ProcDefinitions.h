#pragma once

//////////////////////////////////////////////////////////////////////////
// Defines all procedures for windows in the editor

// the object window
LRESULT CALLBACK ObjProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// the object creation window
LRESULT CALLBACK ObjCreatorProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK DlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
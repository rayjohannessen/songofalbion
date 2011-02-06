#pragma once

//////////////////////////////////////////////////////////////////////////
// Defines all procedures for windows in the editor

// main window
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// the object window
LRESULT CALLBACK ObjProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// the object creation window
LRESULT CALLBACK ObjCreatorProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK NewProjDlgProc(HWND hWndDlg, UINT Msg, WPARAM wParam, LPARAM lParam);
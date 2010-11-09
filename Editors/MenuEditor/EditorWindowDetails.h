#pragma once

//	Windowed or Full screen depending on project setting
#ifdef _DEBUG
const BOOL	g_bIS_WINDOWED			= TRUE;						
#else
const BOOL	g_bIS_WINDOWED			= FALSE;
#endif

enum EditorWindows { EW_MAIN, EW_OBJ, EW_OBJ_CREATION, NUM_WINDOWS };
#define WND_TOP_PXLS 30
#define WND_BORDER_PXLS 4
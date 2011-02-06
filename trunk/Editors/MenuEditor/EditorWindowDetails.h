#pragma once

//	Windowed or Full screen depending on project setting
#ifdef _DEBUG
const BOOL	g_bIS_WINDOWED			= TRUE;						
#else
const BOOL	g_bIS_WINDOWED			= FALSE;
#endif


struct ClearClr 
{
	unsigned char R, G, B; // clear color
	ClearClr(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}
};

enum eEditorWindows { EW_MAIN, EW_OBJ, EW_OBJ_CREATION, NUM_EDITOR_WINDOWS };

#define WND_TOP_PXLS 30
#define WND_BORDER_PXLS 4

#ifdef UNUSED 
#elif defined(__GNUC__) 
# define UNUSED(x) UNUSED_ ## x __attribute__((unused)) 
#elif defined(__LCLINT__) 
# define UNUSED(x) /*@unused@*/ x 
#else 
# define UNUSED(x) x 
#endif
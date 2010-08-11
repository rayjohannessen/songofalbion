#pragma once

#include "ButtonDefines.h"

class CMenu;

enum eMenuType { MT_MAIN, MT_HELP, MT_OPTIONS, MT_PLAY, MT_EXIT, NUM_MENU_TYPES };
const static short NUM_MENUS = 4;	// main, help, options, play (back & exit are not menus, only options)
const static char* gMenuTitles[NUM_MENUS] = 
{
	"Main",
	"Help",
	"Options",
	"Play"
};

typedef void (*RenderPtr)(CMenu* const);
typedef void (*UpdatePtr)(double, CMenu* const);
typedef bool (*InputPtr)(double, const POINT&, CMenu* const);

struct MenuOption 
{
	eMenuType	Type;	// the menu type to switch to if this option is pressed (NUM_MENU_TYPES == gameplaystate)
	string		Text;
	rect		Rect;
	OptionBtnActionFP Action;	// performed when this option is selected

	MenuOption(eMenuType type, string text, OptionBtnActionFP fpAction) : Type(type), Text(text)
	{ Action = fpAction; }

	inline void SetRect(const rect& r)	{ Rect = r; }
};

typedef vector<MenuOption> MenuOptions;
typedef vector<MenuOption>::iterator MenuOptIter;
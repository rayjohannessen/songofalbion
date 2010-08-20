#pragma once

#include "ButtonDefines.h"

class CMenu;

enum eMenuOptionType { MOT_MAIN, MOT_HELP, MOT_OPTIONS, MOT_PLAY, MOT_EXIT, NUM_MENUOPTION_TYPES, NUM_INGAME_MENU_TYPES = 2 };
const static short NUM_MENUS = 3;	// main, help, options, play (back & exit are not menus, only options)
const static char* gMenuTitles[NUM_MENUS] = 
{
	"",			// main menu
	"Help",
	"Options"
};

typedef void (*RenderPtr)(CMenu* const);
typedef void (*UpdatePtr)(double, CMenu* const);
typedef bool (*InputPtr)(double, const POINT&, CMenu* const);

struct MenuOption 
{
	eMenuOptionType	Type;	// the menu type to switch to if this option is pressed
	int			HoverID;
	string		Text;
	point		HoverPos;
	rect		Rect;		// the input/render rect is set when the menu is created
	rect		SrcRect;
	OptionBtnActionFP Action;	// performed when this option is selected

	MenuOption(eMenuOptionType type, string text, OptionBtnActionFP fpAction, rect r = rect(), const rect hoverSrc = rect(), int hoverID = -1, const point& hovPos = point()) 
		: 
	Type(type), 
	HoverID(hoverID),
	Text(text), 
	HoverPos(hovPos),
	Rect(r), 
	SrcRect(hoverSrc)
	{ 
		Action = fpAction; 
	}

	// the input/render rect is set when the menu is created
	inline void SetRect(const rect& r)	{ Rect = r; }
};


typedef vector<MenuOption> MenuOptions;
typedef vector<MenuOption>::iterator MenuOptIter;
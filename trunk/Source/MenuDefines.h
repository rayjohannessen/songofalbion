#pragma once

#include "ButtonDefines.h"

class CGameMenu;

enum eMenuOptionType { MOT_MAIN, MOT_HELP, MOT_OPTIONS, MOT_PLAY, MOT_EXIT, NUM_MENUOPTION_TYPES, NUM_INGAME_MENU_TYPES = 2 };
const static short NUM_MENUS = 3;	// main, help, options, play (back & exit are not menus, only options)
const static char* gMenuTitles[NUM_MENUS] = 
{
	"",			// main menu
	"Help",
	"Options"
};
const float TitleScale = 1.5f;
const float OptionsScale = 0.95f;

typedef void (*RenderPtr)(CGameMenu* const);
typedef void (*UpdatePtr)(double, CGameMenu* const);
typedef bool (*InputPtr)(double, const POINT&, CGameMenu* const);

struct MenuOption 
{
	eMenuOptionType	Type;	// the menu type to switch to if this option is pressed
	int			HoverID;
	int			HoverSnd;
	int			ClickSnd;
	string		Text;
	point		HoverPos;
	rect		Rect;			// the input/render rect (maybe set at creation manually, or done automatically in-lined)
	rect		SrcRect;
	OptionBtnActionFP Action;	// performed when this option is selected

	MenuOption(eMenuOptionType type, string text, OptionBtnActionFP fpAction, int clickSnd, rect inputRect = rect(), int hoverSnd = -1, const rect hoverSrc = rect(), int hoverID = -1, const point& hovPos = point()) 
		: 
	Type(type), 
	HoverID(hoverID),
	HoverSnd(hoverSnd),
	ClickSnd(clickSnd),
	Text(text), 
	HoverPos(hovPos),
	Rect(inputRect), 
	SrcRect(hoverSrc)
	{ 
		Action = fpAction; 
	}

	// the input/render rect may be set when the menu is created (after CTOR call)
	inline void SetRect(const rect& r)	{ Rect = r; }
};


typedef vector<MenuOption> MenuOptions;
typedef vector<MenuOption>::iterator MenuOptIter;
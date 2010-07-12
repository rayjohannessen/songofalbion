#pragma once

const float TEXT_SCALE = 0.9f;

enum eButtonSlot	// for the locations of all available buttons
{
	BL_MENU, 
	BL_SLOT_1_1,
	BL_SLOT_1_2,
	BL_SLOT_1_3,
	BL_SLOT_2_1,
	BL_SLOT_2_2,
	BL_SLOT_2_3,
	BL_SLOT_3_1,
	BL_SLOT_3_2,
	BL_SLOT_3_3,
	BL_NUM_LOCATIONS,
	BL_INVALID = -1,
};

enum eButtonName		// the actual buttons
{
	BN_MENU,
	BN_COMBAT_SKILLS,
	BN_SKILLS,
	BN_NONCOMBAT_SKILLS,
	BN_GEAR,
	BN_SKIP,
	BN_DISBAND,
	BN_WAIT,
	BN_NUM_BUTTONS,
};

// buttons are associated with various objects.
// there are a few buttons that are constant, such as the "menu" button,
// but most buttons correspond to specific objects, e.g., a unit has "gear"
// "attacks" or "skills", etc. even these buttons may vary depending on the type of unit.
// likewise, other objects, such as cities has its own unique set of buttons.
// each non-constant object(e.g. unit)-related button displayed to the screen will correspond to the currently selected
// player object. To allow for changes between buttons needing to be displayed and what they do when clicked,
// 	a map of what buttons are needed for the given object will be queried, what each button type does will then be 
//	gotten from the button and the appropriate information will be gotten from the given source. 
class CUIWindowBase;
class CObject;
class CButton;
class COption;
class CBaseMenuState;

typedef CUIWindowBase* (*ButtonActionFP)(CObject*, CButton&);

struct ButtonSlot 
{
	rect Rect;
	CButton* pButton;	// the button (if any) that currently occupies this slot
	ButtonSlot() : pButton(NULL), Rect(0, 0, 0, 0) {}
};

// functions that a Button's action function pointer can be set to.
// they are called when the button is clicked (or a hot key is pressed)
CUIWindowBase* BtnAction_OptionBox(CObject*, CButton&);
CUIWindowBase* BtnAction_Skip(CObject*, CButton&);
CUIWindowBase* BtnAction_Disband(CObject*, CButton&);
CUIWindowBase* BtnAction_Wait(CObject*, CButton&);

//////////////////////////////////////////////////////////////////////////

typedef CUIWindowBase* (*OptionBtnActionFP)(COption*);

CUIWindowBase* OptionAction_Resume(COption*);
CUIWindowBase* OptionAction_Options(COption*);
CUIWindowBase* OptionAction_Help(COption*);
CUIWindowBase* OptionAction_MainMenu(COption*);
CUIWindowBase* OptionAction_Exit(COption*);

CUIWindowBase* OptionAction_SetQBSlot(COption*);
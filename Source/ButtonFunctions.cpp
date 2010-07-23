#include "stdafx.h"

#include "ButtonDefines.h"
#include "UIOptionsWindow.h"
#include "AbilityObjectBase.h"
#include "Object.h"
#include "Globals.h"
#include "Game.h"
#include "ObjectManager.h"
#include "Map.h"

void OptionListFromAbilities( const Abilities &skills, OptionsList& options );
void MenuOptionList( OptionsList& options );

//////////////////////////////////////////////////////////////////////////
//	Button-related functions
//////////////////////////////////////////////////////////////////////////

CUIWindowBase* BtnAction_OptionBox(CObject* obj, CButton& btn)
{
	CUIOptionsWindow* pWindow = NULL;
	// get the current object's list that corresponds to the button pressed
	switch (btn.GetButtonName())
	{	// units
	case BN_COMBAT_SKILLS:
		{
			OptionsList options;
			OptionListFromAbilities(obj->GetUnlockedAbilities(btn.GetButtonName()), options);

			point btmRight = Globals::g_ptQBPos;
			btmRight.x += 256; // size of the quick bar texture
			btmRight.y -= 10;
			CWindowVariablesBase* pVariables = new CWindowVariablesBase(btmRight, "Combat Skills", options, OptionProps());
			pWindow = new CUIOptionsWindow(Globals::g_pAssets->GetGUIasts()->BlackPixel(), pVariables);
			
		}break;
	case BN_SKILLS:		// same functionality as BN_NONCOMBAT_SKILLS, just a diff image (and therefore name)
		{

		}break;
	case BN_NONCOMBAT_SKILLS:
		{

		}break;
	case BN_GEAR:
		{

		}break;
		// cities
		// buildings
	case BN_MENU:
		{
			OptionsList options;
			MenuOptionList(options);
																				// no title
			CWindowVariablesBase* pVariables = new CWindowVariablesBase(point(0, 0), "", options, OptionProps(), PTF_DEF_ONE, CP_TOP_RIGHT, BLUE, 0.01f, 1.0f, 0, true);	
			pWindow = new CUIOptionsWindow(Globals::g_pAssets->GetGUIasts()->BlackPixel(), pVariables, false);	// no close button for menu ( resume assumes this functionality )

		}break;
	}
	return pWindow;
}
// skip means "This unit will be taken out of move list, but you could manually select it and still move"
CUIWindowBase* BtnAction_Skip(CObject* obj, CButton& btn)
{
	Globals::g_pObjManager->NextObjectRemove();

	return NULL;
}

CUIWindowBase* BtnAction_Disband(CObject* obj, CButton& btn)
{
	Globals::g_pObjManager->NextObjectRemove();
	Globals::g_pObjManager->RemoveObj(Globals::g_pCurrPlayer, obj);

	return NULL;
}
// wait means "Keep this unit in the list and come back to it later"
CUIWindowBase* BtnAction_Wait(CObject* obj, CButton& btn)
{
	Globals::g_pObjManager->NextObjectNoRemove();

	return NULL;
}

CUIWindowBase* BtnAction_EndTurn(CObject*, CButton&)
{
	Globals::g_pObjManager->EndTurn();
	return NULL;
}


void OptionListFromAbilities( const Abilities &skills, OptionsList& options )
{
	AbilitiesIter iter = skills.begin();
	AbilitiesIter end = skills.end();
	for (; iter != end; ++iter)
		options.push_back( COption((*iter)->GetName(), OptionAction_SetQBSlot, (*iter)->GetQBObj()) );
}

void MenuOptionList( OptionsList& options )
{
	options.push_back( COption("Resume", OptionAction_Resume) );
	options.push_back( COption("Options", OptionAction_Options) );
	options.push_back( COption("Help", OptionAction_Help) );
	options.push_back( COption("Main Menu", OptionAction_MainMenu) );
	options.push_back( COption("Exit", OptionAction_Exit) );
}

//////////////////////////////////////////////////////////////////////////
//	option-related functions
//////////////////////////////////////////////////////////////////////////

#include "HUD.h"
#include "Map.h"

CUIWindowBase* OptionAction_Resume(COption*)
{
	Globals::g_pHUD->CloseCurrWindow();
	// TODO:: unpause game if necessary:

	return NULL;
}

CUIWindowBase* OptionAction_Options(COption*)
{

	return NULL;
}
CUIWindowBase* OptionAction_Help(COption*)
{

	return NULL;
}
CUIWindowBase* OptionAction_MainMenu(COption*)
{

	return NULL;
}
CUIWindowBase* OptionAction_Exit(COption*)
{
	// TODO:: ask if they REALLY want to exit:

	Globals::g_pGame->Shutdown();
	exit(0);
	return NULL;
}

CUIWindowBase* OptionAction_SetQBSlot(COption* option)
{
	// TODO:: if object is already on quick bar, show that somehow
	if (!Globals::g_pMap->GetSelectedObject()->IsOnQuickBar(*option->GetQBObj()))
		Globals::g_pHUD->ToggleAddingQBObjects(option->GetQBObj());
	return NULL;
}
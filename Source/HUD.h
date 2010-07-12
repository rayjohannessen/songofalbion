//////////////////////////////////////////////////////////////////////////
// HUD.h
//
// defines and implements all rendering and interaction with the HUD,
// e.g., buttons, interfaces, window frames, etc.
//////////////////////////////////////////////////////////////////////////

#ifndef HUD_H
#define HUD_H

#include "Structs.h"
#include "Timer.h"
#include "Button.h"

class CMap;
class CPlayer;
class CObject;
class CUIWindowBase;
class CUIQuickBar;
class CQuickBarObject;

class CHUD
{
	typedef vector<CUIWindowBase*> WindowList;	// windows (boxes) that are being rendered ([0] being the farthest back)

	WindowList::iterator m_iWindowsIter, m_iEnd;

	CObject* m_pPrevSelectedPlayerObjState;
	CObject* m_pSelectedPlayerObj;	// only ever have ONE selected obj (always a player unit)
	CObject* m_pSelectedTarget;		// only ever have ONE selected enemy obj 

	WindowList m_vWindows;			// any option boxes (windows) that are currently on-screen
	ButtonSlot m_arrBtnSlots[BL_NUM_LOCATIONS];	// used for rendering pos and mouse input of all available button slots
	CButton m_arrButtons[BN_NUM_BUTTONS];		// all ACTUAL buttons

	CTriggerTimer m_Timer;			// timer for displaying clicked image

	CUIQuickBar* m_pQuickBar;			// displays currently selected unit's abilities available for easy access

	CHUD() {}
	~CHUD() {}
	CHUD(const CHUD&);
	CHUD& operator=(const CHUD&);

	void DrawTargetInfo();
	void DrawSelectedObjInfo();
	void DrawResources();
	void DrawButtons();
	// used when a different object (or none at all) is selected
	void ClearButtons();
	void SetButtons();
public:

	static CHUD* GetInstance();

	void Init();
	void InitButtons();
	void InitBtnSlots();
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	//	Renders all pieces of the HUD. Will draw all buttons originally up,
	//	any hovered-over or pressed buttons will simply be drawn over the up
	//	ones.
	//////////////////////////////////////////////////////////////////////////
	void Render();

	//////////////////////////////////////////////////////////////////////////
	// handles updating any mouse-overs,
	// as well as setting any button selects
	// and calling the appropriate functionality
	void Update(double fElapsedTime);

	// RETURN:	int - as a button index, an enum for all buttons
	int Input(POINT& mousePt);

	void CloseCurrWindow();

	//////////////////////////////////////////////////////////////////////////
	// mutators
	void SetButtonSlot(eButtonSlot slotEnum, CButton* button = NULL);
	void SetInitialQBSlots(CObject* const owner, CQuickBarObject** qbObjects);
	void SetQBSlot(int slot, CQuickBarObject* qbObj);
	void ToggleAddingQBObjects(CQuickBarObject* const qbObj);
	void ClearQBSlots();
	void AddWindow(CUIWindowBase* window);

	//////////////////////////////////////////////////////////////////////////
	// accessors
//	CButton* const GetCurrSelBtn() const	{ return m_arrBtnSlots[m_nCurrSelectedIndex].pButton; }
	CButton* const GetButtonArray()				{ return m_arrButtons;					}
	inline CButton* GetButton(eButtonSlot slot)	{ return m_arrBtnSlots[slot].pButton;	}
};

#endif
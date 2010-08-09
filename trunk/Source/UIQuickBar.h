#pragma once

#include "AbilityObjectBase.h"
#include "Timer.h"

class CObject;

#define SEPARATOR_WIDTH 3

class CUIQuickBar
{
	static double	INPUT_DRAG_DELAY;

	bool			m_bAddingNewQBObj;
	short			m_nCurrSelectedAttack;
	short			m_nCurrSelectedDefense;
	short			m_nLastIndex;		// to save the index of the clicked button in case of a drag
	CObject*		m_pOwner;
	CQuickBarObject*	m_pDragQBObj;	// will be NULL unless an object is being drug
	point			m_ptScreenPos;
	point			m_ptFirstObjPos;	// the top-left corner of the first slot
	point			m_ptMousePos;		// for dragging pos
	rect			m_rQuickBar;
	CQuickBarObject*	m_arrQBSlots[NUM_QB_SLOTS];
	rect			m_rRects[NUM_QB_SLOTS];
	CTriggerTimer*	m_Timer;

	void PlaceQBObject( unsigned i );
	void SwapQBObjects( unsigned i );

public:
	// scrnPos == top-left of the graphic
	CUIQuickBar(const point& scrnPos, const point& slotSize);
	~CUIQuickBar();

	void Update(double dElapsed);

	void Input(const POINT& mouse);
	void HandleInputRemoveObj();
	void HandleInputAddNew( unsigned i );
	bool HandleInputObjectsOnQB( unsigned i, bool bCanDrag, MouseButtons mb, short& currSelectionType );

	void Render();

	//////////////////////////////////////////////////////////////////////////
	// MUTATORS
	void InitSlots(CObject* const owner, CQuickBarObject* qbObjects[NUM_QB_SLOTS]);
	inline void SetSlot(int slot, CQuickBarObject* qbObj)	{ m_arrQBSlots[slot] = qbObj; }
	void ClearSlots();
	inline void ToggleAddingQBObj(CQuickBarObject* const qbObj)	{ m_bAddingNewQBObj = true; m_pDragQBObj = qbObj; }
	//////////////////////////////////////////////////////////////////////////
	// ACCESSORS

};
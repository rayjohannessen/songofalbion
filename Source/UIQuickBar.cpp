#include "stdafx.h"

#include "UIQuickBar.h"
#include "Object.h"
#include "Globals.h"
#include "Assets.h"
#include "Map.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "CombatSkill.h"
#include "NonCombatSkill.h"

double CUIQuickBar::INPUT_DRAG_DELAY = 0.6;

CUIQuickBar::CUIQuickBar(const point& scrnPos, const point& slotSize) 
: 
m_bAddingNewQBObj(false),
m_nCurrSelectedAttack(-1),
m_nCurrSelectedDefense(-1),
m_nLastIndex(-1),
m_ptScreenPos(scrnPos),
m_pOwner(NULL),
m_pDragQBObj(NULL),
m_Timer(NULL)
{
	m_ptFirstObjPos = scrnPos; m_ptFirstObjPos.Offset(42, 4);
	m_rQuickBar = rect(scrnPos, point(256, 38));
	int left;
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
	{
		left = m_ptFirstObjPos.x + (i * (slotSize.x + SEPARATOR_WIDTH));
		m_rRects[i] = rect(m_ptFirstObjPos.y, m_ptFirstObjPos.y + slotSize.y, left, left + slotSize.x );
		m_arrQBSlots[i] = NULL;
	}
	m_Timer = new CTriggerTimer(true, false);
}

CUIQuickBar::~CUIQuickBar()
{
	delete m_Timer;
}

void CUIQuickBar::Render()
{
	// TODO:: get image id for the actual bar from assets
	Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->QuickBar(), m_ptScreenPos.x, m_ptScreenPos.y, DEPTH_QUICKBAR);

	if (!m_pOwner)
		return;	// nothing more to render

	// render the icons
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
	{
		if (m_arrQBSlots[i])
			Globals::g_pTM->DrawWithZSort(m_arrQBSlots[i]->ImageID, m_rRects[i].left, m_rRects[i].top, DEPTH_QBICONS, 1.0f, 1.0f, &m_arrQBSlots[i]->SrcRect);
	}
	// draw the highlight around the currently selected attack ability
	if (m_nCurrSelectedAttack > -1)
	{
		rect src = rect(point(0, 0), QBSLOT_SIZE);
		int left = m_rRects[m_nCurrSelectedAttack].left;
		int top  = m_rRects[m_nCurrSelectedAttack].top;
		if (m_nCurrSelectedDefense > -1 && m_nCurrSelectedAttack == m_nCurrSelectedDefense)	// if the same ability is selected for both att/def, draw half/half
			src = rect(point(0, 0), point(QBSLOT_SIZE.x >> 1, QBSLOT_SIZE.y));

		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->AbilityImages(), left, top, 
									DEPTH_QBICONHIGHLT, 1.0f, 1.0f, &src, 0.0f, 0.0f, 0.0f, DARKBLUE);
	}
	// draw the highlight around the currently selected defense ability
	if (m_nCurrSelectedDefense > -1)
	{
		rect src = rect(point(0, 0), QBSLOT_SIZE);
		int left = m_rRects[m_nCurrSelectedDefense].left;
		int top  = m_rRects[m_nCurrSelectedDefense].top;
		if (m_nCurrSelectedAttack > -1 && m_nCurrSelectedAttack == m_nCurrSelectedDefense)	// if the same ability is selected for both att/def, draw half/half
		{
			src = rect(point(16, 0), point(QBSLOT_SIZE.x >> 1, QBSLOT_SIZE.y));
			left += 16;
		}

		Globals::g_pTM->DrawWithZSort(Globals::g_pAssets->GetGUIasts()->AbilityImages(), 
					left, top, DEPTH_QBICONHIGHLT, 1.0f, 1.0f, &src, 0.0f, 0.0f, 0.0f, RED);
	}

	// draw the drag object, if valid
	if (m_pDragQBObj)
		Globals::g_pTM->DrawWithZSort(m_pDragQBObj->ImageID, // set the icon to draw so the middle is at the tip of the mouse
									m_ptMousePos.x - int((float)QBSLOT_SIZE.x * 0.5f), m_ptMousePos.y - int((float)QBSLOT_SIZE.y * 0.5f), 
									DEPTH_QBOBJDRAG, 1.0f, 1.0f, &m_pDragQBObj->SrcRect);
}

void CUIQuickBar::Input(POINT& mouse)
{
	// determine if the mouse is over the quickbar, so we don't select anything underneath
	if (m_rQuickBar.IsPointInRect(mouse))
		Globals::g_pMap->ToggleMapFlagOn(MF_MOUSE_IN_QUICK_BAR);
	else
		Globals::g_pMap->ToggleMapFlagOff(MF_MOUSE_IN_QUICK_BAR);

	if (!m_pOwner)
		return;	// nothing more to update

	m_ptMousePos = mouse;
	bool bCanDrag = m_Timer->Update();

	bool mouseIsInASlot = false;
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
	{
		if (m_rRects[i].IsPointInRect(mouse))
		{
			// TODO::respond to hover

			if (!m_bAddingNewQBObj)	// only dealing with qb objects already on the quick bar
			{
				// mouse was pressed...
				if (HandleInputObjectsOnQB(i, bCanDrag, MOUSE_LEFT, m_nCurrSelectedAttack))
					break;
				if (HandleInputObjectsOnQB(i, bCanDrag, MOUSE_RIGHT, m_nCurrSelectedDefense))
					break;
			} 
			else	// we're adding a new one...maybe
			{

				if (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT) || Globals::g_pDI->MouseButtonPressed(MOUSE_RIGHT))
					HandleInputAddNew(i);
			}

			mouseIsInASlot = true;
			break; // mouse is in a slot, no need to keep looping
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// REMOVING QB Object being drug (mouse released anywhere outside of the QB)
	if (!mouseIsInASlot)
	{
		HandleInputRemoveObj();
	}
}

void CUIQuickBar::Update(double dElapsed)
{
}

void CUIQuickBar::InitSlots(CObject* const owner, CQuickBarObject* qbObjects[NUM_QB_SLOTS])		
{ 
	m_nCurrSelectedAttack = m_nCurrSelectedDefense = 0;
	m_pOwner = owner;
	if (!owner->GetCurrAttackAbility())
		m_nCurrSelectedAttack = -1;
	if (!owner->GetCurrDefenseAbility())
		m_nCurrSelectedDefense = -1;

	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
	{
		m_arrQBSlots[i] = qbObjects[i];
		if (qbObjects[i] && qbObjects[i]->Ability->GetType() == ABT_COMBAT_SKILL)	// find the currently selected ability, if none, defaults to 0
		{
			if (qbObjects[i]->Ability == (CAbilityObjectBase*)owner->GetCurrAttackAbility())
				m_nCurrSelectedAttack = i;
			if (qbObjects[i]->Ability == (CAbilityObjectBase*)owner->GetCurrDefenseAbility())
				m_nCurrSelectedDefense = i;
		}
	}
}

void CUIQuickBar::ClearSlots()
{
	m_nCurrSelectedAttack = m_nCurrSelectedDefense = -1;
	for (unsigned i = 0; i < NUM_QB_SLOTS; ++i)
		m_arrQBSlots[i] = NULL;
}

void CUIQuickBar::SwapQBObjects( unsigned i )
{
	CQuickBarObject* original = m_arrQBSlots[i];
	m_arrQBSlots[i] = m_pDragQBObj;
	m_pOwner->SetQBSlot(i, m_pDragQBObj);
	if (m_pOwner->GetCurrAttackAbility() == m_pDragQBObj->Ability)
		m_nCurrSelectedAttack = i;
	if (m_pOwner->GetCurrDefenseAbility() == m_pDragQBObj->Ability)
		m_nCurrSelectedDefense = i;
	m_arrQBSlots[m_nLastIndex] = original;
	m_pOwner->SetQBSlot(m_nLastIndex, original);
	if (m_pOwner->GetCurrAttackAbility() == original->Ability)
		m_nCurrSelectedAttack = m_nLastIndex;
	if (m_pOwner->GetCurrDefenseAbility() == original->Ability)
		m_nCurrSelectedDefense = m_nLastIndex;
	m_pDragQBObj = NULL;
}

void CUIQuickBar::PlaceQBObject( unsigned i )
{
	m_arrQBSlots[i] = m_pDragQBObj;
	if (m_pOwner->GetCurrAttackAbility() == m_pDragQBObj->Ability)
		m_nCurrSelectedAttack = i;
	if (m_pOwner->GetCurrDefenseAbility()== m_pDragQBObj->Ability)
		m_nCurrSelectedDefense = i;
	if (m_nLastIndex > -1)
		m_pOwner->SetQBSlot(m_nLastIndex, NULL);
	m_pOwner->SetQBSlot(i, m_pDragQBObj);
	m_pDragQBObj = NULL;
}

bool CUIQuickBar::HandleInputObjectsOnQB( unsigned i, bool bCanDrag, MouseButtons mb, short& currSelectionType )
{
	if (Globals::g_pDI->MouseButtonPressed(mb))
	{// over a valid object
		if (m_arrQBSlots[i])
		{
			m_nLastIndex = i;
			m_Timer->StartTimer(INPUT_DRAG_DELAY);	// begin counter to drag
		}
	}
	// we've released the mouse..
	else if (Globals::g_pDI->MouseButtonReleased(mb))
	{
		// there's a valid object:
		if (m_arrQBSlots[i])
		{
			// if we're dragging, take out what's there, place in the new one, and move the original to wherever the new one came from
			if (m_pDragQBObj)
			{
				SwapQBObjects(i);
			}
			// if we're not dragging, and this was the originally clicked slot, select this object 
			else
			{
				if (i == m_nLastIndex)
				{
					currSelectionType = i;
					if (m_arrQBSlots[i]->Ability->GetType() == ABT_COMBAT_SKILL)
					{
						m_pOwner->SetCurrAbilOfType((eAbSelType)mb, (CCombatSkill*)m_arrQBSlots[i]->Ability);
					} 
					else
					{
					}
				}
			}
		} 
		else // there's nothing there
		{
			// if we're dragging, simply place the new one here
			if (m_pDragQBObj)
			{
				PlaceQBObject(i);
			}
			// otherwise, do nothing
		}
		m_nLastIndex = -1;
	}
	//////////////////////////////////////////////////////////////////////////
	// DRAG QB Object
	// determine if we've held the l-btn down long enough to start dragging
	else if (m_nLastIndex == i && Globals::g_pDI->MouseButtonDown(mb))
	{
		// start dragging currently selected ability, only if we're not already doing so
		if (!m_pDragQBObj && bCanDrag)
		{
			m_pDragQBObj = m_arrQBSlots[m_nLastIndex];
			m_arrQBSlots[m_nLastIndex] = NULL;	// remove it from the array
			return true;
		}
	}
	return false;
}

void CUIQuickBar::HandleInputAddNew( unsigned i )
{
	// there's already one here...
	if (m_arrQBSlots[i])
	{// save the original..the user can move that to a new slot or get rid of it the same way they placed this one
		CQuickBarObject* original = m_arrQBSlots[i];
		PlaceQBObject(i);
		m_pDragQBObj = original;
	} 
	else // empty slot
	{
		PlaceQBObject(i);
		m_bAddingNewQBObj = false;
	}
}

void CUIQuickBar::HandleInputRemoveObj()
{
	// a qb object that was already in the QB:
	if (m_nLastIndex > -1 && m_pDragQBObj && (Globals::g_pDI->MouseButtonReleased(MOUSE_LEFT) || Globals::g_pDI->MouseButtonReleased(MOUSE_RIGHT)))
	{
		// remove it from the owner's arr:
		if (m_pDragQBObj->Ability == m_pOwner->GetCurrAttackAbility())
		{
			m_pOwner->SetCurrAttackAbility(NULL);
			m_nCurrSelectedAttack = -1;
		}
		if (m_pDragQBObj->Ability == m_pOwner->GetCurrDefenseAbility())
		{
			m_nCurrSelectedDefense = -1;
			m_pOwner->SetCurrDefenseAbility(NULL);
		}
		m_pOwner->SetQBSlot(m_nLastIndex, NULL);
		m_nLastIndex = -1;
		m_pDragQBObj = NULL;
	} 
	// decided not to add the new qb object:
	else if (m_bAddingNewQBObj && (Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT) || Globals::g_pDI->MouseButtonPressed(MOUSE_RIGHT)))
	{
		m_pDragQBObj = NULL;
		m_bAddingNewQBObj = false;
	}
}
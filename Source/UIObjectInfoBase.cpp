#include "stdafx.h"

#include "UIObjectInfoBase.h"
#include "Globals.h"
#include "Assets.h"
#include "Object.h"

UIObjectInfoBase::UIObjectInfoBase(CObject* const obj, eObjectInfoType type, 
								   int equipID, int infoID, int inventoryID, 
								   Grid& gInv, Grid& gEquip,
								   const point& invPos, const point& equipPos, const point& infoPos, 
								   const size& invInput, const size& equipInput) :
	m_eType(type),
	m_pObject(obj),
	m_nEquippedImage(equipID),
	m_nInfoImage(infoID),
	m_nInventoryImage(inventoryID),
	m_uiInventorySlots(gInv.NUM_COLS*gInv.NUM_ROWS),
	m_InvInputArea(invInput),
	m_EquipInputArea(equipInput),
	m_InvGrid(gInv),
	m_EquipGrid(gEquip),
	m_pMovingObj(NULL),
	m_pHoveredObj(NULL),
	m_pMovingObjSrc(NULL)
{
	
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: Render
//////////////////////////////////////////////////////////////////////////
void UIObjectInfoBase::Render()
{
	// render the grid images
	Globals::g_pTM->Render(m_nInventoryImage, m_ptInvPos.x, m_ptInvPos.y, DEPTH_INVENTORYBGS);
	Globals::g_pTM->Render(m_nEquippedImage, m_ptEquipPos.x, m_ptEquipPos.y, DEPTH_INVENTORYBGS);
	Globals::g_pTM->Render(m_nInfoImage, m_ptInfoPos.x, m_ptInfoPos.y, DEPTH_INVENTORYBGS);

	// available inventory first
	point startPt = m_InvGrid.TOP_LEFT_PT;	// top-left point, accounting for offset
	point actualPt = startPt.Offset(m_InvGrid.BORDER);	// where the object ends up
	unsigned currSlot = 0;	// current cell ID

	InventoryIter iter, end;
	for (iter = m_vAvailable.begin(), end = m_vAvailable.end(); iter != end; ++iter)
	{
		(*iter)->Render(actualPt);
		++currSlot;
		DetermineObjPos(actualPt, startPt, currSlot, m_InvGrid);
	}
	// TODO:: show the remainder of open slots
	for (; currSlot < m_uiInventorySlots; ++currSlot)
	{
	}

	// equipped
	startPt = m_EquipGrid.TOP_LEFT_PT;
	actualPt = startPt.Offset(m_EquipGrid.BORDER);
	for (iter = m_vEquipped.begin(), end = m_vEquipped.end(), currSlot = 0; iter != end; ++iter)
	{
		(*iter)->Render(actualPt);
		++currSlot;
		DetermineObjPos(actualPt, startPt, currSlot, m_EquipGrid);
	}
	// TODO:: show remainder of open slots
	for (; currSlot < m_uiEquipSlots; ++currSlot)
	{
	}

	// hovered object
	if (m_pHoveredObj)
	{
		if (m_bHoverInInv)
			startPt = point(m_InvGrid.TOP_LEFT_PT).Offset(m_InvGrid.BORDER);
		else
			startPt = point(m_EquipGrid.TOP_LEFT_PT).Offset(m_EquipGrid.BORDER);
		DetermineObjPos(actualPt, startPt, m_uiHoverObjID, (m_bHoverInInv ? m_InvGrid : m_EquipGrid));
		// render the hover indicator
		Globals::g_pTM->Render(Globals::g_pAssets->GetGUIasts()->HoverIndicator(), actualPt.x, actualPt.y, DEPTH_INVOBJ);

		// TODO:: display info for hovered object
		// need auto-newlining at end of word functionality:
		//Globals::g_pBitMapFont->DrawStringAutoCenter(m_pHoveredObj->GetInfo().c_str(), , DEPTH_INVOBJ);
	}

	// moving obj
	if (m_pMovingObj)
	{
		// TODO::highlight all equipable slots this object may be placed in

		// object is rendered so mouse is in the middle
		m_pMovingObj->Render(point(m_ptMousePos).Offset(-(OBJ_SIZE >> 1), -(OBJ_SIZE >> 1)), DEPTH_INVOBJDRAG);
	}
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: Update
//
// 
//////////////////////////////////////////////////////////////////////////
void UIObjectInfoBase::Update(double dTimeStep)
{
	m_Timer.Update(dTimeStep);
}

//////////////////////////////////////////////////////////////////////////
// FUNCTION: Input
//
// 
//////////////////////////////////////////////////////////////////////////
enum eInvInputCode {IIC_NONE, IIC_INV, IIC_EQUIP, };
void UIObjectInfoBase::Input(const POINT& mouse)
{
	if (!m_Timer.IsTimerRunning())
		m_Timer.StartTimer();
	m_ptMousePos = mouse;
	m_pHoveredObj = NULL;

	HandleInventoryInput(mouse);

	if (m_Timer.GetElapsed() > 0.2 && (Globals::g_pDI->KeyPressed(DIK_TAB) || Globals::g_pDI->KeyPressed(DIK_ESCAPE)))
	{
		Globals::g_bWindowOpen = false;
		m_pObject->ToggleDisplay();
		m_Timer.ResetTimer();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

bool UIObjectInfoBase::AddObj(InventoryObject* const invObj, bool intoAvailable /*= true*/)
{ 
	if (intoAvailable)
	{
		if (m_vAvailable.size() < m_uiInventorySlots)
		{
			m_vAvailable.push_back(invObj); 
			return true;
		}
	} 
	else
	{
		if (m_vEquipped.size() < m_uiEquipSlots)
		{
			m_vEquipped.push_back(invObj); 
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
//	PRIVATE  /////////
//////////////////////////////////////////////////////////////////////////
void UIObjectInfoBase::ReplaceObj(InventoryObject *const placingObj, Inventory& intoInv, int invInd)
{
	// the replaced one gets put back into the inventory if this object is going into equipped
	if (!m_bHoverInInv)
		intoInv.push_back(m_vEquipped[invInd]);
	// the object is going into the inventory, movingObj becomes replaced object
	else
		m_pMovingObj = intoInv[invInd];

	// the placed one is being hovered now:
	m_pHoveredObj = intoInv[invInd] = placingObj;
}
void UIObjectInfoBase::RemoveObj(const InventoryObject* const invObj, Inventory& inventory, bool del)
{
	InventoryIter iter, end;
	for (iter = inventory.begin(), end = inventory.end(); iter != end; ++iter)
	{
		if ((*iter) == invObj)
		{
			if (del) delete (*iter);
			inventory.erase(iter);
		}
	}
}

bool UIObjectInfoBase::CheckInvForInput(Inventory& inventory, Grid& grid, Inventory* hoveredInv, const POINT& mouse)
{
	int		currID = 0;
	point	startPos = point(grid.TOP_LEFT_PT).Offset(grid.BORDER);
	point	actualPos= startPos;
	rect	currRect(actualPos, OBJ_SIZE);

	InventoryIter iter, end;
	for (iter = inventory.begin(), end = inventory.end(); iter != end; ++iter)
	{
		if (currRect.IsPointInRect(mouse))
		{
			m_pHoveredObj = (*iter);
			m_uiHoverObjID = currID;
			hoveredInv = &m_vAvailable;
			return true;
		}
		++currID;
		DetermineObjPos(actualPos, startPos, currID, grid);
		currRect = rect(actualPos, OBJ_SIZE);
	}
	return false;
}

void UIObjectInfoBase::DetermineObjPos( point &actualPt, point &startPt, int slotID, Grid& grid )
{
	actualPt.x = startPt.x + (slotID % grid.NUM_COLS) * (OBJ_SIZE + grid.SPACE_BETWEEN);
	actualPt.y = startPt.y + (slotID / grid.NUM_COLS) * (OBJ_SIZE + grid.SPACE_BETWEEN);
}

void UIObjectInfoBase::HandleInventoryInput( const POINT& mouse )
{
	Inventory*	hoveredInv = NULL;
	eInvInputCode mouseLoc = IIC_NONE;

	rect rInv(m_InvGrid.TOP_LEFT_PT, m_InvInputArea);
	rect rEquip(m_EquipGrid.TOP_LEFT_PT, m_EquipInputArea);
	if (rInv.IsPointInRect(mouse))
	{
		mouseLoc = IIC_INV;
		// inventory (available) slots
		if (CheckInvForInput(m_vAvailable, m_InvGrid, hoveredInv, mouse))
			m_bHoverInInv = true;
	}
	else if (rEquip.IsPointInRect(mouse))
	{
		mouseLoc = IIC_EQUIP;
		// equip slots
		if (CheckInvForInput(m_vEquipped, m_InvGrid, hoveredInv, mouse))
			m_bHoverInInv = false;
	}

	if (mouseLoc != IIC_NONE && Globals::g_pDI->MouseButtonPressed(MOUSE_LEFT))
	{
		// the mouse is hovering an object and no object is being moved yet
		if (m_pHoveredObj && !m_pMovingObj)
		{
			m_pMovingObj = m_pHoveredObj;
			m_pMovingObjSrc = hoveredInv;
			// remove the hovered obj from the appropriate list
			RemoveObj(m_pHoveredObj, *hoveredInv, false);
			// determine if the slot being hovered now contains another object
			if (m_uiHoverObjID < hoveredInv->size())
				m_pHoveredObj = (*hoveredInv)[m_uiHoverObjID];
		}
		// the user is replacing an object
		else if (m_pHoveredObj && m_pMovingObj)
		{
			ReplaceObj(m_pMovingObj, *hoveredInv, m_uiHoverObjID);
			m_pMovingObjSrc = hoveredInv;
		}
		// the user is either clicking to destroy the object (IIC_NONE),
		// or is placing the object in the inv/equip area beyond 
		// currently occupied slots
		else if (!m_pHoveredObj && m_pMovingObj)
		{
			switch (mouseLoc)
			{
			case IIC_EQUIP:
				{
					if (AddObj(m_pMovingObj, false))
					{
						m_pMovingObj = NULL;
						m_pMovingObjSrc = NULL;
					}
				}break;
			case IIC_INV:
				{
					if (AddObj(m_pMovingObj, true))
					{
						m_pMovingObj = NULL;
						m_pMovingObjSrc = NULL;
					}
				}break;
			case IIC_NONE:
				{
					RemoveObj(m_pMovingObj, *m_pMovingObjSrc, true);
					m_pMovingObj = NULL;
					m_pMovingObjSrc = NULL;
				}break;
			}
		}
	}
}
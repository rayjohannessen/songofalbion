#pragma once

//////////////////////////////////////////////////////////////////////////
// Class : UIObjectInfo
//
//	This is the base for the window that displays the inventory and/or info of an object
//////////////////////////////////////////////////////////////////////////

#include "InventoryObject.h"

enum eObjectInfoType {OIT_UNIT, OIT_CITY, OIT_CASTLE, NUM_OIT};

class UICityInfo;

class UIObjectInfoBase
{
	friend class UICityInfo;
	//////////////////////////////////////////////////////////////////////////
	//	Grid - used to detect input and where to render InvetoryObjects
	//////////////////////////////////////////////////////////////////////////
	struct Grid
	{
		const int NUM_ROWS;
		const int NUM_COLS;
		const int SPACE_BETWEEN;	// the dividers between rows/cols
		const int BORDER;			// width of the outside border
		const point TOP_LEFT_PT;

		Grid(int rows, int cols, point tl, int border = 8, int space = 4) :
		NUM_ROWS(rows),
		NUM_COLS(cols),
		SPACE_BETWEEN(space),
		BORDER(border),
		TOP_LEFT_PT(tl)
		{ }
	};

	eObjectInfoType m_eType;

	bool m_bHoverInInv;	// false if hover obj is in equip grid
	InventoryObject* m_pMovingObj;	// any object being moved by the mouse
	InventoryObject* m_pHoveredObj;	// any object being hovered by the mouse
	Inventory*		 m_pMovingObjSrc;	// the inventory that the current movingObj came from
	unsigned		 m_uiHoverObjID;	// cell id of hovered object

	int m_nEquippedImage;
	int m_nInfoImage;		// for stats/description
	int m_nInventoryImage;	// TODO::may be the same for all...

	unsigned  m_uiInventorySlots;	// how many items can be placed in the inventory (vector)
	unsigned  m_uiEquipSlots;		//	..
	point	  m_ptInvPos;	// for the image positions
	point	  m_ptEquipPos;	//	..
	point	  m_ptInfoPos;	//	..
	size	  m_InvInputArea;
	size	  m_EquipInputArea;
	point	  m_ptMousePos;	// to draw moving object
	Grid	  m_InvGrid;	// for input and rendering locations of inventory objects
	Grid	  m_EquipGrid;	//	..
	Inventory m_vEquipped;
	Inventory m_vAvailable;

	// assumes the hovered object is the one being replaced
	void ReplaceObj(InventoryObject* const placingObj, Inventory& intoInv, int invInd);
	// determines top-left point of object based on currSlot and grid properties
	void DetermineObjPos( point &actualPt, point &startPt, int slotID, Grid& grid );
	// returns true if an object is hovered
	bool CheckInvForInput(Inventory& inventory, Grid& grid, Inventory* hoveredInv, const POINT& mouse);

	//////////////////////////////////////////////////////////////////////////
	//	FUNCTION: RemoveObj
	//////////////////////////////////////////////////////////////////////////
	void RemoveObj(const InventoryObject* const invObj, Inventory& inventory, bool del);

public:
	UIObjectInfoBase(eObjectInfoType type, int equipID, int infoID, int inventoryID, Grid& gInv, Grid& gEquip,
						const point& invPos, const point& equipPos, const point& infoPos, const size& invInput, const size& equipInput);
	virtual ~UIObjectInfoBase() = 0 {};

	//////////////////////////////////////////////////////////////////////////
	//	FUNCTION: Render
	//////////////////////////////////////////////////////////////////////////
	virtual void Render();

	//////////////////////////////////////////////////////////////////////////
	//	FUNCTION: Update
	//
	// 
	//////////////////////////////////////////////////////////////////////////
	virtual void Update(double dTimeStep);

	//////////////////////////////////////////////////////////////////////////
	//	FUNCTION: Input
	//
	// 
	//////////////////////////////////////////////////////////////////////////
	virtual void Input(const POINT& mouse);

	//////////////////////////////////////////////////////////////////////////
	//	AddObj - Returns true if there was enough space left in inventory
	//////////////////////////////////////////////////////////////////////////
	bool AddObj(InventoryObject* const invObj, bool intoAvailable = true);

	//////////////////////////////////////////////////////////////////////////
	// MUTATORS
	inline void AddInvSlots(int amtSlotsToAdd)		{ m_uiInventorySlots += amtSlotsToAdd; }
	inline void AddEquipSlots(int amtSlotsToAdd)	{ m_uiEquipSlots += amtSlotsToAdd;	   }
};
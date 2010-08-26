#pragma once

enum eInventoryObjType { IOT_UNIT, IOT_BUILDING, IOT_CITY, NUM_IOT};

const static int OBJ_SIZE = 64;	// objects are square

class InventoryObject
{
	bool	m_bIsHovered;	// may not need
	bool	m_bIsBeingMoved;// may not need
	eInventoryObjType m_eType;
	int		m_nImageID;
	string	m_strInfo;

public:
	InventoryObject(eInventoryObjType type, int imageID, const string& info);
	virtual ~InventoryObject() = 0 {}

	virtual void Render(const point& pos, float depth = DEPTH_INVOBJ);

	//////////////////////////////////////////////////////////////////////////
	// ACCESSORS
	bool GetIsHovered() const		{ return m_bIsHovered;		}
	bool GetIsBeingMoved() const	{ return m_bIsBeingMoved;	}
	string GetInfo()	const		{ return m_strInfo;			}

	//////////////////////////////////////////////////////////////////////////
	// MUTATORS
	void SetIsHovered(bool val)		{ m_bIsHovered = val;		}
	void SetIsBeingMoved(bool val)	{ m_bIsBeingMoved = val;	}
};

typedef vector<InventoryObject*> Inventory;
typedef vector<InventoryObject*>::const_iterator InventoryIter;
#include "stdafx.h"

#include "InventoryObject.h"
#include "Globals.h"

InventoryObject::InventoryObject(eInventoryObjType type, int imageID, const string& info) :
	m_bIsBeingMoved(false),
	m_bIsHovered(false),
	m_eType(type),
	m_nImageID(imageID),
	m_strInfo(info)
{

}

void InventoryObject::Render(const point& pos, float depth)
{
	Globals::g_pTM->Render(m_nImageID, pos.x, pos.y, depth);
}
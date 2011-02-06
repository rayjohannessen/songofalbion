#include "stdafx.h"

#include "Player.h"
#include "Globals.h"

CPlayer::CPlayer(string strPlayerName, bool bIsHuman, int FactionID) :
	m_nOreRes(100),
	m_nGoldRes(100),
	m_nWoodRes(100),
	m_nCrystalsRes(0),
	m_nNumUnits(0),
	m_nNumCities(0),
	m_nNumBuildings(0),
	m_bIsHuman(bIsHuman)
{
	m_pProfile = new Profile(strPlayerName, FactionID);
}

CPlayer::~CPlayer()
{
	RemoveAll();
	SAFE_DELETE(m_pProfile);
}
//////////////////////////////////////////////////////////////////////////

void CPlayer::RemoveAll()
{
	unsigned i;
	for (i = 0; i < m_nNumCities; ++i)
		SAFE_DELETE(m_pCities[i]);
	for (i = 0; i < m_nNumUnits; ++i)
		SAFE_DELETE(m_pUnits[i]);
	for (i = 0; i < m_nNumBuildings; ++i)
		SAFE_DELETE(m_pBuildings[i]);
	m_nNumBuildings = m_nNumCities = m_nNumUnits = 0;
}

void CPlayer::AddObject(CObject* const obj)
{
	switch (obj->GetType())
	{
	case OBJ_CITY:
		{
			if (m_nNumCities+1 >= MAX_NUM_CITIES)
			{
				// TODO:: don't allow this to happen, grey out option to add object
				// no more cities allowed
				MessageBox(NULL, "Max number of cities reached", "", MB_OK);
			} 
			else
			{
				// add the city to the array
				m_pCities[m_nNumCities++] = (CCity*)obj;
			}		
		}break;
	case OBJ_UNIT:
		{
			if (m_nNumUnits+1 >= MAX_NUM_UNITS)
			{
				// no more units allowed
				MessageBox(NULL, "Max number of cities reached", "", MB_OK);
			} 
			else
			{
				// add the unit to the array
				m_pUnits[m_nNumUnits++] = (CUnit*)obj;
			}
		}break;
	case OBJ_BUILDING:
		{
			if (m_nNumBuildings+1 >= MAX_NUM_BUILDINGS)
			{
				// no more units allowed
				MessageBox(NULL, "Max number of buildings reached", "", MB_OK);
			} 
			else
			{
				// add the unit to the array
				m_pBuildings[m_nNumBuildings++] = (CBuilding*)obj;
			}
		}break;
	}
}
void CPlayer::RemoveObject(eObjType type, unsigned index)
{
	switch(type)
	{
	case OBJ_CITY:
		{
			m_pCities[index]->SetImageID(-1);
			// shift all the cities back that are in the array
			// after the one that has been deleted
			while (++index < MAX_NUM_CITIES && m_pCities[index]->GetImageID() != -1)
			{
				m_pCities[index-1] = m_pCities[index];
			}
			--m_nNumCities;
		}break;
	case OBJ_UNIT:
		{
			m_pUnits[index]->SetImageID(-1);
			// shift all the units back that are in the array
			// after the one that has been deleted
			while (++index < m_nNumUnits && m_pUnits[index]->GetImageID() != -1)
			{
				m_pUnits[index-1] = m_pUnits[index];
			}
			--m_nNumUnits;
		}break;
	case OBJ_BUILDING:
		{
			m_pBuildings[index]->SetImageID(-1);
			// shift all the units back that are in the array
			// after the one that has been deleted
			while (++index < MAX_NUM_BUILDINGS && m_pBuildings[index]->GetImageID() != -1)
			{
				m_pBuildings[index-1] = m_pBuildings[index];
			}
			--m_nNumBuildings;
		}break;
	}
}
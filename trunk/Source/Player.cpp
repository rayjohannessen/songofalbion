#include "stdafx.h"

#include "Player.h"
#include "City.h"
#include "Unit.h"
#include "Building.h"
#include "Globals.h"

CPlayer::CPlayer()
{
	m_pCities = NULL;
	m_pUnits = NULL;
	m_pBuildings = NULL;
	m_profile = NULL;
}
CPlayer::CPlayer(string strPlayerName, bool bIsHuman, int FactionID)
{
	m_pCities	 = new CCity[MAX_NUM_CITIES];
	m_pUnits	 = new CUnit[MAX_NUM_UNITS];
	m_pBuildings = new CBuilding[MAX_NUM_BUILDINGS];

	m_nNumUnits	= m_nNumCities = m_nNumBuildings = 0;

	m_nOreRes		= 100;
	m_nGoldRes		= 100;
	m_nWoodRes		= 100;
	m_nCrystalsRes	= 0;

	m_bIsHuman = bIsHuman;
	m_profile = new Profile(strPlayerName, FactionID);
}
CPlayer::~CPlayer()
{
	RemoveAll();
	SAFE_DELETE(m_profile);
}

void CPlayer::RemoveAll()
{
	SAFE_DELETE_ARRAY(m_pCities);
	SAFE_DELETE_ARRAY(m_pUnits);
	SAFE_DELETE_ARRAY(m_pBuildings);
	m_nNumBuildings = m_nNumCities = m_nNumUnits = 0;
}

void CPlayer::AddCity(CCity& city)
{
	if (m_nNumCities+1 >= MAX_NUM_CITIES)
	{
		// no more cities allowed
		MessageBox(NULL, "Max number of cities reached", "", MB_OK);
	} 
	else
	{
		// add the city to the array
		m_pCities[m_nNumCities++] = city;
	}
}

void CPlayer::RemoveCity(int index)
{
	m_pCities[index].SetImageID(-1);
	// shift all the cities back that are in the array
	// after the one that has been deleted
	while (index++ < MAX_NUM_CITIES && m_pCities[index].GetImageID() != -1)
	{
		m_pCities[index-1] = m_pCities[index];
	}
	--m_nNumCities;
}

void CPlayer::AddUnit(CUnit& unit)
{
	if (m_nNumUnits+1 >= MAX_NUM_UNITS)
	{
		// no more units allowed
		MessageBox(NULL, "Max number of cities reached", "", MB_OK);
	} 
	else
	{
		// add the unit to the array
		m_pUnits[m_nNumUnits++] = unit;
	}
}

void CPlayer::RemoveUnit(int index)
{
	m_pUnits[index].SetImageID(-1);
	// shift all the units back that are in the array
	// after the one that has been deleted
	while (index++ < m_nNumUnits && m_pUnits[index].GetImageID() != -1)
	{
		m_pUnits[index-1] = m_pUnits[index];
	}
	--m_nNumUnits;

}

void CPlayer::AddBuilding(CBuilding& building)
{
	if (m_nNumBuildings+1 >= MAX_NUM_BUILDINGS)
	{
		// no more units allowed
		MessageBox(NULL, "Max number of buildings reached", "", MB_OK);
	} 
	else
	{
		// add the unit to the array
		m_pBuildings[m_nNumBuildings++] = building;
	}
}

void CPlayer::RemoveBldng(int index)
{
	m_pBuildings[index].SetImageID(-1);
	// shift all the units back that are in the array
	// after the one that has been deleted
	while (index++ < MAX_NUM_BUILDINGS && m_pBuildings[index].GetImageID() != -1)
	{
		m_pBuildings[index-1] = m_pBuildings[index];
	}
	--m_nNumBuildings;
}
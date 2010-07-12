#include "stdafx.h"

#include "ObjectManager.h"

#include "Object.h"
#include "City.h"
#include "Building.h"
#include "Unit.h"
#include "Player.h"
#include "Globals.h"
#include "Map.h"

ObjectManager::ObjectManager(void)
{
}

ObjectManager::~ObjectManager(void)
{
}

CObject* ObjectManager::NextObjectRemove()
{
	CObject* nextObj = NULL;
	if (m_lCurrPlayerObjects.size())
	{
		m_iCurrObjIter = m_lCurrPlayerObjects.erase(m_iCurrObjIter);
		// one has been erased, see if there's any left now
		if (m_lCurrPlayerObjects.size())
		{
			if (m_iCurrObjIter == m_iCurrObjEnd)	// if we're at the end, go back to beginning
				m_iCurrObjIter = m_lCurrPlayerObjects.begin();
			nextObj = (*m_iCurrObjIter);
		}
	}
	Globals::g_pMap->SelectObj(nextObj, true);
	return nextObj;
}

CObject* ObjectManager::NextObjectNoRemove()
{
	++m_iCurrObjIter;
	if (m_iCurrObjIter == m_iCurrObjEnd)	// if we're at the end, go back to beginning
		m_iCurrObjIter = m_lCurrPlayerObjects.begin();
	Globals::g_pMap->SelectObj((*m_iCurrObjIter), true);
	return (*m_iCurrObjIter);
}

void ObjectManager::ChangeCurrObj(CObject *currObj)
{
	m_iCurrObjIter = m_lCurrPlayerObjects.begin();
	for ( ; m_iCurrObjIter != m_iCurrObjEnd; ++m_iCurrObjIter)
		if (currObj == (*m_iCurrObjIter))
			break;	// found the unit, if this doesn't get hit...something's wrong
}

CObject* ObjectManager::StartTurn(CPlayer* currPlayer)
{
	m_lCurrPlayerObjects.clear();
	int numUnits = currPlayer->GetNumUnits();
	if (numUnits > 0)
	{
		for (int i = 0; i < numUnits; ++i)
		{
			((CUnit*)currPlayer->GetUnits())[i].ResetStamina();
			m_lCurrPlayerObjects.push_back(&currPlayer->GetUnits()[i]);
		}
		m_iCurrObjIter = m_lCurrPlayerObjects.begin();
		m_iCurrObjEnd  = m_lCurrPlayerObjects.end();
		Globals::g_pMap->SelectObj((*m_iCurrObjIter));
		return (*m_iCurrObjIter);
	}
	else
		return NULL;	// there are no units for this player
}
void ObjectManager::EndTurn()
{

}

void ObjectManager::Update(double fTimeStep, const pointf* moveAmt)
{
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);

		// Draw buildings
		CBuilding* bldng = player->GetBuildings(); int numBldngs = player->GetNumBldngs();
		for (int i = 0; i < numBldngs; ++i)
		{
			if (bldng[i].GetImageID() == -1)
				continue;
			bldng[i].Update(fTimeStep, moveAmt);
		}
		// Draw cities
		CCity* city = player->GetCities(); int numCities = player->GetNumCities();
		for (int i = 0; i < numCities; ++i)
		{
			if (city[i].GetImageID() == -1)
				continue;
			city[i].Update(fTimeStep, moveAmt);
		}
		// Draw units
		CUnit* unit = player->GetUnits(); int numUnits = player->GetNumUnits();	
		for (int i = 0; i < numUnits; ++i)
		{
			if (unit[i].GetImageID() == -1)
				continue;
			unit[i].Update(fTimeStep, moveAmt);
		}
	}
}

void ObjectManager::Render()
{
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);

		// Draw buildings
		CBuilding* bldng = player->GetBuildings(); int numBldngs = player->GetNumBldngs();
		for (int i = 0; i < numBldngs; ++i)
		{
			if (bldng[i].GetImageID() == -1)
				continue;
			bldng[i].Render(Globals::g_pMap->GetViewport());
		}
		// Draw cities
		CCity* city = player->GetCities(); int numCities = player->GetNumCities();
		for (int i = 0; i < numCities; ++i)
		{
			if (city[i].GetImageID() == -1)
				continue;
			city[i].Render(Globals::g_pMap->GetViewport());
		}
		// Draw units
		CUnit* unit = player->GetUnits(); int numUnits = player->GetNumUnits();	
		for (int i = 0; i < numUnits; ++i)
		{
			if (unit[i].GetImageID() == -1)
				continue;
			unit[i].Render(Globals::g_pMap->GetViewport());
		}
	}
}

void ObjectManager::Init()
{

}

void ObjectManager::AddObject(CPlayer* player, CObject*& obj, point& sPos)
{
	switch (obj->GetType())
	{
	case OBJ_CITY:
		{
			obj->SetScrnPos(sPos);
			player->AddCity(*((CCity*)obj));
		}
		break;
	case OBJ_UNIT:
		{
			((CUnit*)obj)->SetScrnPos(sPos);
			player->AddUnit(*((CUnit*)obj));
		}
		break;
	case OBJ_BUILDING:
		{
			obj->SetScrnPos(sPos);
			player->AddBuilding(*((CBuilding*)obj));
		}
		break;
	}
	SAFE_DELETE(obj);
}

void ObjectManager::RemoveAll()
{
	for (short i = 0; i < Globals::g_nNumPlayers; ++i)
		Globals::GetPlayers()[i]->RemoveAll();
}

void ObjectManager::RemoveObj(CPlayer* player, CObject* pObj)
{
	Globals::g_pMap->ActionIfSelected(pObj);
	// get type of hovered object
	int numObj = 0;
	switch (pObj->GetType())
	{
	case OBJ_CITY:
		{
			numObj = player->GetNumCities();
			for (int i = 0; i < numObj; ++i)
			{
				if (pObj == &player->GetCities()[i])
				{
					player->RemoveCity(i);
				}break;
			}
		}
		break;
	case OBJ_UNIT:
		{
			numObj = player->GetNumUnits();
			for (int i = 0; i < numObj; ++i)
			{
				if (pObj == &player->GetUnits()[i])
				{
					player->RemoveUnit(i);
				}break;
			}
		}
		break;
	case OBJ_BUILDING:
		{
			numObj = player->GetNumBldngs();
			for (int i = 0; i < numObj; ++i)
			{
				if (pObj == &player->GetBuildings()[i])
				{
					player->RemoveBldng(i);
				}break;
			}
		}
		break;
	case 4:
		{

		}
		break;
	}
}

void ObjectManager::Shutdown()
{
	RemoveAll();
}

CUnit* ObjectManager::IsSpaceOccupied(const point& coord, const CPlayer* const currPlayer)
{
	int numUnits = 0;
	CUnit* pUnit = NULL;
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);
		if (player == currPlayer)
			continue;
		numUnits = player->GetNumUnits();
		for (int i = 0; i < numUnits; ++i)
		{
			pUnit = &player->GetUnits()[i];
			if (pUnit->GetCoord() == coord)
				return pUnit;
		}
	}	
	return NULL;
}
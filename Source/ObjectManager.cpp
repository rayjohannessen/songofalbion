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
			if (m_iCurrObjIter == m_lCurrPlayerObjects.end())	// if we're at the end, go back to beginning
				m_iCurrObjIter = m_lCurrPlayerObjects.begin();
			nextObj = (*m_iCurrObjIter);
		}
	}
	Globals::g_pMap->SelectObj(nextObj, true);
	return nextObj;
}

CObject* ObjectManager::NextObjectNoRemove()
{
	if (m_lCurrPlayerObjects.size())	// if we're at the end, go back to beginning
	{	
		++m_iCurrObjIter;
		if (m_iCurrObjIter == m_lCurrPlayerObjects.end())
		{
			m_iCurrObjIter = m_lCurrPlayerObjects.begin();
			Globals::g_pMap->SelectObj((*m_iCurrObjIter), true);
		}
	}
	else
		return NULL;

	return (*m_iCurrObjIter);
}

void ObjectManager::ChangeCurrObj(CObject *currObj)
{
	m_iCurrObjIter = m_lCurrPlayerObjects.begin();
	for ( ; m_iCurrObjIter != m_lCurrPlayerObjects.end(); ++m_iCurrObjIter)
		if (currObj == (*m_iCurrObjIter))
			break;	// found the unit, if this doesn't get hit...something's wrong
}

CObject* ObjectManager::StartTurn(CPlayer* currPlayer)
{
	m_lCurrPlayerObjects.clear();
	unsigned numUnits = currPlayer->GetNumUnits();
	if (numUnits > 0)
	{
		for (unsigned i = 0; i < numUnits; ++i)
		{
			currPlayer->GetUnits()[i]->Reset();
			m_lCurrPlayerObjects.push_back(currPlayer->GetUnits()[i]);
		}
		m_iCurrObjIter = m_lCurrPlayerObjects.begin();
		Globals::g_pMap->SelectObj((*m_iCurrObjIter));
		Globals::g_pMap->ToggleMapFlagOn(MF_CENTER_MAP);
		return (*m_iCurrObjIter);
	}
	else
		return NULL;	// there are no units for this player
}
void ObjectManager::EndTurn()
{
	//if ( m_lCurrPlayerObjects.size() && (*m_iCurrObjIter) )
		Globals::g_pMap->OnEndTurn();//ActionIfSelected(*m_iCurrObjIter);
	Globals::GotoNextPlayer();
}

void ObjectManager::Input(const POINT& mouse)
{
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	unsigned numObjects;
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);

		// Draw buildings
		CBuilding** bldng = player->GetBuildings(); numObjects = player->GetNumBldngs();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (bldng[i]->GetImageID() == -1)
				continue;
			bldng[i]->Input(mouse);
		}
		// Draw cities
		CCity** city = player->GetCities(); numObjects = player->GetNumCities();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (city[i]->GetImageID() == -1)
				continue;
			city[i]->Input(mouse);
		}
		// Draw units
		CUnit** unit = player->GetUnits(); numObjects = player->GetNumUnits();	
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (unit[i]->GetImageID() == -1)
				continue;
			unit[i]->Input(mouse);
		}
	}
}

void ObjectManager::Update(double dTimeStep, const pointf* moveAmt)
{
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	unsigned numObjects;
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);

		// Draw buildings
		CBuilding** bldng = player->GetBuildings(); numObjects = player->GetNumBldngs();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (bldng[i]->GetImageID() == -1)
				continue;
			bldng[i]->Update(dTimeStep, moveAmt);
		}
		// Draw cities
		CCity** city = player->GetCities(); numObjects = player->GetNumCities();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (city[i]->GetImageID() == -1)
				continue;
			city[i]->Update(dTimeStep, moveAmt);
		}
		// Draw units
		CUnit** unit = player->GetUnits(); numObjects = player->GetNumUnits();	
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (unit[i]->GetImageID() == -1)
				continue;
			unit[i]->Update(dTimeStep, moveAmt);
		}
	}
}

void ObjectManager::Render()
{
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	unsigned numObjects;
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);

		// Draw buildings
		CBuilding** bldng = player->GetBuildings(); numObjects = player->GetNumBldngs();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (bldng[i]->GetImageID() == -1)
				continue;
			bldng[i]->Render(Globals::g_pMap->GetViewport());
		}
		// Draw cities
		CCity** city = player->GetCities(); numObjects = player->GetNumCities();
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (city[i]->GetImageID() == -1)
				continue;
			city[i]->Render(Globals::g_pMap->GetViewport());
		}
		// Draw units
		CUnit** unit = player->GetUnits(); numObjects = player->GetNumUnits();	
		for (unsigned i = 0; i < numObjects; ++i)
		{
			if (unit[i]->GetImageID() == -1)
				continue;
			unit[i]->Render(Globals::g_pMap->GetViewport());
		}
	}
}

void ObjectManager::Init()
{

}

void ObjectManager::AddObject(CObject* const obj, point& sPos)
{
	CPlayer* player = Globals::GetPlayerByFactionID(obj->GetFactionID());
	switch (obj->GetType())
	{
	case OBJ_CITY:
		{
			obj->SetScrnPos(sPos);
		}
		break;
	case OBJ_UNIT:
		{
			((CUnit*)obj)->SetScrnPos(sPos);
		}
		break;
	case OBJ_BUILDING:
		{
			obj->SetScrnPos(sPos);
		}
		break;
	}
	player->AddObject(obj);
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
	unsigned numObj = 0;
	switch (pObj->GetType())
	{
	case OBJ_CITY:
		{
			numObj = player->GetNumCities();
			for (unsigned i = 0; i < numObj; ++i)
			{
				if (pObj == player->GetCities()[i])
				{
					player->RemoveObject(OBJ_CITY, i);
				}break;
			}
		}
		break;
	case OBJ_UNIT:
		{
			numObj = player->GetNumUnits();
			for (unsigned i = 0; i < numObj; ++i)
			{
				if (pObj == player->GetUnits()[i])
				{
					CUnit* unit = (CUnit*)pObj;
					if (unit->GetNeighbor())
						unit->GetNeighbor()->CenterUnit();
					player->RemoveObject(OBJ_UNIT, i);
				}break;
			}
		}
		break;
	case OBJ_BUILDING:
		{
			numObj = player->GetNumBldngs();
			for (unsigned i = 0; i < numObj; ++i)
			{
				if (pObj == player->GetBuildings()[i])
				{
					player->RemoveObject(OBJ_BUILDING, i);
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

CUnit* ObjectManager::IsSpaceOccupied(const point& coord)
{
	int numUnits = 0;
	CUnit* pUnit = NULL;
	m_iPlayersIter = Globals::GetPlayers().begin();
	m_iPlayersIterEnd = Globals::GetPlayers().end();
	for ( ; m_iPlayersIter != m_iPlayersIterEnd; ++m_iPlayersIter)
	{
		CPlayer* player = (*m_iPlayersIter);
		if (player == Globals::GetCurrPlayer())
			continue;
		numUnits = player->GetNumUnits();
		for (int i = 0; i < numUnits; ++i)
		{
			pUnit = player->GetUnits()[i];
			if (pUnit->GetCoord() == coord)
				return pUnit;
		}
	}	
	return NULL;
}
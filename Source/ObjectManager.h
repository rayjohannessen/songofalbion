#pragma once

class CObject;
class CPlayer;
class CUnit;

class ObjectManager
{
	typedef vector<CPlayer*>::iterator PlayersIter;
	typedef list<CObject*> Objects;
	typedef list<CObject*>::iterator ObjectsIter;

	PlayersIter m_iPlayersIter, m_iPlayersIterEnd;
	ObjectsIter m_iCurrObjIter;

	Objects m_lCurrPlayerObjects;

public:
	ObjectManager(void);
	~ObjectManager(void);

	void Init();
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// add/remove object(s)
	void AddObject(CObject*& obj, point& sPos);
	void RemoveAll();
	void RemoveObj(CPlayer* player, CObject* obj);
	//////////////////////////////////////////////////////////////////////////

	void Input(const POINT& mouse);
	void Update(double fElapsed, const pointf* moveAmt = NULL);
	void Render();

	CUnit* IsSpaceOccupied(const point& coord);

	//////////////////////////////////////////////////////////////////////////
	// turn-related
	// used when a unit is done for the turn, if so remove it from the list. Return next unit in the list
	CObject* NextObjectRemove( );
	CObject* NextObjectNoRemove( );
	// used if the player selects a random unit during their turn
	void ChangeCurrObj(CObject* currObj);
	CObject* StartTurn(CPlayer* currPlayer);
	void EndTurn();
	//////////////////////////////////////////////////////////////////////////
};

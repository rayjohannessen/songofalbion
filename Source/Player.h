#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NUM_CITIES 50
#define MAX_NUM_UNITS  100
#define MAX_NUM_BUILDINGS 50

#include "City.h"
#include "Unit.h"
#include "Building.h"

struct Profile 
{
	string name;
	short nFactionID;
	Profile(string strName, short factionID)
	{
		name = strName;
		nFactionID = factionID;
	}
};

class CPlayer
{
	enum {FAC_LOGRESS, FAC_PRYDAIN, NUM_FACTIONS};

	// the player's info
	Profile* m_pProfile;
	bool	 m_bIsHuman;

	// contains all the player's cities, can hold up to [MAX_NUM_CITIES]
	CCity* m_pCities[MAX_NUM_CITIES];
	unsigned m_nNumCities;
	// contains all the player's units, can hold up to [MAX_NUM_UNITS]
	CUnit* m_pUnits[MAX_NUM_UNITS];
	unsigned m_nNumUnits;

	CBuilding* m_pBuildings[MAX_NUM_BUILDINGS];
	unsigned m_nNumBuildings;

	// resources
	unsigned m_nWoodRes;
	unsigned m_nOreRes;
	unsigned m_nCrystalsRes;
	unsigned m_nGoldRes;

	CPlayer(const CPlayer&);
public:

	~CPlayer();
	CPlayer(string strPlayerName, bool bIsHuman, int FactionID = FAC_LOGRESS);

	void RemoveAll();
	void RemoveObject(eObjType type, unsigned index);
	void AddObject(CObject* const obj);

	//////////////////////////////////////////////////////////////////////////
	// accessors / mutators
	inline CCity** GetCities()			{return m_pCities;		}
	inline unsigned GetNumCities()		{return m_nNumCities;	}
	inline CUnit** GetUnits()			{return m_pUnits;		}
	inline unsigned GetNumUnits()		{return m_nNumUnits;	}
	inline CBuilding** GetBuildings()	{return m_pBuildings;	}
	inline unsigned GetNumBldngs()		{return m_nNumBuildings;}

	inline Profile* GetProfile()		{return m_pProfile;		}

	// resources
	inline unsigned GetGoldRes()		{return m_nGoldRes;		}
	inline unsigned GetWoodRes()		{return m_nWoodRes;		}
	inline unsigned GetOreRes()			{return m_nOreRes;		}
	inline unsigned GetCrystalRes()		{return m_nCrystalsRes;	}


};

#endif
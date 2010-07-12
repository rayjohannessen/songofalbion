#ifndef PLAYER_H
#define PLAYER_H

#define MAX_NUM_CITIES 50
#define MAX_NUM_UNITS  100
#define MAX_NUM_BUILDINGS 50

// #include <string>
// using std::string;

class CCity;
class CUnit;
class CBuilding;

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
	Profile* m_profile;
	bool	 m_bIsHuman;

	// contains all the player's cities, can hold up to [MAX_NUM_CITIES]
	CCity* m_pCities;
	int m_nNumCities;
	// contains all the player's units, can hold up to [MAX_NUM_UNITS]
	CUnit* m_pUnits;
	int m_nNumUnits;

	CBuilding* m_pBuildings;
	int m_nNumBuildings;

	// resources
	int m_nWoodRes;
	int m_nOreRes;
	int m_nCrystalsRes;
	int m_nGoldRes;

	CPlayer(const CPlayer&);
public:
	CPlayer();
	~CPlayer();
	CPlayer(string strPlayerName, bool bIsHuman, int FactionID = FAC_LOGRESS);

	void RemoveAll();
	void AddCity(CCity& city);
	void RemoveCity(int index);
	void AddUnit(CUnit& unit);
	void RemoveUnit(int index);
	void AddBuilding(CBuilding& building);
	void RemoveBldng(int index);

	//////////////////////////////////////////////////////////////////////////
	// accessors / mutators
	inline CCity* GetCities()			{return m_pCities;		}
	inline int GetNumCities()			{return m_nNumCities;	}
	inline CUnit* GetUnits()			{return m_pUnits;		}
	inline int GetNumUnits()			{return m_nNumUnits;	}
	inline CBuilding* GetBuildings()	{return m_pBuildings;	}
	inline int GetNumBldngs()			{return m_nNumBuildings;}

	inline Profile* GetProfile()		{return m_profile;		}

	// resources
	inline int GetGoldRes()				{return m_nGoldRes;		}
	inline int GetWoodRes()				{return m_nWoodRes;		}
	inline int GetOreRes()				{return m_nOreRes;		}
	inline int GetCrystalRes()			{return m_nCrystalsRes;	}


};

#endif
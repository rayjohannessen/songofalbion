#ifndef BLDNG_H
#define BLDNG_H

#include "Object.h"
using namespace BuildingDefines;
class CUnit;

class CBuilding : public CObject
{
	int			m_nBuildingType;	// castle, tower...
	int			m_nMaxHP;
	int			m_nHitPts;
	unsigned	m_uiGarrisonCapacity;	// how many troops can it hold?	

	Garrison    m_vGarrison;	// the units that are inside the building

public:
	~CBuilding() {}
	CBuilding();
	CBuilding(eBuildingType buildingType, int type, point coord, point sPos, 
				string name, const char* faction = "Llwyddi", int factionID = 0);

	void Update(double dTimeStep, const pointf* moveAmt = NULL);
	void Render(const rect& viewPort);

	//////////////////////////////////////////////////////////////////////////
	// ACCESSORS
	inline const Garrison* GetGarrison() const { if (m_vGarrison.size()) return &m_vGarrison; else return NULL; }

	//////////////////////////////////////////////////////////////////////////
	// MUTATORS
	inline bool GarrisonUnit(CUnit* const _unit) 
	{ 
		if (m_vGarrison.size() < m_uiGarrisonCapacity)
		{
			m_vGarrison.push_back(_unit); 
			return true;
		}
		return false;
	}

	void SetPostBaseCTORVars();
};

#endif
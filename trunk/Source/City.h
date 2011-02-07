#ifndef CITY_H
#define CITY_H

#include "Object.h"

const int CITY_HEIGHT = 64;
const int CITY_WIDTH = 128;

class UICityInfo;
class CUnit;

class CCity : public CObject
{
	int		m_nCityType;// oriental, celtic, hut, etc..
	int		m_nSize;	// how big in population is the city (not 0-based)
	unsigned m_uiGarrisonCapacity;

	UICityInfo* m_pCityInfo;
	Garrison    m_vGarrison;	// the units that are inside the city

public:
	~CCity();
	CCity();
	CCity(int type, int cityType, int size, point coord, point sPos, 
			string name, const char* faction = "Llwyddi", int factionID = 0);

	void Update(double dTimeStep, const pointf* moveAmt = NULL);
	void Render(const rect& viewPort);
	void Input(const POINT& mouse);

	//////////////////////////////////////////////////////////////////////////
	// accessors
	inline int GetCityType()	const	{return m_nCityType;}
	inline int GetCitySize()	const	{return m_nSize;}
	inline const Garrison* GetGarrison() const { if (m_vGarrison.size()) return &m_vGarrison; else return NULL;}

	//////////////////////////////////////////////////////////////////////////
	// mutators
	void GarrisonUnit(CUnit* const _unit) { m_vGarrison.push_back(_unit); }
};
#endif
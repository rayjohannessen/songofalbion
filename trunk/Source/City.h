#ifndef CITY_H
#define CITY_H

#include "Object.h"

const int CITY_HEIGHT = 64;
const int CITY_WIDTH = 128;

class CCity : public CObject
{
	int		m_nCityType;// oriental, celtic, hut, etc..
	int		m_nSize;	// how big in population is the city

public:
	~CCity() {}
	CCity();
	CCity(int type, int cityType, int size, point coord, point sPos, 
			string name, const char* faction = "Llwyddi", int factionID = 0);

	void Update(double dTimeStep, const pointf* moveAmt = NULL);
	void Render(const rect& viewPort);

	//////////////////////////////////////////////////////////////////////////
	// accessors
	inline int GetCityType()	const	{return m_nCityType;}
	inline int GetCitySize()	const	{return m_nSize;}

	//////////////////////////////////////////////////////////////////////////
	// mutators
};
#endif
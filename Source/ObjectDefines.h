#pragma once

#include "ButtonDefines.h"

//////////////////////////////////////////////////////////////////////////
///////////// UNIT DEFINES	//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace UnitDefines
{
	#define MOVE_SPEED 150.0f;	// TODO:: this will change for each unit
	enum eUnitType {UT_GROUND, UT_AIR, UT_SEA, };
	
	// units
	enum eUnits
	{
		UNIT_UMKNIGHT, 
		UNIT_END
	};
	
	const static string gUnitNames[] = 
	{
		"UMKnight"
	};
}

//////////////////////////////////////////////////////////////////////////
///////////// CITY DEFINES	//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace CityDefines
{

}

//////////////////////////////////////////////////////////////////////////
///////////// BUILDING DEFINES	//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace BuildingDefines
{

}

//////////////////////////////////////////////////////////////////////////
// Common to all objects
//////////////////////////////////////////////////////////////////////////

using namespace UnitDefines;
namespace ObjectDefines
{
	typedef string ObjName;
	typedef vector<pair<eButtonSlot, eButtonName>> ButtonNamesVec;
	typedef pair<eButtonSlot, eButtonName> ButtonPair;
	typedef map<ObjName, vector<pair<eButtonSlot, eButtonName>>> ObjectButtonsMap;
	typedef map<ObjName, vector<pair<eButtonSlot, eButtonName>>>::iterator ObjButtonsIter;

	static ObjectButtonsMap*		g_pObjButtonsMap = NULL;
	static void SetObjectButtonMatches()
	{
		g_pObjButtonsMap = new ObjectButtonsMap;

		eUnits currUnit = UNIT_UMKNIGHT;	// knight has all buttons (so far)
		eButtonName i = (eButtonName)1;
		eButtonSlot s = (eButtonSlot)1;
		for ( ; i < BN_NUM_BUTTONS; i = eButtonName(i + 1), s = eButtonSlot(s + 1))
		{
			if (i == BN_SKILLS)
			{
				s = eButtonSlot(s - 1);
				continue; // this unit uses BN_NONCOMBAT_SKILLS instead
			}
			(*g_pObjButtonsMap)[gUnitNames[currUnit]].push_back( ButtonPair(s, i) );
		}
	}
}
#pragma once

#include "ButtonDefines.h"

//////////////////////////////////////////////////////////////////////////
///////////// UNIT DEFINES	//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace UnitDefines
{
	#define MOVE_SPEED 150.0f;	// TODO:: this will change for each unit
	#define BATTLE_OS 25.0f;	// how much units offset when facing one another on a single tile in battle
	enum eUnitType {UT_GROUND, UT_AIR, UT_SEA, NUM_UT};
	
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
	enum eCityType { CT_VILLAGE, CT_CAERN, CT_TOWNSHIP, CT_FORTRESS, NUM_CT};

	const static string gCityTypeNames[] =
	{
		"Village",
		"Caern",
		"Township",
		"Fortress"
	};
}

//////////////////////////////////////////////////////////////////////////
///////////// BUILDING DEFINES	//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
namespace BuildingDefines
{
	enum eBuildingType {BT_CASTLE, BT_TOWER, NUM_BT, };

	const static string gBuildingNames[] =
	{
		"Castle",
		"Tower"
	};
}

//////////////////////////////////////////////////////////////////////////
// Common to all objects
//////////////////////////////////////////////////////////////////////////

using namespace UnitDefines;
using namespace CityDefines;
using namespace BuildingDefines;

namespace ObjectDefines
{
	enum eObjType	{ OBJ_CITY, OBJ_UNIT, OBJ_BUILDING, NUM_OBJECTS };

	typedef string ObjName;
	typedef vector<pair<eButtonSlot, eButtonName>> ButtonNamesVec;
	typedef pair<eButtonSlot, eButtonName> ButtonPair;
	typedef map<ObjName, vector<pair<eButtonSlot, eButtonName>>> ObjectButtonsMap;
	typedef map<ObjName, vector<pair<eButtonSlot, eButtonName>>>::iterator ObjButtonsIter;

	static ObjectButtonsMap*		g_pObjButtonsMap = NULL;

	// sets every selectable object's buttons which are to be displayed on the HUD when selected
	static void SetObjectButtonMatches()
	{
		g_pObjButtonsMap = new ObjectButtonsMap;

		eObjType objectsInd = eObjType(0);

		eButtonName i;	// NOT including menu and end turn buttons
		eButtonSlot s;

		// go through all object types
		for ( ; objectsInd < NUM_OBJECTS; objectsInd = eObjType(objectsInd + 1))
		{
			switch (objectsInd)
			{
			case OBJ_UNIT:
				{
					eUnits currUnit = UNIT_UMKNIGHT;	// knight has all buttons (so far)

					// go through the buttons
					for (i = BN_COMBAT_SKILLS, s = BL_SLOT_1_1; i < BN_NUM_BUTTONS; i = eButtonName(i + 1), s = eButtonSlot(s + 1))
					{
						if (i == BN_SKILLS)
						{
							s = eButtonSlot(s - 1);
							continue; // this unit uses BN_NONCOMBAT_SKILLS instead
						}
						(*g_pObjButtonsMap)[gUnitNames[currUnit]].push_back( ButtonPair(s, i) );
					}
				}break;
			case OBJ_CITY:
				{
					// all city types have these buttons 
					for (eCityType cityType = (eCityType)0; cityType < NUM_CT; cityType = eCityType(cityType + 1))
					{
						(*g_pObjButtonsMap)[gCityTypeNames[cityType]].push_back(ButtonPair(BL_SLOT_1_1, BN_COMBAT_SKILLS));
						(*g_pObjButtonsMap)[gCityTypeNames[cityType]].push_back(ButtonPair(BL_SLOT_1_2, BN_NONCOMBAT_SKILLS));
						(*g_pObjButtonsMap)[gCityTypeNames[cityType]].push_back(ButtonPair(BL_SLOT_1_3, BN_GEAR));
					}

					// go through the buttons
// 					for (i = BN_COMBAT_SKILLS, s = BL_SLOT_1_1; i < BN_NUM_BUTTONS; i = eButtonName(i + 1), s = eButtonSlot(s + 1))
// 					{
// 						if (i == BN_SKILLS)
// 						{
// 							s = eButtonSlot(s - 1);
// 							continue; // this unit uses BN_NONCOMBAT_SKILLS instead
// 						}
// 						(*g_pObjButtonsMap)[gCityTypeNames[cityType]].push_back( ButtonPair(s, i) );
// 					}
				}break;
			case OBJ_BUILDING:
				{
					// all city types have these buttons 
					for (eBuildingType eBldngType = (eBuildingType)0; eBldngType < NUM_BT; eBldngType = eBuildingType(eBldngType + 1))
					{
						(*g_pObjButtonsMap)[gBuildingNames[eBldngType]].push_back(ButtonPair(BL_SLOT_1_1, BN_COMBAT_SKILLS));
						(*g_pObjButtonsMap)[gBuildingNames[eBldngType]].push_back(ButtonPair(BL_SLOT_1_2, BN_NONCOMBAT_SKILLS));
						(*g_pObjButtonsMap)[gBuildingNames[eBldngType]].push_back(ButtonPair(BL_SLOT_1_3, BN_GEAR));
					}

				}break;
			}
		}
	}
}
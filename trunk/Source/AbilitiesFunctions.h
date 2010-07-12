#pragma once

#include "AbilityObjectBase.h"
class CObject;

//////////////////////////////////////////////////////////////////////////
//		COMBAT FUNCTIONS
//////////////////////////////////////////////////////////////////////////
//	Attack functions handle what results occur during the update of the
//	corresponding ability (skill, w/e), e.g., an attack skill would use
//	one of these functions to calculate how much damage is done based on
//	its specific formula
//////////////////////////////////////////////////////////////////////////


namespace CombatFunctions
{
	// functions RETURN true if damage has been dealt to the target (i.e. target was indeed hit..play hit anim), false otherwise


	//////////////////////////////////////////////////////////////////////////
	//	Base attack functions
	//////////////////////////////////////////////////////////////////////////

	// Ground units
	void GroundBaseAttack(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);

	// Air units
	void AirBaseAttack(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);
	
	// Sea units
	void SeaBaseAttack(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);

	//////////////////////////////////////////////////////////////////////////
	//	Ground
	//////////////////////////////////////////////////////////////////////////
	void Charge(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);
	void Flank(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);

	//////////////////////////////////////////////////////////////////////////
	//	Air
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Sea
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// FORMULAS - returns the object that has died (if any), otherwise NULL
	//////////////////////////////////////////////////////////////////////////
	void formula_BasicAttacker(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);
	void formula_BasicDefender(CObject* targetObj, CObject* thisPtr, AbilityReturn& abilRet);

	inline void clamp(float& val, float low = 0.0f, float high = 1.0f);
	inline void clamp_low(float& val, float low = 0.0f);
	inline void clamp_high(float& val, float high = 1.0f);

	static const float BaseAtkBonus = 1.75f;
	static const float BaseDefPenalty = 0.8f;
}
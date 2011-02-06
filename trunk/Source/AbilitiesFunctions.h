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
	//////////////////////////////////////////////////////////////////////////
	//	Base attack functions
	//////////////////////////////////////////////////////////////////////////

	// Ground units
	void GroundBaseAttack(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);

	// Air units
	void AirBaseAttack(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);
	
	// Sea units
	void SeaBaseAttack(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);

	//////////////////////////////////////////////////////////////////////////
	//	Ground
	//////////////////////////////////////////////////////////////////////////
	void Charge(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);
	void Flank(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);
	void Fortify(CObject* defender, CObject* attacker, AbilityReturnBase* abilRet);

	//////////////////////////////////////////////////////////////////////////
	//	Air
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Sea
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// FORMULAS - returns the results in CombatAbilityReturn struct
	//////////////////////////////////////////////////////////////////////////
	void formula_BasicAttacker(CObject* targetObj, CObject* thisPtr, CombatAbilityReturn& abilRet);
	void formula_BasicDefender(CObject* targetObj, CObject* thisPtr, CombatAbilityReturn& abilRet);

	// helpers
	inline void clamp(float& val, float low = 0.0f, float high = 1.0f);
	inline void clamp_low(float& val, float low = 0.0f);
	inline void clamp_high(float& val, float high = 1.0f);

	// battle constants
	static const float BaseAtkBonus = 1.75f;
	static const float BaseDefPenalty = 0.8f;
}

namespace NonCombatFunctions
{
	void HealUnit(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);

	void PromoteUnit(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* abilRet);
}
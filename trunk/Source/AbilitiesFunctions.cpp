#include "stdafx.h"

#include "AbilitiesFunctions.h"
#include "Unit.h"
#include "Object.h"
#include "Animation.h"
#include "ObjectManager.h"
#include "Globals.h"

namespace CombatFunctions
{
	//////////////////////////////////////////////////////////////////////////
	// Base attack functions
	//////////////////////////////////////////////////////////////////////////

	// Ground units
	void GroundBaseAttack(CObject* defender, CObject* attacker, AbilityReturn& abilRet)
	{
		abilRet.Clear();
		abilRet.ApplyDamages = ((CUnit*)attacker)->GetCurrAnim().IsAtMiddlePoint();
		if (abilRet.ApplyDamages)
		{
			formula_BasicAttacker(defender, attacker, abilRet);
		}

	}

	// Air units
	void AirBaseAttack(CObject* defender, CObject* attacker, AbilityReturn& abilRet)
	{
		abilRet.Clear();
	}

	// Sea units
	void SeaBaseAttack(CObject* defender, CObject* attacker, AbilityReturn& abilRet)
	{
		abilRet.Clear();
	}

	//////////////////////////////////////////////////////////////////////////
	//	Ground
	//////////////////////////////////////////////////////////////////////////
	void Charge(CObject* defender, CObject* attacker, AbilityReturn& abilRet)
	{
		abilRet.Clear();
		abilRet.ApplyDamages = ((CUnit*)attacker)->GetCurrAnim().IsAtMiddlePoint();
		if (abilRet.ApplyDamages)
		{
			formula_BasicAttacker(defender, attacker, abilRet);
		}
	}
	void Flank(CObject* defender, CObject* attacker, AbilityReturn& abilRet)
	{
		abilRet.Clear();
		abilRet.ApplyDamages = ((CUnit*)attacker)->GetCurrAnim().IsAtMiddlePoint();
		if (abilRet.ApplyDamages)
		{
			formula_BasicAttacker(defender, attacker, abilRet);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//	Air
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Sea
	//////////////////////////////////////////////////////////////////////////


		
	//////////////////////////////////////////////////////////////////////////
	// FORMULAS
	//////////////////////////////////////////////////////////////////////////

	// TODO:: take into account current target's current defensive ability ... if applicable,
	// also -- gauge the tendency of the random range by the experience of the unit (higher exp tends toward higher end more often),
	// could replace .1f with an experience-related formula
	// allow for critical hits (call it something else in-game)
	// TODO::add terrain into one of these formulas
	void formula_BasicAttacker(CObject* _defender, CObject* _attacker, AbilityReturn& abilRet)
	{
		const static float rangeModifier = .2f;
		float dmgToTarget = 0.0f;
		CUnit* defender, *attacker;
		defender	= (CUnit*)_defender;
		attacker	= (CUnit*)_attacker;

		CAbilityProperties* attackerProps = &((CAbilityObjectBase*)attacker->GetCurrAttackAbility())->GetProps();
		CAbilityProperties* defenderProps = &((CAbilityObjectBase*)defender->GetCurrDefenseAbility())->GetProps(); 

		float attackerAttk  = (float)attacker->GetAttk();
		float defenderDef	= (float)defender->GetAttk();
		// the less health the unit has, the worse they do in battle
		float attackerHPmod	= (float)attacker->GetVitality() / (float)attacker->GetMaxVit() + 0.3f;
		attackerHPmod += (attackerHPmod * 0.5f);
		clamp_high(attackerHPmod);
		float defenderHPmod	= (float)defender->GetVitality() / (float)defender->GetMaxVit() + 0.3f;
		defenderHPmod += (defenderHPmod * 0.5f);
		clamp_high(defenderHPmod);

		// attack - attacker
		//												low side								
		dmgToTarget		= (RAND_FLOAT((attackerAttk * BaseAtkBonus * attackerProps->AttkDamageMod) - (attackerAttk * attackerProps->AttkDamageMod * rangeModifier),
										 (attackerAttk * BaseAtkBonus * attackerProps->AttkDamageMod) + (attackerAttk * attackerProps->AttkDamageMod * rangeModifier) ) 
										 * attackerHPmod);
		//												high side
		//TODO:: offset attack by defense
		dmgToTarget    -= (RAND_FLOAT((defenderDef * BaseDefPenalty) - (defenderDef * defenderProps->DefDamageMod * rangeModifier),
										 (defenderDef * BaseDefPenalty) + (defenderDef * defenderProps->DefDamageMod * rangeModifier) ) 
										 * defenderHPmod);
		clamp_low(dmgToTarget);

		if (dmgToTarget > 0)
		{
			abilRet.HitObjs.push_back(defender);
			defender->AddToVitality(-(int)dmgToTarget);
		}

		if (defender->GetVitality() <= 0)
			abilRet.ObjsToEliminate.push_back(defender);
	}

	void formula_BasicDefender(CObject* _defender, CObject* _attacker, AbilityReturn& abilRet)
	{
// 		const static float rangeModifier = .2f;
// 		float dmgToAttacker = 0.0f;
// 		CUnit* defender, *attacker;
// 		defender	= (CUnit*)_defender;
// 		attacker	= (CUnit*)_attacker;
// 
// 		float defenderAttk	= (float)defender->GetAttk();
// 		float attackerDef	= (float)attacker->GetDef();
// 
// 		CAbilityProperties* attackerProps = &attacker->GetCurrAttackAbility()->GetProps();
// 		CAbilityProperties* defenderProps = &defender->GetCurrDefenseAbility()->GetProps(); 
// 
// 		// the less health the unit has, the worse they do in battle
// 		float attackerHPmod	= (float)attacker->GetVitality() / (float)attacker->GetMaxVit() + 0.2f;
// 		clamp_high(attackerHPmod);
// 		float defenderHPmod	= (float)defender->GetVitality() / (float)defender->GetMaxVit() + 0.2f;
// 		clamp_high(defenderHPmod);
// 
// 		// attack - defender
// 		//												low side								
// 		dmgToAttacker	= (RAND_FLOAT((defenderAttk * BaseAtkBonus) - (defenderAttk * defenderProps->AttkDamageMod * rangeModifier),
// 			(defenderAttk * BaseAtkBonus) + (defenderAttk * defenderProps->AttkDamageMod * rangeModifier) ) 
// 			* defenderHPmod);
// 		//												high side
// 		//TODO:: offset attack by defense
// 		dmgToAttacker  -= (RAND_FLOAT((attackerDef * BaseDefPenalty) - (attackerDef * attackerProps->DefDamageMod * rangeModifier),
// 			(attackerDef * BaseDefPenalty) + (attackerDef * attackerProps->DefDamageMod * rangeModifier) ) 
// 			* attackerHPmod);
// 
// 		clamp_low(dmgToAttacker);
// 
// 		if (dmgToAttacker > 0)
// 		{
// 			abilRet.HitObjs.push_back(attacker);
// 			attacker->AddToVitality(-(int)dmgToAttacker);
// 		}
// 		if (attacker->GetVitality() <= 0)
// 			abilRet.ObjsToEliminate.push_back(attacker);
	}
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//	Helper functions
	//////////////////////////////////////////////////////////////////////////
#pragma region HELPER_FUNCTIONS
	void clamp(float& val, float low, float high)
	{
		if (val < low)
			val = low;
		else if (val > high)
			val = high;
	}
	void clamp_low(float& val, float low /* = 0.0f */)
	{
		if (val < low)
			val = low;
	}
	void clamp_high(float& val, float high /* = 1.0f */)
	{
		if (val > high)
			val = high;
	}
#pragma endregion
}

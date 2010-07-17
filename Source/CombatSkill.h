#pragma once

#include "AbilityObjectBase.h"

struct CCombatAbilProperties
{
	int		AttackStam;		// required stamina to use for attack
	int		DefStam;		// required stamina to use for defense
	int		FreeCounters;	// how many free counter attacks this ability has (if any)
	int		CurrFreeCounters;

	CCombatAbilProperties(bool bDefault = true) :
		AttackStam(0),
		DefStam(0),
		FreeCounters(0),
		CurrFreeCounters(0)
	{
		if (bDefault)
			Default();
	}
	CCombatAbilProperties(int attStam, int defStam, int freeCounterAtk) :
		AttackStam(attStam),
		DefStam(defStam),
		FreeCounters(freeCounterAtk),
		CurrFreeCounters(freeCounterAtk)
	{

	}

	void Default()
	{
		AttackStam	= 1;
		DefStam		= 1;
		FreeCounters = CurrFreeCounters = 0;
	}
};

class CCombatSkill : public CAbilityObjectBase
{
	CCombatAbilProperties m_CombatProps;

public:
	CCombatSkill() : CAbilityObjectBase() {}
	CCombatSkill(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj, CAbilityProperties& props, CCombatAbilProperties& combatProps);
	~CCombatSkill();
	void Update(double fTimeStep, AbilityReturn& abilRet);
	void Render();

	void ResetResultsApplied();
	inline void ResetFreeCounter()							{ m_CombatProps.CurrFreeCounters = m_CombatProps.FreeCounters;	}
	inline CCombatAbilProperties& GetCombatProps()			{ return m_CombatProps; }
	inline int	GetStaminaRequired(bool bGetAttackStam)		{ if (bGetAttackStam) return m_CombatProps.AttackStam; else return m_CombatProps.DefStam; }
};


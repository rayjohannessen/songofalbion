#pragma once

#include "AbilityObjectBase.h"

struct CombatSkillProperties
{
	int		AttackStam;		// required stamina to use for attack
	int		DefStam;		// required stamina to use for defense
	int		FreeCounters;	// how many free counter attacks this ability has (if any)
	int		CurrFreeCounters;

	float	AttkDamageMod;	// affects the owner's attack stat when attacking
	float	DefDamageMod;	// affects the owner's defense stat when defending

	CombatSkillProperties(bool bDefault = true) :
		AttackStam(0),
		DefStam(0),
		FreeCounters(0),
		CurrFreeCounters(0),
		AttkDamageMod(0.0f),
		DefDamageMod(0.0f)
	{
		if (bDefault)
			Default();
	}
	CombatSkillProperties(int attStam, int defStam, int freeCounterAtk, float atkMod, float defMod) :
		AttackStam(attStam),
		DefStam(defStam),
		FreeCounters(freeCounterAtk),
		CurrFreeCounters(freeCounterAtk),
		AttkDamageMod(atkMod),
		DefDamageMod(defMod)
	{

	}

	void Default()
	{
		AttackStam	= 1;
		DefStam		= 1;
		FreeCounters = CurrFreeCounters = 0;

		AttkDamageMod = 1.065f;
		DefDamageMod  = 1.05f;
	}
};

class CCombatSkill : public CAbilityObjectBase
{
	CombatSkillProperties m_CombatProps;

public:
	CCombatSkill() : CAbilityObjectBase() {}
	CCombatSkill(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj, CombatSkillProperties& combatProps);
	virtual ~CCombatSkill();
	void Update(double dTimeStep, AbilityReturnBase* const);
	void Render();

	void ResetResultsApplied();
	inline void ResetFreeCounter()							{ m_CombatProps.CurrFreeCounters = m_CombatProps.FreeCounters;	}
	inline CombatSkillProperties* GetProps()				{ return &m_CombatProps; }
	inline int	GetStaminaRequired(bool bGetAttackStam)		{ if (bGetAttackStam) return m_CombatProps.AttackStam; else return m_CombatProps.DefStam; }
};


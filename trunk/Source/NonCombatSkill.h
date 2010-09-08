#pragma once

#include "AbilityObjectBase.h"

struct NonCombatSkillProperties
{

	NonCombatSkillProperties(bool bDefault = true) 
	{
		if (bDefault)
			Default();
	}
	NonCombatSkillProperties(int val1, int val2) 
	{

	}

	void Default()
	{
	}
};

////////////////////////// CNonCombatSkill ////////////////////////////////////////
//	This class would render any effects related to the skill, call the
//	appropriate attack function, and update any specific variables to this
//	skill necessary...also, this would be used to render an icon in the HUD (if that is how it will be done)
//
// NOTE: "non-combat skills" are the same as "skills"...the only difference is
//		 in the semantics, skills are used for non-combat units' abilities 
//		 displaying "Skills" on the GUI...as opposed to a combat unit with
//		 non-combat skills, which are displayed as "non-combat skills" (because 
//		 the combat units also have "combat skills")
//////////////////////////////////////////////////////////////////////////

class CNonCombatSkill : public CAbilityObjectBase
{
	NonCombatSkillProperties m_Properties;

public:
	CNonCombatSkill() : CAbilityObjectBase() {}
	CNonCombatSkill(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj, const NonCombatSkillProperties& props);
	virtual ~CNonCombatSkill();
	void Update(double dTimeStep, AbilityReturnBase* const);
	void Render();

	NonCombatSkillProperties* GetProps()	{ return &m_Properties; }
};

typedef CNonCombatSkill CSkill;	
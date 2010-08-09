#pragma once

#include "AbilityObjectBase.h"

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

public:
	CNonCombatSkill() : CAbilityObjectBase() {}
	CNonCombatSkill(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj, CAbilityProperties& props);
	~CNonCombatSkill();
	void Update(double dTimeStep, AbilityReturn& abilRetu);
	void Render();
};

typedef CNonCombatSkill CSkill;	
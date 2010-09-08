#include "stdafx.h"

#include "CombatSkill.h"
#include "Globals.h"

CCombatSkill::CCombatSkill(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj, CombatSkillProperties& combatProps) : 
	CAbilityObjectBase(type, pos, name, abilityFunc, qbObj),
	m_CombatProps(combatProps)
{
}

CCombatSkill::~CCombatSkill()
{
	// base class deletes m_pQBObj
}

void CCombatSkill::Update(double dTimeStep, AbilityReturnBase* const abilRet)
{
	if(!m_bResultsApplied)
	{
		m_fpAbilityFunc(m_pTarget, m_pOwner, abilRet);
		m_bResultsApplied = ((CombatAbilityReturn*)abilRet)->ApplyDamages;
		SimulatedVFX.StartTimer(2.5f); // if the ability has been performed, simulate some visual effects happening...wait a bit
	}
	else	
	{
		if (SimulatedVFX.Update(dTimeStep))
			abilRet->Finished = true;
	}
}

void CCombatSkill::Render()
{
}

void CCombatSkill::ResetResultsApplied()
{
	CAbilityObjectBase::ResetResultsApplied();
}
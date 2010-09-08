#include "StdAfx.h"

#include "NonCombatSkill.h"
#include "Globals.h"

CNonCombatSkill::CNonCombatSkill(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj, const NonCombatSkillProperties& props) : 
	CAbilityObjectBase(type, pos, name, abilityFunc, qbObj),
	m_Properties(props)
{

}

CNonCombatSkill::~CNonCombatSkill()
{

}

void CNonCombatSkill::Update(double dTimeStep, AbilityReturnBase* const abilRet)
{
	m_fpAbilityFunc(m_pTarget, m_pOwner, abilRet);
}

void CNonCombatSkill::Render()
{
// 	if (m_nImageID > -1)
// 	{
// 		Globals::g_pTM->DrawWithZSort(m_nImageID, m_ptPos.x, m_ptPos.y, 1.0f);
// 	}
}
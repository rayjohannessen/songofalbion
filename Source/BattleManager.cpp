#include "stdafx.h"

#include "BattleManager.h"
#include "ObjectManager.h"
#include "Unit.h"
#include "Object.h"
#include "Building.h"
#include "City.h"
#include "AbilityObjectBase.h"
#include "CombatSkill.h"

bool CBattleManager::Update(double dElapsedTime)
{
	if (!m_pCurrAbility)	// nothing to do, get out
		return false;

	bool battleComplete = false;
	if (m_bPerformingHit)
	{
		if (!((CUnit*)m_pCurrentObj)->GetCurrAnim().IsPlaying())	// wait until the unit being hit is done playing Hit anim
		{// mark ability as finished, go on to the next object
			m_bPerformingHit = false;
			battleComplete = OnAbilityFinished();
		}
		return battleComplete;
	}
	else if (m_bPerformingDeath)
	{
		if (!((CUnit*)m_pCurrentObj)->GetCurrAnim().IsPlaying())  // wait until the unit being killed is done playing Dying anim
		{
			m_bPerformingDeath = false;
			// TODO:: check if any units remain to continue the battle
			battleComplete = true;
			Reset();
		}
		return battleComplete;
	}

	// TODO:: incorporate defensive abilities... (needing to update both abilities simultaneously)
	// for now, just do the currently attacking object's ability
	CombatAbilityReturn abilRet;
	m_pCurrAbility->Update(dElapsedTime, &abilRet);

	if (abilRet.ApplyDamages)
		OnApplyDamages(&abilRet);
	else if(abilRet.Finished)
		battleComplete = OnAbilityFinished(&abilRet);
	

	return battleComplete;
}

bool CBattleManager::OnAbilityFinished( CombatAbilityReturn *abilRet )
{
	// reset current - should be done at the beginning of an object's turn
	m_pCurrAbility->ResetResultsApplied();

	CUnit* unit;
	// which object goes next?
	if (m_pCurrentObj == m_pAttacker)
	{
		m_pCurrAbility	= m_pOrigDefenderAbil;
		m_pCurrentObj	= m_pDefender;
		unit			= ((CUnit*)m_pCurrentObj);
		// this unit is defending, see if it has any free counters
		if (m_pCurrAbility->GetProps()->CurrFreeCounters > 0)
		{
			// a free counter-attack
			--m_pCurrAbility->GetProps()->CurrFreeCounters;
			unit->ChangeAnim("Attack");
			unit->GetCurrAnim().Play();
			return false;
		} 
		else if (false /* TODO:: this player chose to use stamina to counter-attack*/ && unit->GetStamina() >= m_pCurrAbility->GetStaminaRequired(false))
		{
			unit->ChangeAnim("Attack");
			unit->GetCurrAnim().Play();
			return false;
		}
		// the defender cannot counter-attack, fall through to next if check
	}
	if (m_pCurrentObj == m_pDefender)	// either the defender can't counter, or it just has...see if the attacker can attack again
	{
// 		m_pCurrAbility	= m_pOrigAttackerAbil;
// 		m_pCurrentObj	= m_pAttacker;
// 		unit			= ((CUnit*)m_pCurrentObj);
// 		if (unit->GetStamina() >= m_pCurrAbility->GetStaminaRequired(true))
// 		{
// 			unit->DecrementStamina(m_pCurrAbility->GetStaminaRequired(true));
// 		} 
// 		else	// the attacker is done as well, signal to end this battle
// 		{
// 			Reset();
// 			return true;
// 		}
		m_pDefender->GetCurrDefenseAbility()->GetProps()->CurrFreeCounters = m_pCurrAbility->GetProps()->CurrFreeCounters;
		Reset();
		return true;	// the attacker is done as well, signal to end this battle
	}

	return false;
}

void CBattleManager::OnApplyDamages( CombatAbilityReturn *abilRet )
{
	ObjIter iter = abilRet->ObjsToEliminate.begin();
	ObjIter end  = abilRet->ObjsToEliminate.end();
	for ( ; iter != end; ++iter)
	{
		m_pCurrentObj = (*iter);
		((CUnit*)(*iter))->ChangeAnim("Death");
		((CUnit*)(*iter))->GetCurrAnim().Play();
		// find victor
		// TODO:: adjust for multiple objects...
		if (m_pCurrentObj == m_pDefender)
			*m_pVictor = m_pAttacker;
		else
			*m_pVictor = m_pDefender;
	}
	
	if (abilRet->ObjsToEliminate.size() > 0)	// TODO:: check for multiple objects & also -> same objects being hit/dying, only do death
	{
		m_bPerformingDeath = true;
		return;
	}
	else
		m_bPerformingHit = true;

	iter = abilRet->HitObjs.begin();
	end  = abilRet->HitObjs.end();
	for ( ; iter != end; ++iter)
	{
		((CUnit*)(*iter))->ChangeAnim("Hit");
		((CUnit*)(*iter))->GetCurrAnim().Play();
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CBattleManager::CBattleManager() :
m_pAttacker(NULL),
m_pDefender(NULL),
m_pCurrentObj(NULL),
m_pVictor(NULL),
m_pOrigAttackerAbil(NULL),
m_pOrigDefenderAbil(NULL),
m_pCurrAbility(NULL)
{

}
void CBattleManager::Init(CObject* attacker, CObject* defender, CObject*& mapsVictorObj)
{
	m_pAttacker = attacker;
	m_pDefender = defender;
	m_pCurrentObj = attacker;
	m_pVictor  = &mapsVictorObj;

	GetSetAbility(m_pOrigAttackerAbil, attacker, true);
	GetSetAbility(m_pOrigDefenderAbil, defender, false);
	
	m_pOrigDefenderAbil->SetObjects(attacker, defender);
	m_pOrigAttackerAbil->SetObjects(defender, attacker);
	m_pCurrAbility = m_pOrigAttackerAbil;
	m_bPerformingHit = m_bPerformingDeath = false;
}

void CBattleManager::GetSetAbility(CCombatSkill*& abilToSet, CObject* const obj, bool isAttackAbil)
{
	eButtonName defaultType;
	CCombatSkill* currAbil = obj->GetCurrDefaultAbility(defaultType);
	if (NULL == currAbil)
	{// we have to grab the default ability
		switch (defaultType)
		{
		case BN_COMBAT_SKILLS:
			{
				abilToSet = new CCombatSkill(*(CCombatSkill*)obj->GetAbilitiesMap()[BN_COMBAT_SKILLS][0]);
			}break;
		}
		if (isAttackAbil)
			obj->SetCurrAttackAbility(abilToSet);
		else
			obj->SetCurrDefenseAbility(abilToSet);
	}
	else	// use the current ability
	{
		switch (defaultType)
		{
		case BN_COMBAT_SKILLS:
			{
				if (isAttackAbil)
					abilToSet = new CCombatSkill(*obj->GetCurrAttackAbility());
				else
					abilToSet = new CCombatSkill(*obj->GetCurrDefenseAbility());
			}break;
		}
	}
}

void CBattleManager::Reset()
{
	SAFE_DELETE(m_pOrigAttackerAbil);
	SAFE_DELETE(m_pOrigDefenderAbil);
	m_pCurrAbility = NULL;
}

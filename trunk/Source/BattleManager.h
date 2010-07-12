#pragma once

class CObject;
class CCombatSkill;
struct AbilityReturn;

class CBattleManager
{
	typedef vector<CObject*>::iterator ObjIter;

	bool	m_bPerformingHit;
	bool	m_bPerformingDeath;

	// objects involved in battle
	CObject* m_pAttacker;
	CObject* m_pDefender;
	CObject* m_pCurrentObj;
	// abilities of the objects
	CCombatSkill* m_pOrigAttackerAbil;
	CCombatSkill* m_pOrigDefenderAbil;
	CCombatSkill* m_pCurrAbility;

	void GetSetAbility(CCombatSkill*& abilToSet, CObject* const obj, bool isAttackAbil);
	inline void Reset();
public:
	CBattleManager();
	void Init(CObject* attacker, CObject* defender);

	// Update -- returns true if battle is finished
	bool Update(double dElapsedTime);

	// returns true if the battle is complete
	bool OnAbilityFinished( AbilityReturn* abilRet = NULL);
	void OnApplyDamages( AbilityReturn *abilRet );
};
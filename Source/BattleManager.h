#pragma once

class CObject;
class CCombatSkill;
struct CombatAbilityReturn;

class CBattleManager
{
	typedef vector<CObject*>::iterator ObjIter;

	bool	m_bPerformingHit;
	bool	m_bPerformingDeath;

	// objects involved in battle
	CObject* m_pAttacker;
	CObject* m_pDefender;
	CObject* m_pCurrentObj;
	CObject** m_pVictor;	// points to the map's member, set if it's the last "man" standing
	// abilities of the objects
	CCombatSkill* m_pOrigAttackerAbil;
	CCombatSkill* m_pOrigDefenderAbil;
	CCombatSkill* m_pCurrAbility;

	void GetSetAbility(CCombatSkill*& abilToSet, CObject* const obj, bool isAttackAbil);
	inline void Reset();
public:
	CBattleManager();
	void Init(CObject* attacker, CObject* defender, CObject *& mapsDestObj);

	// Update -- returns true if battle is finished
	bool Update(double dElapsedTime);

	// returns true if the battle is complete
	bool OnAbilityFinished( CombatAbilityReturn* abilRet = NULL);
	void OnApplyDamages( CombatAbilityReturn *abilRet );
};
#pragma once

#define QBSLOT_SIZE point(32, 32)
class CAbilityObjectBase;
class CQuickBarObject 
{
public:
	static const int NUM_COLUMNS = 8;

	CAbilityObjectBase* Ability;
	int		ImageID;		// the action's icon image
	rect	SrcRect;		// where on the sheet the icon is
	string	Description;

	CQuickBarObject() : ImageID(-1), Description(""), Ability(NULL)
	{}
	CQuickBarObject(int id, const string& description, int rectID) : 
		Ability(NULL),
		ImageID(id),
		SrcRect( point((rectID % NUM_COLUMNS) * QBSLOT_SIZE.x, (rectID / NUM_COLUMNS) * QBSLOT_SIZE.y), QBSLOT_SIZE ),
		Description(description)
	{}
	~CQuickBarObject()
	{}
};
#define NUM_QB_SLOTS 5

//////////////////////////////////////////////////////////////////////////
// Ability return structs - for holding/passing the results of an ability
//							after having been used
//////////////////////////////////////////////////////////////////////////
class CObject;
typedef vector<CObject*> ObjList;
struct AbilityReturnBase 
{
	bool Finished;

	AbilityReturnBase() : Finished(false)
	{}
	virtual void Reset() = 0;
};
struct CombatAbilityReturn : public AbilityReturnBase
{
	ObjList HitObjs;
	ObjList ObjsToEliminate;
	bool	ApplyDamages;

	virtual ~CombatAbilityReturn() {}
	void Reset()
	{
		HitObjs.clear();
		ObjsToEliminate.clear();
		Finished = ApplyDamages = false;
	}
	CombatAbilityReturn() : AbilityReturnBase(), ApplyDamages(false)
	{}
};
struct NonCombatAbilityReturn : public AbilityReturnBase 
{
	virtual ~NonCombatAbilityReturn() {}
	void Reset()
	{
		Finished = false;
	}
	NonCombatAbilityReturn() : AbilityReturnBase()
	{}
};
//////////////////////////////////////////////////////////////////////////

typedef void (*AbilityFunction)(CObject* targetObj, CObject* thisPtr, AbilityReturnBase* const abilRet);

enum eAbilityTypes 
{
	ABT_COMBAT_SKILL,
	ABT_NONCOMBAT_SKILL,
	ABT_SKILL = ABT_NONCOMBAT_SKILL,
	NUM_ABILITY_TYPES,
};

// TODO::TEMP (simulated vfx)
#include "Timer.h"

class CAbilityObjectBase
{

protected:
	bool			m_bResultsApplied;	// specifies if the abilities results have been calculated and applied to the target
	eAbilityTypes	m_eType;
	CQuickBarObject* m_pQBObj;
	//////////////////////////////////////////////////////////////////////////
	// NOTE: target and owner are strictly for when the ability is being used
	CObject*		m_pTarget, *m_pOwner;// the objects that the ability deals with, target and owner (owner == the owner of the ability)
	//////////////////////////////////////////////////////////////////////////
	string			m_strName;
	point			m_ptPos;	// screen pos
	AbilityFunction m_fpAbilityFunc;

	CTriggerTimer SimulatedVFX;

public:
	CAbilityObjectBase() : m_strName("NONE"), m_ptPos(0, 0), m_pTarget(NULL), m_pOwner(NULL), m_pQBObj(NULL) {}
	CAbilityObjectBase(eAbilityTypes type, point pos, string name, AbilityFunction abilityFunc, CQuickBarObject* qbObj) : 
		m_bResultsApplied(false),
		m_eType(type), 
		m_pTarget(NULL), 
		m_pOwner(NULL), 
		m_strName(name), 
		m_ptPos(pos), 
		m_fpAbilityFunc(abilityFunc)
	{
		m_pQBObj = new CQuickBarObject(*qbObj);
		m_pQBObj->Ability = this;
	}
	CAbilityObjectBase(const CAbilityObjectBase& obj)
	{
		m_bResultsApplied = obj.m_bResultsApplied;
		m_eType			= obj.m_eType;
		m_pTarget 		= obj.m_pTarget;
		m_pOwner  		= obj.m_pOwner;
		m_strName 		= obj.m_strName;
		m_ptPos   		= obj.m_ptPos;
		m_fpAbilityFunc = obj.m_fpAbilityFunc;
		m_pQBObj		= new CQuickBarObject(*obj.m_pQBObj);
		m_pQBObj->Ability = this;
	}

	virtual ~CAbilityObjectBase() { SAFE_DELETE(m_pQBObj); }

	virtual void Update(double dTimeStep, AbilityReturnBase* const abilRet) = 0;
	virtual void Render() = 0;

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	inline string GetName()				const	{ return m_strName;			}
	inline eAbilityTypes GetType()		const	{ return m_eType;			}
	inline CQuickBarObject* GetQBObj()	const	{ return m_pQBObj;			}

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	inline void SetObjects( CObject* target, CObject* owner)	{ m_pTarget = target; m_pOwner = owner;	}
	virtual void ResetResultsApplied()							{ m_bResultsApplied = false;			}
};

typedef vector<CAbilityObjectBase*> Abilities;
typedef Abilities::const_iterator AbilitiesIter;
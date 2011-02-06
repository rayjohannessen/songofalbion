#pragma once

#include "AbilityObjectBase.h"
#include "ButtonDefines.h"

class CObject;
class TiXmlElement;
struct ObjectData;

typedef string ObjectName;
typedef string AbilityName;

///////////////////// AbilitiesMananger //////////////////////////////////
//	This class will hold all ability functions that can be used.
//	when an object's ability is set by it's string name, the ability is
//	retrieved and set from here 
//
//	- A SINGLETON -
//
//////////////////////////////////////////////////////////////////////////

enum eAbility 
{
	A_BASE = 0,
	A_CHARGE = 1, A_FLANK = 2,
	A_HEALUNIT = 3, A_PROMOTEUNIT = 4,
	A_FORTIFY = 5,
	NUM_ABILITIES
};

struct AbilityInfo 
{
	string Name;
	AbilityFunction FuncPtr;
	AbilityInfo() {}
	AbilityInfo(const string name, AbilityFunction func) : Name(name), FuncPtr(func) {}
};

class CAbilitiesManager
{
	typedef vector<AbilityName> AbilityNames;		// all of the ability names (separated by types into individual variable vectors)
	typedef pair<ObjectName, AbilityName> ObjectAbilityPair;			// an object name paired with its ability names
	typedef pair<AbilityName, CAbilityObjectBase*> AbilityNameFuncPair;			// an ability name (string) paired with its ability
	typedef multimap<ObjectName, CAbilityObjectBase*> ObjectAbilityMultimap;		// multimap of all objects and all their abilities
	typedef multimap<ObjectName, CAbilityObjectBase*>::iterator ObjectAbilityIter;	// the iterator
	typedef pair<multimap<ObjectName, CAbilityObjectBase*>::iterator,					// the range for the multimap of objects and their abilities
				 multimap<ObjectName, CAbilityObjectBase*>::iterator> AbilityMultimapRange;

	typedef int ButtonEnum;	// i.e., the button type (eButtonName)
	typedef vector<CAbilityObjectBase*> Abilities;
	typedef map<eButtonName, vector<CAbilityObjectBase*>> UnlockedAbilitiesMap;	// the unlocked abilities map for a specific button (part of the following multimap)
	typedef multimap<ObjectName, map<eButtonName, vector<CAbilityObjectBase*>>> UnlockedAbilitiesMultimap;	// multimap of each object's abilities, which correspond to button enums
	typedef multimap<ObjectName, map<eButtonName, vector<CAbilityObjectBase*>>>::iterator UnlockedAbilitiesIter; // the iterator
	typedef pair<multimap<ObjectName, map<eButtonName, vector<CAbilityObjectBase*>>>::iterator,		// the range for the multimap of objects and their unlocked abilities
		multimap<ObjectName, map<eButtonName, vector<CAbilityObjectBase*>>>::iterator> UnlockedAbilitiesRange;
	typedef vector<CQuickBarObject*> QuickBarObjects;
	typedef vector<CQuickBarObject*>::iterator QBObjIter;

	AbilityNames m_vAbilityNames;	// filled with attack names that files have definitions for the corresponding attacks

	ObjectAbilityMultimap	m_mObjectAbilities;
	AbilityMultimapRange	m_Range;
	ObjectAbilityIter		m_iBegin;

	UnlockedAbilitiesMultimap	m_mStartingUnlockedAbilities;
	UnlockedAbilitiesIter	m_iBeginUnlockedAbilities;
	UnlockedAbilitiesRange	m_RangeUnlockedAbilities;

	QuickBarObjects	m_vQBObjects;

	// make this a singleton
	CAbilitiesManager() {}
	~CAbilitiesManager() {}
	CAbilitiesManager(const CAbilitiesManager&);
	CAbilitiesManager& operator= (const CAbilitiesManager&);

	void GetUnitInfo( TiXmlElement* pRoot );
	void GetCityInfo( TiXmlElement* pRoot );
	void GetBuildingInfo( TiXmlElement* pRoot );

	void GetObjectInfo( TiXmlElement* _root );

	void InitUnitCombatBaseAbility(const ObjectData* unitData);
	bool LoadAbilityInfo(const string& path);

public:
	static CAbilitiesManager* GetInstance();
	void Init();
	void Shutdown();

	void AddAttackName(const string& attackName) { m_vAbilityNames.push_back(attackName); }
	//////////////////////////////////////////////////////////////////////////
	// Accessors
	CAbilityObjectBase* GetAbility(const string& unitName, const string& ability);
//	vector<string> GetAllAttackStrings(const string& unitName);

	// fills out "abilities", returns true if abilities exist for the "objName" given
	bool GetUnlockedStartingAbilities(const string& objName, UnlockedAbilitiesMap& abilities);
};
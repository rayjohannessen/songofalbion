#include "stdafx.h"

#include "AbilitiesManager.h"
#include "Unit.h"
#include "Globals.h"
#include "CombatSkill.h"
#include "NonCombatSkill.h"
#include "Assets.h"
#include "Wrappers/tinyxml/tinyxml.h"
#include "AbilitiesFunctions.h"
using namespace CombatFunctions;
using namespace NonCombatFunctions;

// correspond to eAbility enums
// TODO:: maybe change this ?? put name in XML file instead?
const static AbilityInfo arrAbilityInfo[NUM_ABILITIES] =
{
	AbilityInfo("Attack", NULL),
	AbilityInfo("Charge", Charge),
	AbilityInfo("Flank", Flank),
	AbilityInfo("Heal Unit", HealUnit),
	AbilityInfo("Promote Unit", PromoteUnit),
	AbilityInfo("Fortify", Fortify)
};

#pragma region INITIALIZATION_FUNCTIONS
void CAbilitiesManager::Init()
{
	if( !LoadAbilityInfo("Resources/Objects/") )
		MessageBox(NULL, "Some or all Attack info failed to load.", "Error", MB_OK);
}
// TODO:: there's got to be a cleaner (less code-repetition) way
bool CAbilitiesManager::LoadAbilityInfo(const string &path)
{
	bool bSuccess = true;
	vector<string> folders;
	vector<string> toIgnore; toIgnore.push_back(".svn");
	Utilities::GetFoldersInDirectory(path, folders, &toIgnore);	// get all the folders in Objects dir

	vector<string>::iterator iter, end;
	for (iter = folders.begin(), end = folders.end(); iter != end; ++iter)
	{
		string fileName = path + (*iter) + "/XML/" + (*iter) + "AbilityInfo.xml";
		TiXmlDocument doc(fileName.c_str());

		if (doc.LoadFile())
		{
			TiXmlElement* pRoot = doc.RootElement();
			if (pRoot)
			{
				string val = pRoot->Value();
				if (val == "Units")
				{
					GetUnitInfo(pRoot);
				}
				else if (val == "Cities")
				{
					GetCityInfo(pRoot);
				}
				else if (val == "Buildings")
				{
					GetBuildingInfo(pRoot);
				}
			}
			doc.Clear();
		}
		// 		else
		// 			bSuccess = false;
	}

	return bSuccess;
}

void CAbilitiesManager::InitUnitCombatBaseAbility(const ObjectData* unitData)
{
	eButtonName buttonEnum = BN_COMBAT_SKILLS;
	Abilities vUnlockedAbilities;
	CAbilityObjectBase* ability = NULL;
	CQuickBarObject* qbObj = NULL;
	// setup all the base attacks
	switch (unitData->Type)
	{
	case UnitDefines::UT_GROUND:
		{			// TODO:: read in point pos?
			qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", 1);
			ability = new CCombatSkill(ABT_COMBAT_SKILL, point(0, 0), arrAbilityInfo[A_BASE].Name, GroundBaseAttack, qbObj, CombatSkillProperties());
			m_vQBObjects.push_back(qbObj);
			m_mObjectAbilities.insert( make_pair( unitData->Name, ability ) );
		}break;
	case UnitDefines::UT_AIR:
		{
			qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", 1);
			ability = new CCombatSkill(ABT_COMBAT_SKILL, point(0, 0), arrAbilityInfo[A_BASE].Name, AirBaseAttack, qbObj, CombatSkillProperties());
			m_vQBObjects.push_back(qbObj);
			m_mObjectAbilities.insert( make_pair( unitData->Name, ability ) );
		}break;
	case UnitDefines::UT_SEA:
		{
			qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", 1);
			ability = new CCombatSkill(ABT_COMBAT_SKILL, point(0, 0), arrAbilityInfo[A_BASE].Name, SeaBaseAttack, qbObj, CombatSkillProperties());
			m_vQBObjects.push_back(qbObj);
			m_mObjectAbilities.insert( make_pair( unitData->Name, ability ) );
		}break;
	}
	// all base attacks are starting abilities
	vUnlockedAbilities.push_back(ability);
	// create the map for this unit name, put it in the abilities
	UnlockedAbilitiesMap m; m[buttonEnum] = vUnlockedAbilities;
	m_mStartingUnlockedAbilities.insert( make_pair( unitData->Name, m ) );
}
void CAbilitiesManager::GetUnitInfo( TiXmlElement* pRoot )
{
	TiXmlElement* pUnit = pRoot->FirstChildElement("Unit");
	eAbility eAttackType;
	bool bIsStartingAbility;
	int  rectId;
	float atkMod, defMod;
	int defStam, atkStam, freeCounter;
	CCombatSkill* combatSkill;
	//	CNonCombatSkill* nonCombatSkill;

	while (pUnit)	// continue while we have more units
	{
		ObjectData nameAndType;
		nameAndType.Type = atoi(pUnit->Attribute("type"));	// type (ground, air, sea...)
		nameAndType.Name = pUnit->Attribute("name");		// load in unit name

		Globals::g_vUnitNames->push_back(nameAndType);
		InitUnitCombatBaseAbility(&nameAndType);

		TiXmlElement* abilityInfo = pUnit->FirstChildElement("CombatInfo");
		if (abilityInfo)
		{
			TiXmlElement* pAttack = abilityInfo->FirstChildElement("CombatAbility");
			while (pAttack)	// continue while there are more attacks
			{
				// TODO:: load in any modifiers to the attack
				bIsStartingAbility	= (bool)atoi(pAttack->Attribute("startingAbility"));
				eAttackType			= (eAbility)atoi(pAttack->Attribute("type"));
				rectId				= atoi(pAttack->Attribute("rectid"));
				atkMod				= (float)atof(pAttack->Attribute("attackModifier"));
				defMod				= (float)atof(pAttack->Attribute("defenseModifier"));
				defStam				= atoi(pAttack->Attribute("defenseStamina"));
				atkStam				= atoi(pAttack->Attribute("attackStamina"));
				freeCounter			= atoi(pAttack->Attribute("freeCounter"));

				Globals::g_pAbilitiesManager->AddAttackName(arrAbilityInfo[eAttackType].Name);

				CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", rectId);
				m_vQBObjects.push_back(qbObj);
				combatSkill = new CCombatSkill(ABT_COMBAT_SKILL, point(0,0), arrAbilityInfo[eAttackType].Name, arrAbilityInfo[eAttackType].FuncPtr, qbObj, CombatSkillProperties(atkStam, defStam, freeCounter, atkMod, defMod));
				m_mObjectAbilities.insert(make_pair(nameAndType.Name, combatSkill));

				if (bIsStartingAbility)
				{
					m_RangeUnlockedAbilities = m_mStartingUnlockedAbilities.equal_range(nameAndType.Name);
					m_iBeginUnlockedAbilities = m_RangeUnlockedAbilities.first;
					(*m_iBeginUnlockedAbilities).second[BN_COMBAT_SKILLS].push_back(combatSkill);
				}

				pAttack = pAttack->NextSiblingElement("CombatAbility");
			}
		}
		else if ( (abilityInfo = pUnit->FirstChildElement("NonCombatInfo")) )
		{

		}
		pUnit = pUnit->NextSiblingElement("Unit");
	}
}

void CAbilitiesManager::GetCityInfo( TiXmlElement* pRoot )
{
	TiXmlElement* pcity = pRoot->FirstChildElement("City");
	eAbility eAbilityType;
	int  rectId;
	bool bIsStartingAbility;
	float atkMod, defMod;
	int defStam, atkStam, freeCounter;
	CCombatSkill* combatSkill;
	CNonCombatSkill* nonCombatSkill;
	eButtonName buttonEnum;

	while (pcity)	// continue while we have more city types
	{
		ObjectData nameAndType;	// only Name needed for city
		nameAndType.Name = pcity->Attribute("name");		// load in city name (same as size...?)
		UnlockedAbilitiesMap m;

		TiXmlElement* abilityInfo = pcity->FirstChildElement("NonCombatInfo");
		if (abilityInfo)
		{
			buttonEnum = BN_NONCOMBAT_SKILLS;
			Abilities vUnlockedAbilities;	// a new Abilities vector for each city typename

			TiXmlElement* pAbility = abilityInfo->FirstChildElement("NonCombatAbility");
			while (pAbility)	// continue while there are more abilities
			{
				bIsStartingAbility	= (bool)atoi(pAbility->Attribute("startingAbility"));
				eAbilityType		= (eAbility)atoi(pAbility->Attribute("type"));
				rectId				= atoi(pAbility->Attribute("rectid"));

				Globals::g_pAbilitiesManager->AddAttackName(arrAbilityInfo[eAbilityType].Name);

				CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "City NCombat Ability desc", rectId);
				m_vQBObjects.push_back(qbObj);
				nonCombatSkill = new CNonCombatSkill(ABT_NONCOMBAT_SKILL, point(0,0), arrAbilityInfo[eAbilityType].Name, arrAbilityInfo[eAbilityType].FuncPtr, qbObj, NonCombatSkillProperties());
				m_mObjectAbilities.insert(make_pair(nameAndType.Name, nonCombatSkill));

				if (bIsStartingAbility)
				{	
					vUnlockedAbilities.push_back(nonCombatSkill);
				}

				pAbility = pAbility->NextSiblingElement("NonCombatAbility");
			}
			// add to the unlocked ability map for this city's NON_COMBAT_SKILLS
			m[buttonEnum] = vUnlockedAbilities;
		}
		if ( (abilityInfo = pcity->FirstChildElement("CombatInfo")) )
		{
			buttonEnum = BN_COMBAT_SKILLS;
			Abilities vUnlockedAbilities;	// a new Abilities vector for each city typename
			TiXmlElement* pAttack = abilityInfo->FirstChildElement("CombatAbility");
			while (pAttack)	// continue while there are more attacks
			{
				// TODO:: load in any modifiers to the attack
				bIsStartingAbility	= (bool)atoi(pAttack->Attribute("startingAbility"));
				eAbilityType		= (eAbility)atoi(pAttack->Attribute("type"));
				rectId				= atoi(pAttack->Attribute("rectid"));
				atkMod				= (float)atof(pAttack->Attribute("attackModifier"));
				defMod				= (float)atof(pAttack->Attribute("defenseModifier"));
				defStam				= atoi(pAttack->Attribute("defenseStamina"));
				atkStam				= atoi(pAttack->Attribute("attackStamina"));
				freeCounter			= atoi(pAttack->Attribute("freeCounter"));

				Globals::g_pAbilitiesManager->AddAttackName(arrAbilityInfo[eAbilityType].Name);

				CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", rectId);
				m_vQBObjects.push_back(qbObj);
				combatSkill = new CCombatSkill(ABT_COMBAT_SKILL, point(0,0), arrAbilityInfo[eAbilityType].Name, arrAbilityInfo[eAbilityType].FuncPtr, qbObj, CombatSkillProperties(atkStam, defStam, freeCounter, atkMod, defMod));
				m_mObjectAbilities.insert(make_pair(nameAndType.Name, combatSkill));

				if (bIsStartingAbility)
				{
					vUnlockedAbilities.push_back(combatSkill);
				}

				pAttack = pAttack->NextSiblingElement("CombatAbility");
			}
			m[buttonEnum] = vUnlockedAbilities;
		}
		pcity = pcity->NextSiblingElement("City");
		m_mStartingUnlockedAbilities.insert( make_pair( nameAndType.Name, m ) );
	}
}

void CAbilitiesManager::GetBuildingInfo( TiXmlElement* pRoot )
{
	TiXmlElement* pBuilding = pRoot->FirstChildElement("Building");
	int  rectId;
	eAbility eAbilityType;
	bool bIsStartingAbility;
	float atkMod, defMod;
	int defStam, atkStam, freeCounter;
	CCombatSkill* combatSkill;
	CNonCombatSkill* nonCombatSkill;
	eButtonName buttonEnum;

	while (pBuilding)	// continue while we have more building types
	{
		ObjectData nameAndType;
		nameAndType.Name = pBuilding->Attribute("name");		// load in building name (same as size...?)
		UnlockedAbilitiesMap m;

		TiXmlElement* abilityInfo = pBuilding->FirstChildElement("NonCombatInfo");
		if (abilityInfo)
		{
			buttonEnum = BN_NONCOMBAT_SKILLS;
			Abilities vUnlockedAbilities;	// a new Abilities vector for each city typename

			TiXmlElement* pAbility = abilityInfo->FirstChildElement("NonCombatAbility");
			while (pAbility)	// continue while there are more abilities
			{
				bIsStartingAbility	= (bool)atoi(pAbility->Attribute("startingAbility"));
				eAbilityType		= (eAbility)atoi(pAbility->Attribute("type"));
				rectId				= atoi(pAbility->Attribute("rectid"));

				Globals::g_pAbilitiesManager->AddAttackName(arrAbilityInfo[eAbilityType].Name);

				CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Building NCombat Ability desc", rectId);
				m_vQBObjects.push_back(qbObj);
				nonCombatSkill = new CNonCombatSkill(ABT_NONCOMBAT_SKILL, point(0,0), arrAbilityInfo[eAbilityType].Name, arrAbilityInfo[eAbilityType].FuncPtr, qbObj, NonCombatSkillProperties());
				m_mObjectAbilities.insert(make_pair(nameAndType.Name, nonCombatSkill));

				if (bIsStartingAbility)
				{	
					vUnlockedAbilities.push_back(nonCombatSkill);
				}

				pAbility = pAbility->NextSiblingElement("NonCombatAbility");
			}
			// add to the unlocked ability map for this building's NON_COMBAT_SKILLS
			m[buttonEnum] = vUnlockedAbilities;
		}
		if ( (abilityInfo = pBuilding->FirstChildElement("CombatInfo")) )
		{
			buttonEnum = BN_COMBAT_SKILLS;
			Abilities vUnlockedAbilities;	// a new Abilities vector for each typename
			TiXmlElement* pAttack = abilityInfo->FirstChildElement("CombatAbility");
			while (pAttack)	// continue while there are more attacks
			{
				// TODO:: load in any modifiers to the attack
				bIsStartingAbility	= (bool)atoi(pAttack->Attribute("startingAbility"));
				eAbilityType		= (eAbility)atoi(pAttack->Attribute("type"));
				rectId				= atoi(pAttack->Attribute("rectid"));
				atkMod				= (float)atof(pAttack->Attribute("attackModifier"));
				defMod				= (float)atof(pAttack->Attribute("defenseModifier"));
				defStam				= atoi(pAttack->Attribute("defenseStamina"));
				atkStam				= atoi(pAttack->Attribute("attackStamina"));
				freeCounter			= atoi(pAttack->Attribute("freeCounter"));

				Globals::g_pAbilitiesManager->AddAttackName(arrAbilityInfo[eAbilityType].Name);

				CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", rectId);
				m_vQBObjects.push_back(qbObj);
				combatSkill = new CCombatSkill(ABT_COMBAT_SKILL, point(0,0), arrAbilityInfo[eAbilityType].Name, arrAbilityInfo[eAbilityType].FuncPtr, qbObj, CombatSkillProperties(atkStam, defStam, freeCounter, atkMod, defMod));
				m_mObjectAbilities.insert(make_pair(nameAndType.Name, combatSkill));

				if (bIsStartingAbility)
				{
					vUnlockedAbilities.push_back(combatSkill);
				}

				pAttack = pAttack->NextSiblingElement("CombatAbility");
			}
			m[buttonEnum] = vUnlockedAbilities;
		}
		pBuilding = pBuilding->NextSiblingElement("Building");
		m_mStartingUnlockedAbilities.insert( make_pair( nameAndType.Name, m ) );
	}
}

CAbilitiesManager* CAbilitiesManager::GetInstance()
{
	static CAbilitiesManager instance;
	return &instance;
}
#pragma endregion

void CAbilitiesManager::Shutdown()
{
	ObjectAbilityIter end;
	for (m_iBegin = m_mObjectAbilities.begin(), end = m_mObjectAbilities.end(); m_iBegin != end; ++m_iBegin)
		SAFE_DELETE((*m_iBegin).second);
	m_mObjectAbilities.clear();

	for (QBObjIter qbIter = m_vQBObjects.begin(), qbEnd = m_vQBObjects.end(); qbIter != qbEnd; ++qbIter)
		SAFE_DELETE((*qbIter));
	m_vQBObjects.clear();
}

CAbilityObjectBase* CAbilitiesManager::GetAbility(const string &unitName, const string &ability)
{
	m_Range = m_mObjectAbilities.equal_range(unitName);

	for ( m_iBegin = m_Range.first; m_iBegin != m_Range.second; ++m_iBegin)
		if ((*m_iBegin).second->GetName() == ability)
			return (*m_iBegin).second;

	// not found...shouldn't get here
	return NULL;
}

// vector<string> CAbilitiesManager::GetAllAttackStrings(const string &unitName)
// {
// 	m_Range = m_mObjectAbilities.equal_range(unitName);
// 	vector<string> attacks;
// 	for ( m_iBegin = m_Range.first; m_iBegin != m_Range.second; ++m_iBegin)
// 		attacks.push_back((*m_iBegin).second.first);
// 	return attacks;
// }

bool CAbilitiesManager::GetUnlockedStartingAbilities(const string &objName, UnlockedAbilitiesMap& abilities)
{
	// gotten from a multimap of Object names, containing a map of ability enums for the 
	//	type of ability (the key), corresponding to all the starting abilities
	if (m_mStartingUnlockedAbilities.size())
	{
		unsigned count = m_mStartingUnlockedAbilities.count(objName);
		if (count)
		{
			m_RangeUnlockedAbilities = m_mStartingUnlockedAbilities.equal_range(objName);
			m_iBeginUnlockedAbilities = m_RangeUnlockedAbilities.first;
			abilities = (*m_iBeginUnlockedAbilities).second;
			return true;
		}
	}
	return false;
}


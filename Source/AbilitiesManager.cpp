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


CAbilitiesManager* CAbilitiesManager::GetInstance()
{
	static CAbilitiesManager instance;
	return &instance;
}

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

void CAbilitiesManager::Init()
{
	m_mAbilityInfo["Attack"] = AbilityInfo("Attack", NULL);
	m_mAbilityInfo["Charge"] = AbilityInfo("Charge", Charge);
	m_mAbilityInfo["Flank"] = AbilityInfo("Flank", Flank);
	m_mAbilityInfo["Heal_Unit"] = AbilityInfo("Heal_Unit", HealUnit);
	m_mAbilityInfo["Promote_Unit"] = AbilityInfo("Promote_Unit", PromoteUnit);
	m_mAbilityInfo["Fortify"] = AbilityInfo("Fortify", Fortify);

	if( !LoadAbilityInfo("Resources/Objects/") )
		MessageBox(NULL, "Some or all Attack info failed to load.", "Error", MB_OK);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

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
				GetObjectInfo(pRoot);
			doc.Clear();
		}
		else
			MessageBox(NULL, (fileName + " does not exit").c_str(), "FileNotFound", MB_OK);
	}

	return bSuccess;
}

void CAbilitiesManager::GetObjectInfo( TiXmlElement* _root )
{
	TiXmlElement* pObject = _root->FirstChildElement();
	string strObjType = pObject->Value();
	AbilityName strAbilityName;
	bool bIsStartingAbility;
	int  rectId;
	
	while (pObject)	// continue while we have more objects
	{
		UnlockedAbilitiesMap m;
		ObjectData nameAndType;
		nameAndType.Type = atoi(pObject->Attribute("type"));	// type (ground, air, sea...)
		nameAndType.Name = pObject->Attribute("name");			// load in name

		if (strObjType == "Unit")	// unit-specific base attacks have to be initialized first
		{
			Globals::g_vUnitNames->push_back(nameAndType);
			InitUnitCombatBaseAbility(&nameAndType);
		}

		TiXmlElement* abilityInfo = pObject->FirstChildElement("CombatInfo");
		if (abilityInfo)	// combat ability
		{
			Abilities vUnlockedAbilities;	// a new Abilities vector for each typename

			TiXmlElement* pAttack = abilityInfo->FirstChildElement("CombatAbility");
			while (pAttack)	// continue while there are more attacks
			{
				bIsStartingAbility	= atoi(pAttack->Attribute("startingAbility")) != 0; // != is to avoid int to bool perf warning 
				strAbilityName		= pAttack->Attribute("name");
				rectId				= atoi(pAttack->Attribute("rectid"));

				// load in any modifiers to the attack
				CombatSkillProperties CSkillProps;
				CSkillProps.AttkDamageMod = (float)atof(pAttack->Attribute("attackModifier"));
				CSkillProps.DefDamageMod  = (float)atof(pAttack->Attribute("defenseModifier"));
				CSkillProps.DefStam	= atoi(pAttack->Attribute("defenseStamina"));
				CSkillProps.AttackStam = atoi(pAttack->Attribute("attackStamina"));
				CSkillProps.FreeCounters = atoi(pAttack->Attribute("freeCounter"));

				Globals::g_pAbilitiesManager->AddAttackName(m_mAbilityInfo[strAbilityName].Name);

				CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", rectId);
				m_vQBObjects.push_back(qbObj);
				CCombatSkill* combatSkill = new CCombatSkill(ABT_COMBAT_SKILL, 
															 point(0,0), 
															 m_mAbilityInfo[strAbilityName].Name, 
															 m_mAbilityInfo[strAbilityName].FuncPtr, 
															 qbObj, 
															 CSkillProps);
				m_mObjectAbilities.insert(make_pair(nameAndType.Name, combatSkill));

				if (bIsStartingAbility)
				{
					if (strObjType == "Unit")
					{	// NOTE: as of now all units have starting unlocked abilities, so the multimap's second will always be valid
						m_RangeUnlockedAbilities = m_mStartingUnlockedAbilities.equal_range(nameAndType.Name);
						m_iBeginUnlockedAbilities = m_RangeUnlockedAbilities.first;
						(*m_iBeginUnlockedAbilities).second[BN_COMBAT_SKILLS].push_back(combatSkill);
					}
					else
						vUnlockedAbilities.push_back(combatSkill);
				}

				pAttack = pAttack->NextSiblingElement("CombatAbility");
			}
			if (strObjType != "Unit")
				m[BN_COMBAT_SKILLS] = vUnlockedAbilities;
		}
		if ( (abilityInfo = pObject->FirstChildElement("NonCombatInfo")) )
		{
			TiXmlElement* abilityInfo = pObject->FirstChildElement("NonCombatInfo");
			if (abilityInfo)
			{
				Abilities vUnlockedAbilities;	// a new Abilities vector for each typename

				TiXmlElement* pAbility = abilityInfo->FirstChildElement("NonCombatAbility");
				while (pAbility)	// continue while there are more abilities
				{
					bIsStartingAbility	= atoi(pAbility->Attribute("startingAbility")) != 0;
					strAbilityName		= pAbility->Attribute("name");
					rectId				= atoi(pAbility->Attribute("rectid"));

					Globals::g_pAbilitiesManager->AddAttackName(m_mAbilityInfo[strAbilityName].Name);

					CQuickBarObject* qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "NonCombat Ability desc", rectId);
					m_vQBObjects.push_back(qbObj);
					CNonCombatSkill* nonCombatSkill = new CNonCombatSkill(ABT_NONCOMBAT_SKILL, 
																		  point(0,0), 
																		  m_mAbilityInfo[strAbilityName].Name, 
																		  m_mAbilityInfo[strAbilityName].FuncPtr, 
																		  qbObj, 
																		  NonCombatSkillProperties());

					m_mObjectAbilities.insert(make_pair(nameAndType.Name, nonCombatSkill));

					if (bIsStartingAbility)
						vUnlockedAbilities.push_back(nonCombatSkill);

					pAbility = pAbility->NextSiblingElement("NonCombatAbility");
				}
				// add to the unlocked ability map for this object's NON_COMBAT_SKILLS
				m[BN_NONCOMBAT_SKILLS] = vUnlockedAbilities;
			}
		}
		pObject = pObject->NextSiblingElement(strObjType.c_str());

		if (strObjType != "Unit")
			m_mStartingUnlockedAbilities.insert( make_pair( nameAndType.Name, m ) );
	}
}

void CAbilitiesManager::InitUnitCombatBaseAbility(const ObjectData* unitData)
{
	eButtonName buttonEnum = BN_COMBAT_SKILLS;
	Abilities vUnlockedAbilities;
	CAbilityObjectBase* ability = NULL;
	CQuickBarObject* qbObj = NULL;
	// setup all the base attacks

	AbilityFunction abilityFunc;
	switch (unitData->Type)
	{
	case UnitDefines::UT_GROUND:
		{	// TODO:: read in point pos?
			abilityFunc = GroundBaseAttack;
		}break;
	case UnitDefines::UT_AIR:
		{
			abilityFunc = AirBaseAttack;
		}break;
	case UnitDefines::UT_SEA:
		{
			abilityFunc = SeaBaseAttack;
		}break;
	}

	qbObj = new CQuickBarObject(Globals::g_pAssets->GetGUIasts()->AbilityImages(), "Basic Attack bla bla", 1);
	ability = new CCombatSkill(ABT_COMBAT_SKILL, point(0, 0), m_mAbilityInfo["Attack"].Name, abilityFunc, qbObj, CombatSkillProperties());
	m_vQBObjects.push_back(qbObj);
	m_mObjectAbilities.insert( make_pair( unitData->Name, ability ) );

	// all base attacks are starting abilities
	vUnlockedAbilities.push_back(ability);
	// create the map for this unit name, put it in the abilities
	UnlockedAbilitiesMap m; m[buttonEnum] = vUnlockedAbilities;
	m_mStartingUnlockedAbilities.insert( make_pair( unitData->Name, m ) );
}

//////////////////////////////////////////////////////////////////////////

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


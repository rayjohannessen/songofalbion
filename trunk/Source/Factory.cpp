////////////////////////////////////////////////////////
//   File Name	:	"Factory.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Creates base objects
////////////////////////////////////////////////////////

#include"Factory.h"
#include"ObjectManager.h"
#include "tinyxml/tinyxml.h"
#include"Ninja.h"
#include"Boss.h"
#include"Turtle.h"
#include"BattleItem.h" 
#include"BattleMap.h"
#include"Assets.h"


#define TOTAL_NUM_WEAPONS 12

Factory::Factory(void)
{
	for(int i = 0; i < TOTAL_NUM_WEAPONS; i++)
	{
		weapons[i] = new CBase;
	}
	LoadItems("Resources/XML/VG_Items.xml");
}

Factory::~Factory(void)
{
	for(int i = 0; i < TOTAL_NUM_WEAPONS; i++)
	{
		delete weapons[i];
	}
}
Factory* Factory::GetInstance()
{
	static Factory instance;
	return &instance;
}
bool Factory::LoadItems(const char* fileName)
{
	TiXmlDocument doc;

	if(!doc.LoadFile(fileName))
	{
		MessageBox(0, "Failed to load items.", "Error",  MB_OK);
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();
	TiXmlElement* pBattleItem = pRoot->FirstChildElement("BattleItem");
	if(pBattleItem)
	{
		char* name[3];
		int dmg[3], range[3], radius[3];
		TiXmlElement* pItem = pBattleItem->FirstChildElement("Item");
		
		for(int i = 0; i < 3; i++)
		{
			
			name[i] = (char*)pItem->Attribute("name");
			pItem->Attribute("Dmg", &dmg[i]);
			pItem->Attribute("range", &range[i]);
			pItem->Attribute("radius", &radius[i]);

			pItem = pItem->NextSiblingElement("Item");
		}

		blackeggs.SetName("Black Eggs");
		blackeggs.SetDamage(dmg[0]);
		blackeggs.SetRange(range[0]);
		blackeggs.SetRadius(radius[0]);

		grenado.SetName("Grenado");
		grenado.SetDamage(dmg[1]);
		grenado.SetRange(range[1]);
		grenado.SetRadius(radius[1]);

		pizza.SetName("Pizza");
		pizza.SetDamage(dmg[2]);
		pizza.SetRange(range[2]);
		pizza.SetRadius(radius[2]);
	}
	TiXmlElement* pWeapon = pBattleItem->NextSiblingElement("Weapon");
	if(pWeapon)
	{
		string Name[12];
		char* temp;
		int atk[12], def[12];
		TiXmlElement* pItem = pWeapon->FirstChildElement("Item");
		for(int i = 0; i < 12; i++)
		{
			temp = (char*)pItem->Attribute("name");
			//Name[i]
			//temp = Name[i].c_str();
			pItem->Attribute("atk", &atk[i]);
			pItem->Attribute("def", &def[i]);

			weapons[i]->SetName(temp);
			weapons[i]->SetStrength(atk[i]);
			weapons[i]->SetDefense(def[i]);

			pItem = pItem->NextSiblingElement("Item");
		}


	}

	return true;
}
CBoss* Factory::CreateBoss()
{
	CBoss* boss = new CBoss();

	boss->SetAccuracy(10);
	boss->SetBaseAP(16);
	boss->SetCurrAP(16);
	boss->SetDefense(10);
	boss->SetHealthMax(250);
	//boss->SetHealth(50);
	boss->SetHealth(400);
	boss->SetSpeed(10);
	boss->SetStrength(20);

	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 1,0.35f);//idle1,0
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 2,0.35f);//idle2,1
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 3,0.05f, false);//high,2
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 4,0.15f, false);//low,3
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 5,0.15f);//flip
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 6,0.15f);//ko
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 7,0.15f);//lose
	boss->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_shredder.dat", 8,0.15f, false);//ground special
	boss->AddAnim(anim);

	boss->SetVelX(100);
	boss->SetVelY(50);

	
	ObjectManager::GetInstance()->Add(boss);
	return boss;
}

CNinja* Factory::CreateNinja(int level)
{
	CNinja* ninja = new CNinja();

	CAnimation anim;
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 1,0.75f);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 2,0.15f);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 3,0.15f);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 4,0.15f);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 5,0.15f, false);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 6,0.15f);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 7,0.15f);
	ninja->AddAnim(anim);
	anim.Load("Resources/AnimationInfo/VG_WhiteNinja1.dat", 8,0.15f);
	ninja->AddAnim(anim);

	ninja->SetStrength(16);
	ninja->SetDefense(4);
	ninja->SetAccuracy(10);
	ninja->SetRange(1);
	ninja->SetLevel(level);


	ObjectManager::GetInstance()->Add(ninja);

	return ninja;
}
CTurtle* Factory::CreateTurtle(char* name)
{
	CTurtle* turtle = new CTurtle();
	turtle->SetName(name);
	ObjectManager::GetInstance()->Add(turtle);
	return turtle;
}
CBattleItem* Factory::CreateBattleItem(int type, POINT mapPoint)
{
	CBattleItem* item = new CBattleItem();

	switch(type)
	{
	case BLACK_EGGS:
		item->SetName("Black Eggs");
		item->SetRange(blackeggs.GetRange());
		item->SetDamage(blackeggs.GetDamage());
		item->SetImageID(CAssets::GetInstance()->aEggID);
		item->SetNumType(0);

		break;
	case GRENADO:
		item->SetName("Grenado");
		item->SetRange(grenado.GetRange());
		item->SetDamage(grenado.GetDamage());
		item->SetNumType(1);
		item->SetImageID(CAssets::GetInstance()->aGrenadoID);
		break;
	case PIZZA:
		item->SetName("Pizza");
		item->SetHeal(pizza.GetDamage());
		item->SetNumType(2);
		item->SetImageID(CAssets::GetInstance()->aPizzaID);
		break;
	}
	CBattleMap* pBM =  CBattleMap::GetInstance();
	item->SetCurrTile(mapPoint, pBM->GetOffsetX(),pBM->GetOffsetY(),pBM->GetTileWidth(), pBM->GetTileHeight(),pBM->GetNumCols(),false);

	ObjectManager::GetInstance()->Add(item);
	return item;
}
CBase* Factory::CreateWeapon(int type, POINT mapPoint, bool bAddtoMngr)
{
	CBase* weapon = new CBase();

	CBase* temp;
	
	switch(type)
	{
	case BOKKEN:
		temp = weapons[BOKKEN];
		temp->SetName("Bokken");
		temp->SetImageID(CAssets::GetInstance()->aSwordID);
		temp->SetNumType(0);
		break;
	case TACHI:
		temp = weapons[TACHI];
		temp->SetName("Tachi");
		temp->SetImageID(CAssets::GetInstance()->aSwordID);
		temp->SetNumType(1);

		break;
	case KATANA:
		temp = weapons[KATANA];
		temp->SetName("Katana");
		temp->SetImageID(CAssets::GetInstance()->aSwordID);
		temp->SetNumType(2);

		break;
	case NINJATO:
		temp = weapons[NINJATO];
		temp->SetName("Ninjato");
		temp->SetImageID(CAssets::GetInstance()->aSwordID);
		temp->SetNumType(3);

		break;
	case WOODNUN:
		temp = weapons[WOODNUN];
		temp->SetName("Wooden Nunchaku");
		temp->SetImageID(CAssets::GetInstance()->aNunchakuID);
		temp->SetNumType(4);

		break;
	case GLASSNUN:
		temp = weapons[GLASSNUN];
		temp->SetName("Glass Nunchaku");
		temp->SetImageID(CAssets::GetInstance()->aNunchakuID);
		temp->SetNumType(5);

		break;
	case STEELNUN:
		temp = weapons[STEELNUN];
		temp->SetName("Steel Nunchaku");
		temp->SetImageID(CAssets::GetInstance()->aNunchakuID);
		temp->SetNumType(6);

		break;
	case OAKSTAFF:
		temp = weapons[OAKSTAFF];
		temp->SetName("Oak Bo Staff");
		temp->SetImageID(CAssets::GetInstance()->aStaffID);
		temp->SetNumType(7);

		break;
	case BAMBOOSTAFF:
		temp = weapons[BAMBOOSTAFF];
		temp->SetName("Bamboo Staff");
		temp->SetImageID(CAssets::GetInstance()->aStaffID);
		temp->SetNumType(8);

		break;
	case STEELSTAFF:
		temp = weapons[STEELSTAFF];
		temp->SetName("Steel Staff");
		temp->SetImageID(CAssets::GetInstance()->aStaffID);
		temp->SetNumType(9);

		break;
	case RUSTYSAI:
		temp = weapons[RUSTYSAI];
		temp->SetName("Rusty Sais");
		temp->SetImageID(CAssets::GetInstance()->aSaiID);
		temp->SetNumType(10);

		break;
	case POLISHEDSAI:
		temp = weapons[POLISHEDSAI];
		temp->SetName("Polished Sais");
		temp->SetImageID(CAssets::GetInstance()->aSaiID);
		temp->SetNumType(11);

		break;
	}
	weapon->SetWeapon(temp->GetName(),temp->GetStrength(), temp->GetDefense(), temp->GetImageID(), temp->GetNumType());

	weapon->SetType(OBJECT_WEAPON);
	weapon->SetWidth(32);
	weapon->SetHeight(32);

	if (bAddtoMngr)
	{
		CBattleMap* pBM =  CBattleMap::GetInstance();
		weapon->SetCurrTile(mapPoint, pBM->GetOffsetX(),pBM->GetOffsetY(),pBM->GetTileWidth(), pBM->GetTileHeight(),pBM->GetNumCols(),false);
	
		ObjectManager::GetInstance()->Add(weapon);
	}

	return weapon;
}
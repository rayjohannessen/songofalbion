#include "stdafx.h"

#include "Assets.h"
#include "Globals.h"
#include "AnimationDefinesEnums.h"
#include "AnimationsManager.h"
#include "HUD.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// CAssets /////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CAssets::LoadGUI(int index)
{
	return m_pGUI->LoadAssets(index);
}
int CAssets::LoadMap(int index)
{
	return m_pMap->LoadAssets(index);
}
int CAssets::LoadUnits(int index)
{
	return m_pUNITS->LoadAssets(index);
}

void CAssets::LoadAllAssets()	// debug
{
	LoadGUI();
	LoadMap();
	LoadUnits();
}

void CAssets::LoadInGameGUI()
{
	m_pGUI->LoadInGameHUD();
}

void CAssets::Init()
{
	m_pGUI		= new CAssetsGUI();
	m_pMap		= new CAssetsMap();
	m_pUNITS	= new CAssetsUnits();
	LoadGUI();
}
void CAssets::Shutdown()
{
	SAFE_DELETE(m_pUNITS);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pGUI);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// GUI Assets
//////////////////////////////////////////////////////////////////////////

CAssetsGUI::CAssetsGUI()
{
	SetPath("Resources/GUI/");
}
// TODO::not sure if all these will be loaded up front yet...
const int CAssetsGUI::LoadAssets(int index)
{
	m_nMouse				= Globals::g_pTM->LoadTexture((GetPath() + "mouse.png").c_str());
	m_nBlackPixel			= Globals::g_pTM->LoadTexture((GetPath() + "BlackPixel.png").c_str());
	m_nQBFrame				= Globals::g_pTM->LoadTexture((GetPath() + "QuickBar.png").c_str());
	m_nAbilityImages		= Globals::g_pTM->LoadTexture((GetPath() + "AbilityImages.png").c_str());

	return 0;
}

void CAssetsGUI::LoadInGameHUD()
{
	m_nHUD					= Globals::g_pTM->LoadTexture((GetPath() + "HUD.png").c_str(),			white);
	m_nHUDFrame				= Globals::g_pTM->LoadTexture((GetPath() + "HUD_Frame.png").c_str(),	white);
	m_nPlayerInfoBG			= Globals::g_pTM->LoadTexture((GetPath() + "playerInfoBG.png").c_str(), white);
	m_nEnemyInfoBG			= Globals::g_pTM->LoadTexture((GetPath() + "enemyInfoBG.png").c_str(),	white);

	m_nFactions[FAC_LOGRESS] = Globals::g_pTM->LoadTexture((GetPath() + "faction-1.png").c_str(), white);
	m_nFactions[FAC_PRYDAIN] = Globals::g_pTM->LoadTexture((GetPath() + "faction-2.png").c_str(), white);

	SetHUDButtons();
}

void CAssetsGUI::SetHUDButtons()
{
	// set the 3 button states' images (up, hover, down) for all buttons
	int up	  = Globals::g_pTM->LoadTexture((GetPath() + "btn-up.png").c_str(), btn_brown);
	int hover = Globals::g_pTM->LoadTexture((GetPath() + "btn-hover.png").c_str(), btn_brown);
	int down  = Globals::g_pTM->LoadTexture((GetPath() + "btn-down.png").c_str(), btn_brown);
	for (int i = 0; i < BN_NUM_BUTTONS; ++i)
		Globals::g_pHUD->GetButtonArray()[i].SetIDs(up, hover, down);
}

//////////////////////////////////////////////////////////////////////////
// MAP Assets
//////////////////////////////////////////////////////////////////////////

CAssetsMap::CAssetsMap() : m_nCity(0), m_nCastle(0)
{
	SetPath("Resources/Map/");
}
// TODO::not sure if all these will be loaded up front yet...
const int CAssetsMap::LoadAssets(int index)
{
	m_nCity		= Globals::g_pTM->LoadTexture((GetPath() + "cities.png").c_str(), white);
	m_nCastle	= Globals::g_pTM->LoadTexture((GetPath() + "castles.png").c_str(), white);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// UNIT Assets
//////////////////////////////////////////////////////////////////////////

CAssetsUnits::CAssetsUnits()
{
	SetPath("Resources/Objects/Units/");
	m_nUnmountedKnightAttacks = -1;
}
const int CAssetsUnits::LoadAssets(int index)
{
	// get all the folders in the "Resources/Units/" directory
	vector<string> folders;
	Utilities::GetFoldersInDirectory(GetPath(), folders, &string("XML")); // ignore the XML folder, that's for the abilitiesManager
	vector<string>::iterator iter, end;

	for (iter = folders.begin(), end = folders.end(); iter != end; ++iter)
	{
		string unitFolder = GetPath() + (*iter);

		// now get all the files in the current unit folder
		vector<string> files; 
		string currDir = GetPath() + (*iter) + "/";
		Utilities::GetFilesInDirectory(currDir, files);
		// format of file name:	 --- example "UMKnight_Attack_12_128.png"
		// UMKnight = unit name
		// Attack	= anim name
		// 12		= number of frames in anim
		// 128		= size of a frame

		vector<string>::iterator fileIter, fileEnd;
		// load unit's info (just attack for now)
		for ( fileIter = files.begin(), fileEnd = files.end(); fileIter != fileEnd; ++fileIter)
		{
			string unitName = "", animName = "";
			int numFrames = 0, frameSize = 0;
			string name = (*fileIter);
			GetAnimPropsFromFileName(name, unitName, animName, numFrames, frameSize);
													// fix color key...
			int id = Globals::g_pTM->LoadTexture((currDir + name).c_str(), green[1]);

			AnimProperties vars(id, Globals::g_pTM->GetTextureWidth(id) / frameSize, numFrames, animName, point(frameSize, frameSize));
			Globals::g_pAnimManager->AddAnim(unitName, vars);
		}
	}

	return 0;	// TODO:: use return code?
}

// private
void CAssetsUnits::GetAnimPropsFromFileName( string name, string &unitName, string &animName, int &numFrames, int &frameSize )
{
	static const char underline = '_';
	static const char dot = '.';
	string temp = "";
	short index = 0;
	char currChar = name[index];
	short count = 0;
	while (currChar)
	{
		if ( currChar == underline || currChar == dot )
		{
			++count;
			if (count == 4)
				break;	// got all the info from the file name we need
			currChar = name[++index];
		}
		switch(count)
		{
		case UNIT_NAME: // count = 0
			{
				unitName += currChar;
			}break;
		case ANIM_NAME:
			{
				animName += currChar;
			}break;
		case NUM_FRAMES:
			{
				temp += currChar;
				if ( name[index + 1] == underline )
				{ 
					numFrames = atoi(temp.c_str());
					temp = "";
				}
			}break;
		case FRAME_SIZE:
			{
				temp += currChar;
				if ( name[index + 1] == dot )
				{
					frameSize = atoi(temp.c_str());
					temp = "";
				}
			}break;
		}
		currChar = name[++index];
	}
}

#include "stdafx.h"

#include "Assets.h"
#include "Globals.h"
#include "AnimationDefinesEnums.h"
#include "AnimationsManager.h"
#include "HUD.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// CAssets /////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CAssets::LoadNecessaryGUIElements()
{
	m_pGUI->LoadAssets();
}
void CAssets::LoadMapAssets()
{
	m_pMap->LoadAssets();
}
void CAssets::LoadUnitAssets(int index)
{
	m_pUNITS->LoadAssets(index);
}
void CAssets::LoadMenuSounds()
{
	m_pMenuSnds->LoadAssets();
}
void CAssets::LoadInGameGUI()
{
	m_pGUI->LoadInGameHUD();
}
void CAssets::LoadMenuAssets()
{
	m_pGUI->LoadMenuAssets();
}

//////////////////////////////////////////////////////////////////////////
void CAssets::LoadAllAssets()	// debug
{
	LoadNecessaryGUIElements();
	LoadMapAssets();
	LoadUnitAssets();
	LoadMenuSounds();
}
//////////////////////////////////////////////////////////////////////////

void CAssets::Init()
{
	m_pGUI		= new CAssetsGUI();
	m_pMap		= new CAssetsMap();
	m_pUNITS	= new CAssetsUnits();
	m_pMenuSnds = new CAssetsMenuSnds();
	LoadNecessaryGUIElements();
}
void CAssets::Shutdown()
{
	SAFE_DELETE(m_pUNITS);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pGUI);
	SAFE_DELETE(m_pMenuSnds);
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
void CAssetsGUI::LoadAssets(int index)
{
	m_nMouse				= Globals::g_pTM->LoadTexture((GetPath() + "mouse.png").c_str());
	m_nBlackPixel			= Globals::g_pTM->LoadTexture((GetPath() + "BlackPixel.png").c_str());
}
// private
void CAssetsGUI::LoadInGameHUD()
{
	m_nAbilityImages		= Globals::g_pTM->LoadTexture((GetPath() + "AbilityImages.png").c_str());
	m_nQBFrame				= Globals::g_pTM->LoadTexture((GetPath() + "QuickBar.png").c_str());
	m_nHUD					= Globals::g_pTM->LoadTexture((GetPath() + "HUD.png").c_str(),			white);
	m_nHUDFrame				= Globals::g_pTM->LoadTexture((GetPath() + "HUD_Frame.png").c_str(),	white);
	m_nPlayerInfoBG			= Globals::g_pTM->LoadTexture((GetPath() + "playerInfoBG.png").c_str(), white);
	m_nEnemyInfoBG			= Globals::g_pTM->LoadTexture((GetPath() + "enemyInfoBG.png").c_str(),	white);
	m_nWindowFrame			= Globals::g_pTM->LoadTexture((GetPath() + "Window.png").c_str(), white);

	m_arrFactions[FAC_LOGRESS] = Globals::g_pTM->LoadTexture((GetPath() + "faction-1.png").c_str(), white);
	m_arrFactions[FAC_PRYDAIN] = Globals::g_pTM->LoadTexture((GetPath() + "faction-2.png").c_str(), white);

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

// private
void CAssetsGUI::LoadMenuAssets()
{
	m_arrMenuBGs[MOT_MAIN]	= Globals::g_pTM->LoadTexture((GetPath() + "BGS/MainMenu_BG.png").c_str());
	m_arrMenuBGs[MOT_OPTIONS]=Globals::g_pTM->LoadTexture((GetPath() + "BGS/Harp_and_Title.png").c_str());
	m_arrMenuBGs[MOT_HELP]	= Globals::g_pTM->LoadTexture((GetPath() + "BGS/Celtic_Cross_BG.png").c_str());
	m_nMainMenuGlows		= Globals::g_pTM->LoadTexture((GetPath() + "BGS/MainMenu_BG_glows.png").c_str());
	m_nHarp					= Globals::g_pTM->LoadTexture((GetPath() + "BGS/Harp_and_Title.png").c_str());
	m_nSOATitle				= Globals::g_pTM->LoadTexture((GetPath() + "BGS/Harp_and_Title.png").c_str());
}

//////////////////////////////////////////////////////////////////////////
// MAP Assets
//////////////////////////////////////////////////////////////////////////

CAssetsMap::CAssetsMap() : m_nCity(0), m_nCastle(0)
{
	SetPath("Resources/Map/");
}
// TODO::not sure if all these will be loaded up front yet...
void CAssetsMap::LoadAssets(int index)
{
	m_nCity		= Globals::g_pTM->LoadTexture((GetPath() + "cities.png").c_str(), white);
	m_nCastle	= Globals::g_pTM->LoadTexture((GetPath() + "castles.png").c_str(), white);
}

//////////////////////////////////////////////////////////////////////////
// UNIT Assets
//////////////////////////////////////////////////////////////////////////

CAssetsUnits::CAssetsUnits()
{
	SetPath("Resources/Objects/Units/");
	m_nUnmountedKnightAttacks = -1;
}
void CAssetsUnits::LoadAssets(int index)
{
	// get all the folders in the "Resources/Units/" directory
	vector<string> folders; vector<string> toIgnore;
	toIgnore.push_back(string("XML"));
	toIgnore.push_back(string(".svn"));
	Utilities::GetFoldersInDirectory(GetPath(), folders, &toIgnore); // ignore the XML folder, that's for the abilitiesManager
	vector<string>::iterator iter, end;

	for (iter = folders.begin(), end = folders.end(); iter != end; ++iter)
	{
		string unitFolder = GetPath() + (*iter);

		// now get all the files in the current unit folder
		vector<string> files; 
		string currDir = GetPath() + (*iter) + "/";
		toIgnore.clear();
		toIgnore.push_back(string("svn"));
		Utilities::GetFilesInDirectory(currDir, files, &toIgnore);
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
}

//////////////////////////////////////////////////////////////////////////
//	Menu Sounds															//
//////////////////////////////////////////////////////////////////////////

CAssetsMenuSnds::CAssetsMenuSnds()
{
	SetPath("Resources/Music/Menus/");
}

void CAssetsMenuSnds::LoadAssets(int index)
{
	m_arrMenuMusic[MOT_MAIN]		= Globals::g_pFMOD->LoadSound((GetPath() + "DarkIsland.mp3").c_str(), FMOD_LOOP_NORMAL);
	m_arrMenuHoverSnd[MOT_HELP]		= Globals::g_pFMOD->LoadSound((GetPath() + "magic_swirl.mp3").c_str());
	m_arrMenuHoverSnd[MOT_OPTIONS]	= Globals::g_pFMOD->LoadSound((GetPath() + "harp.mp3").c_str());
	m_arrMenuHoverSnd[MOT_PLAY] 	= Globals::g_pFMOD->LoadSound((GetPath() + "horse_whinny.mp3").c_str());
	m_arrMenuHoverSnd[MOT_EXIT] 	= Globals::g_pFMOD->LoadSound((GetPath() + "fire_crackle.mp3").c_str());
	m_nMenuClickSnd					= Globals::g_pFMOD->LoadSound((GetPath() + "option_click.mp3").c_str());
	m_nInGameBtnInvalid				= Globals::g_pFMOD->LoadSound((GetPath() + "btn_invalid.mp3").c_str());
	m_nInGameBtnClick				= Globals::g_pFMOD->LoadSound((GetPath() + "Btn_ingame.mp3").c_str());
}

//////////////////////////////////////////////////////////////////////////

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

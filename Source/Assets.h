//////////////////////////////////////////////////////////////////////////
//	File	:	Assets.h
//
//	purpose	:	this will be used to load assets. The goal is to have assets
//				that are quick to load loaded up front and done for good.
//				other assets such as units, which have tons of frames of anims,
//				will be able to be loaded the first time they are used
//				
//////////////////////////////////////////////////////////////////////////
//	Class: BaseAsset
//				The BaseAsset class will be responsible for setting up the generic
//				variables such as various color keys.
//////////////////////////////////////////////////////////////////////////
#ifndef ASSETS_H
#define ASSETS_H

#include "HUD.h"

class BaseAsset
{
	string m_strPath;

	BaseAsset(const BaseAsset&);
	BaseAsset& operator=(const BaseAsset&);

protected:
	DWORD white;
	DWORD btn_brown;
	DWORD green[2];	// for differing key colors

	BaseAsset() 
	{
		white = 0xffffffff;
		btn_brown = D3DCOLOR_XRGB(116, 64, 9);
		green[0] = D3DCOLOR_XRGB(66, 93, 41); green[1] = D3DCOLOR_XRGB(64, 92, 40);
	}
public:
	virtual ~BaseAsset() = 0 {}

	// set up to return NULL if assets are already loaded,
	// otherwise the array is populated with the proper image IDs
	virtual void LoadAssets(int index = -1) = 0;

	void SetPath(string path) {m_strPath = path;}
	string GetPath() const {return m_strPath;}
};

//////////////////////////////////////////////////////////////////////////
/////////////////////////////// CAssetsGUI ///////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "MenuDefines.h"

class CAssetsGUI : BaseAsset
{
	enum {FAC_LOGRESS, FAC_PRYDAIN, NUM_FACTIONS};

	friend class CAssets;
	
	int m_nBlackPixel;
	int m_nMouse;
	int m_nHUD;
	int m_nHUDFrame;
	int m_nQBFrame;
	int m_nAbilityImages;
	int m_nPlayerInfoBG;
	int m_nEnemyInfoBG;
	int m_nWindowFrame;
	int m_arrFactions[NUM_FACTIONS];
	int m_arrMenuBGs[NUM_MENUS];
	int m_nMainMenuGlows;
	int m_nHarp;
	int m_nSOATitle;

	CAssetsGUI();
	// TODO::not sure if all these will be loaded up front yet...
	void LoadAssets(int index = -1);
	void SetHUDButtons();
	void LoadInGameHUD();
	void LoadMenuAssets();

public:
	~CAssetsGUI() {}

	//////////////////////////////////////////////////////////////////////////
	// accessors
	inline int Mouse() const		{ return m_nMouse;			}
	inline int HUD()   const		{ return m_nHUD;			}
	inline int Frame() const		{ return m_nHUDFrame;		}
	inline int PlayerInfoBG() const	{ return m_nPlayerInfoBG;	}
	inline int EnemyInfoBG() const	{ return m_nEnemyInfoBG;	}
	inline int FactionImages(int factionID)	const	{ return m_arrFactions[factionID];	}
	inline int BlackPixel()	const	{ return m_nBlackPixel;		}
	inline int QuickBar() const		{ return m_nQBFrame;		}
	inline int AbilityImages() const{ return m_nAbilityImages;	}
	inline int WindowFrame() const  { return m_nWindowFrame;	}
	inline int MenuBGs(eMenuOptionType menu) const	{ return m_arrMenuBGs[menu];	}
	inline int MenuGlows() const	{ return m_nMainMenuGlows;	}
	inline int Harp() const			{ return m_nHarp;			}
	inline int SOATitle() const		{ return m_nSOATitle;		}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////// CAssetsMap //////////////////////////////////
//////////////////////////////////////////////////////////////////////////

enum {CITY_ORIENTAL = 0, CITY_HUT = 64, CITY_CELTIC = 128};	// tops (y coord) of each city type
class CAssetsMap : BaseAsset
{
	friend class CAssets;
	
	int m_nCity;
	int m_nCastle;

	CAssetsMap();
	// TODO::not sure if all these will be loaded up front yet...
	void LoadAssets(int index = -1);

public:
	~CAssetsMap()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// accessors
	int City() const { return m_nCity; }
	int Castle()const{ return m_nCastle;}
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////// CAssetsUnits ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CAssetsUnits : BaseAsset
{
	friend class CAssets;
	int m_nUnmountedKnightAttacks;

	CAssetsUnits();
	void LoadAssets(int index = -1);
	void GetAnimPropsFromFileName( string name, string &unitName, string &animName, int &numFrames, int &frameSize );

public:
	~CAssetsUnits() {}

	int UnmountedKnightAttacks()	const	{ return m_nUnmountedKnightAttacks;	}
};

class CAssetsMenuSnds : BaseAsset
{
	friend class CAssets;

	int m_arrMenuMusic[NUM_MENUS];
	int m_arrMenuHoverSnd[NUM_MENUOPTION_TYPES];
	int m_nMenuClickSnd;
	int m_nInGameBtnInvalid;
	int m_nInGameBtnClick;
	
	CAssetsMenuSnds();
	void LoadAssets(int index = -1);
	
public:
	~CAssetsMenuSnds() {}

	int MenuMusic(eMenuOptionType menu)		const	{ return m_arrMenuMusic[menu];		}
	int MenuHoverSnd(eMenuOptionType menu)	const	{ return m_arrMenuHoverSnd[menu];	}
	int MenuClickSnd()	const						{ return m_nMenuClickSnd;			}
	int BtnInvalidSnd()	const						{ return m_nInGameBtnInvalid;		}
	int BtnInGameSnd() const						{ return m_nInGameBtnClick;			}
};

//////////////////////////////////////////////////////////////////////////
//////////////////////// CAssets /////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class CAssets
{
	CAssetsGUI* m_pGUI;
	CAssetsMap* m_pMap;
	CAssetsUnits* m_pUNITS;
	CAssetsMenuSnds* m_pMenuSnds;

	CAssets()	: m_pGUI(NULL), m_pMap(NULL), m_pUNITS(NULL), m_pMenuSnds(NULL)
	{
	}
	~CAssets()
	{
	}
	CAssets (const CAssets&);
	CAssets& operator= (const CAssets&);
public:

	static CAssets* GetInstance() { static CAssets instance; return &instance;}

	void LoadNecessaryGUIElements();
	void LoadMapAssets();
	void LoadUnitAssets(int index = -1);
	void LoadMenuSounds();
	void LoadInGameGUI();	
	void LoadMenuAssets();

	//////////////////////////////////////////////////////////////////////////
	// should not be used except for debugging
	void LoadAllAssets();
	//////////////////////////////////////////////////////////////////////////

	void Init();
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// ACCESSORS
	inline const CAssetsGUI* const		GetGUIasts()	const { return m_pGUI;		}
	inline const CAssetsMap* const		GetMapasts()	const { return m_pMap;		}
	inline const CAssetsUnits* const	GetUnits()		const { return m_pUNITS;	}
	inline const CAssetsMenuSnds* const	GetMenuSnds()	const { return m_pMenuSnds;	}
};

#endif
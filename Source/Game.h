#ifndef GAME_H
#define GAME_H


#include "IGameState.h"
#include "MenuDefines.h"

class CPlayer;
class CTimer;
class CMenu;

class CGame
{
private:
	//pointers to other classes
	IGameState*			m_pCurrentState;
	CMenu*				m_pCurrMenu;
	string				m_sCurrProfName;

	// variables
	bool	m_bIsRunning;
	int		m_nScreenWidth;
	int		m_nScreenHeight;
	float	m_fSFXVolume;
	float	m_fMusicVolume;
	double	m_dTimeStep;
	//PLAYERINFO m_PlayerInfo;

	CTimer* m_pTimer;

	//Constructor
	CGame(void);
	//Copy Constructor
	CGame(const CGame&);
	//Assignment Operator
	CGame& operator=(const CGame&);
	//Destructor
	~CGame(void);


public:
	void ChangeMenu(eMenuType mt, bool exitingGameplay);
	///////////////////////////////////////////////
	//	Function:	ChangeState
	//
	//	Purpose:	Change the State of the Game
	///////////////////////////////////////////////
	void ChangeState(IGameState* pGameState);
	/////////////////////////////////////////////////////////////////
	//	Function:	Main
	//
	//	Purpose:	To run while the game runs from start to finish
	////////////////////////////////////////////////////////////////
	bool Main(const POINT& mouse);
	////////////////////////////////////////////////////////////////
	//	Function:	Shutdown
	//
	//	Purpose:	Clean up all memory 
	////////////////////////////////////////////////////////////////
	void Shutdown(void);
	////////////////////////////////////////////////////////////
	//	Function:	Initialize
	//
	//	Purpose:	Initialize the game
	////////////////////////////////////////////////////////////
	void Initialize(HWND hWnd, HINSTANCE hInstance, int nScreenWidth, int nScreenHeight, bool bIsWindowed);
	///////////////////////////////////////////////////////
	//	Function:	GetInstance
	//
	//	Purpose:	Gets the instance of CGame
	///////////////////////////////////////////////////////
	static CGame* GetInstance(void);
	///////////////////////////////////////////////////////
	//	Function:	LoadSettings
	//
	//	Purpose:	To load the game settings
	///////////////////////////////////////////////////////
	void LoadSettings(void);
	////////////////////////////////////////////////////////
	//	Function: GetFMODSystem
	//
	//	Purpose: Return the FMODSystem pointer
	////////////////////////////////////////////////////////
	//FMOD::System* GetFMODSystem(void);

	//static friend void MessageProc(CBaseMessage* pMsg);

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	inline bool			GetIsRunning()		const	{return m_bIsRunning;	}
	inline int			GetScreenWidth ()	const	{return m_nScreenWidth;	}
 	inline int			GetScreenHeight ()	const	{return m_nScreenHeight;}
	inline float		GetSFXVolume()		const	{return m_fSFXVolume;	}
	inline float		GetMusicVolume()	const	{return m_fMusicVolume;	}
	inline string		GetProfName()		const	{return m_sCurrProfName;}
	inline CMenu* const GetCurrMenu()		const	{return m_pCurrMenu;	}
	inline const IGameState* const GetCurrState()	const	{return m_pCurrentState;}
	// 	bool GetIsPaused()			{return m_bIsPaused;}

	//////////////////////////////////////////////////////////////////////////
	// Mutators
	//////////////////////////////////////////////////////////////////////////
	inline void SetSFXVolume	(float _nSFXVolume);
	inline void SetMusicVolume	(float _nMusicVolume);
	inline void SetIsRunning	(bool _bIsRunning)		{m_bIsRunning = _bIsRunning;}
	inline void SetProfName		(string& profName)		{m_sCurrProfName = profName;}
	inline void SetCurrMenu		(CMenu* const menu)		{m_pCurrMenu = menu;		}
	//	void SetIsPaused	()					{m_bIsPaused = !m_bIsPaused;}
	// 	void SetLastScore	(int _nLastScore)		{m_nLastScore = _nLastScore;}
};

#endif
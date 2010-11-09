#pragma once

class MenuEditorInterface;

class CApplication
{
	friend class MenuEditorInterface;

	// all app data relevant to project stored in Globals::
	CApplication(void) {}
	~CApplication(void) {}
	CApplication(CApplication&);
	CApplication& operator= (CApplication&);

	// return false if exiting
	bool UpdateAll(float fDeltaTime);
	bool MainInGame(float fDeltaTime);

public:

	static CApplication* GetInstance();

	// initialize (standalone, i.e. editor is running apart from the game)
	void Init(HWND hwnd, HINSTANCE hInstance, size& screenSize, bool windowed, bool vSync);
	
	void Shutdown();

	// returns false if exiting application
	bool Main();

};

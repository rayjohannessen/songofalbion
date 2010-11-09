#pragma once
#include "imenu.h"

class CMainMenu : public IMenu
{
public:
	CMainMenu(void);
	virtual ~CMainMenu(void);

	void Render();
	void Update(float fElapsed);

	void Save(const string& file);
	void Load(const string& file);
	
};

#include "stdafx.h"

#include "MenuFunctions.h"
#include "Menu.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "BitmapFont.h"
#include "Globals.h"

namespace HelpMenu
{
	void Render(CGameMenu* const menu)
	{

	}

	void Update(double fElapsed, CGameMenu* const menu)
	{

	}

	bool Input(double fElapsed, const POINT& mouse, CGameMenu* const menu)
	{

		return true;
	}
}

namespace OptionMenu
{
	void Render(CGameMenu* const menu)
	{
		MenuOptIter iter, end;
	}

	void Update(double fElapsed, CGameMenu* const menu)
	{

	}

	bool Input(double fElapsed, const POINT& mouse, CGameMenu* const menu)
	{

		return true;
	}
}

namespace MainMenu
{
	void Render(CGameMenu* const menu)
	{
		MenuOptIter iter, end;
	}

	void Update(double fElapsed, CGameMenu* const menu)
	{

	}

	bool Input(double fElapsed, const POINT& mouse, CGameMenu* const menu)
	{

		return true;
	}
}
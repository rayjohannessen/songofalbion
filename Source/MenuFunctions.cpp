#include "stdafx.h"

#include "MenuFunctions.h"
#include "Menu.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "BitmapFont.h"
#include "Globals.h"

namespace HelpMenu
{
	void Render(CMenu* const menu)
	{

	}

	void Update(double fElapsed, CMenu* const menu)
	{

	}

	bool Input(double fElapsed, const POINT& mouse, CMenu* const menu)
	{

		return true;
	}
}

namespace OptionMenu
{
	void Render(CMenu* const menu)
	{
		MenuOptIter iter, end;
	}

	void Update(double fElapsed, CMenu* const menu)
	{

	}

	bool Input(double fElapsed, const POINT& mouse, CMenu* const menu)
	{

		return true;
	}
}

namespace MainMenu
{
	void Render(CMenu* const menu)
	{
		MenuOptIter iter, end;
	}

	void Update(double fElapsed, CMenu* const menu)
	{

	}

	bool Input(double fElapsed, const POINT& mouse, CMenu* const menu)
	{

		return true;
	}
}
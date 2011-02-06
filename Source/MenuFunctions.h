////////////////////////////////////////////////////////////////////////////////////////////
//
//  Author:		Ramon Johannessen
//
//  Purpose:	Define menu functions, for performing additional functionality
//
////////////////////////////////////////////////////////////////////////////////////////////

#include "Wrappers/CSGD_TextureManager.h"
#include "Wrappers/CSGD_DirectInput.h"
#include "Globals.h"

namespace HelpMenu
{
	void Render(CGameMenu* const menu);

	void Update(double fElapsed, CGameMenu* const menu);

	bool Input(double fElapsed, const POINT& mouse, CGameMenu* const menu);
}

namespace OptionMenu
{
	void Render(CGameMenu* const menu);

	void Update(double fElapsed, CGameMenu* const menu);

	bool Input(double fElapsed, const POINT& mouse, CGameMenu* const menu);
}

namespace MainMenu
{
	void Render(CGameMenu* const menu);

	void Update(double fElapsed, CGameMenu* const menu);

	bool Input(double fElapsed, const POINT& mouse, CGameMenu* const menu);
}
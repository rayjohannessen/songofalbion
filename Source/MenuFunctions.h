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
	void Render(CMenu* const menu);

	void Update(double fElapsed, CMenu* const menu);

	bool Input(double fElapsed, const POINT& mouse, CMenu* const menu);
}

namespace OptionMenu
{
	void Render(CMenu* const menu);

	void Update(double fElapsed, CMenu* const menu);

	bool Input(double fElapsed, const POINT& mouse, CMenu* const menu);
}

namespace MainMenu
{
	void Render(CMenu* const menu);

	void Update(double fElapsed, CMenu* const menu);

	bool Input(double fElapsed, const POINT& mouse, CMenu* const menu);
}
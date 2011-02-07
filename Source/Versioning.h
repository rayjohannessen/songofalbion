#pragma once
//////////////////////////////////////////////////////////////////////////
// Versioning - meant to be used with Versioning Controlled Build (VCB)
//					which is an ad-on available for Visual Studio (I know it works for 2008)
//////////////////////////////////////////////////////////////////////////


class Versioning
{
	char	m_szVersion[64];

public:

	//////////////////////////////////////////////////////////////////////////
	// SetupVersion - does all the initial work - only needs to be called once
	//
	// _filePath - the path to where the version info is stored (the executable)
	// _preversionString - what the version is for - e.g. "Game Version"
	//////////////////////////////////////////////////////////////////////////
	const char* SetupVersion(LPTSTR _filePath, string _preversionString);

	const char* GetVersionString() const { return m_szVersion; }
};

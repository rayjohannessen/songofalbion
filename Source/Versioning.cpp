#include <Stdafx.h>
#include "Versioning.h"

#pragma comment(lib, "version.lib")

const char* Versioning::SetupVersion(LPTSTR _filePath, string _preversionString)
{
	LPTSTR lpszFilePath = _filePath; 
	DWORD dwDummy; 
	DWORD dwFVISize = GetFileVersionInfoSize( lpszFilePath , &dwDummy ); 
	LPBYTE lpVersionInfo = new BYTE[dwFVISize]; 
	GetFileVersionInfo( lpszFilePath , 0 , dwFVISize , lpVersionInfo ); 
	UINT uLen; 
	VS_FIXEDFILEINFO *lpFfi; 
	VerQueryValue( lpVersionInfo , ("\\") , (LPVOID *)&lpFfi , &uLen ); 
	DWORD dwFileVersionMS = lpFfi->dwFileVersionMS; 
	DWORD dwFileVersionLS = lpFfi->dwFileVersionLS; 
	delete [] lpVersionInfo; 
	DWORD dwLeftMost = HIWORD(dwFileVersionMS); 
	DWORD dwSecondLeft = LOWORD(dwFileVersionMS); 
	DWORD dwSecondRight = HIWORD(dwFileVersionLS); 
	DWORD dwRightMost = LOWORD(dwFileVersionLS); 

	sprintf_s( m_szVersion, "%s%d.%d.%d.%d" , _preversionString.c_str(), dwLeftMost, dwSecondLeft, dwSecondRight, dwRightMost ); 
	return m_szVersion;
}


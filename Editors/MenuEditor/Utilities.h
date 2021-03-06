#pragma once

namespace Utilities
{
	//	Macro to safely Release COM Objects.
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = NULL; }
#endif

	//	Macro to safely delete pointers.
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			if (p) { delete p; p = NULL; }
#endif

	//	Macro to safely delete array pointers.
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	if (p) { delete [] p; p = NULL; }
#endif

	//	Macro for reporting Wrapper errors
#ifndef DXERROR
#define DXERROR(a)	{ OutputDebugString(a); OutputDebugString("\n"); return false; }
#endif

void GetFilesInDirectory(const string& _dir, vector<string>& _files, vector<string>* extToIgnore = NULL);
void GetFoldersInDirectory(const string& _dir, vector<string>& _folders, vector<string>* folderToIgnore = NULL);

void Error(LPTSTR lpszFunction);

}
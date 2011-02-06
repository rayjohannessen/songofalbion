#include "stdafx.h"

#include "Utilities.h"

namespace Utilities
{
	string GetExtension(const string& fileName)
	{
		string ext = "";
		int ind = 0;
		while (fileName[ind++] != '.') {if (fileName[ind] == '\0') return ext;}
		for (;fileName[ind] != '\0';)
			ext += fileName[ind++];
		return ext;
	}
	void GetFilesInDirectory(const string& _dir, vector<string>& _files, vector<string>* extToIgnore )
	{
		WIN32_FIND_DATA data;
		char dir[MAX_PATH + 1];
		if (_dir[_dir.length()] == '\\')
			sprintf_s(dir, MAX_PATH + 1, "%s*", _dir.c_str());
		else
			sprintf_s(dir, MAX_PATH + 1, "%s\\*", _dir.c_str());

		HANDLE h = FindFirstFile(dir, &data);
		if (h == NULL)
			return;

		string name;
		if (extToIgnore)
		{
			while (FindNextFile(h, &data))
			{
				name = data.cFileName;
				bool ignore = false;
				for (unsigned i = 0; i < extToIgnore->size(); ++i)
				{
					if ((*extToIgnore)[i] == GetExtension(name))
					{ ignore = true; break; }
				}
				if (name != "Thumbs.db" && name != ".." && !ignore)
						_files.push_back(data.cFileName);
			}
		} 
		else
		{
			while (FindNextFile(h, &data))
			{
				name = data.cFileName;
				if (name != "Thumbs.db" && name != "..")
					_files.push_back(data.cFileName);	
			}
		}

		FindClose(h);
	}

	void GetFoldersInDirectory(const string& _dir, vector<string>& _folders, vector<string>* folderToIgnore )
	{
		WIN32_FIND_DATA data;
		char dir[MAX_PATH + 1];
		if (_dir[_dir.length()] == '\\')
			sprintf_s(dir, MAX_PATH + 1, "%s*", _dir.c_str());
		else
			sprintf_s(dir, MAX_PATH + 1, "%s\\*", _dir.c_str());

		HANDLE h = FindFirstFile(dir, &data);
		if (h == NULL)
			return;

		string name;
		if (folderToIgnore)
		{
			while (FindNextFile(h, &data))
			{
				name = data.cFileName;
				bool ignore = false;
				for (unsigned i = 0; i < folderToIgnore->size(); ++i)
				{
					if ((*folderToIgnore)[i] == name)
					{ ignore = true; break; }
				}
				if (name != "Thumbs.db" && name != ".." && !ignore)
					_folders.push_back(data.cFileName);
			}
		} 
		else
		{
			while (FindNextFile(h, &data))
			{
				name = data.cFileName;
				if (name != "Thumbs.db" && name != "..")
					_folders.push_back(data.cFileName);	
			}
		}

		FindClose(h);
	}

#include <strsafe.h>

	void Error(LPTSTR lpszFunction) 
	{ 
		// Retrieve the system error message for the last-error code

		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError(); 
		if (dw == 0)
			return;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
			(lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
		StringCchPrintf((LPTSTR)lpDisplayBuf, 
			LocalSize(lpDisplayBuf),
			TEXT("%s failed with error %d: %s"), 
			lpszFunction, dw, lpMsgBuf); 
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
		SetLastError(0);
		/*	ExitProcess(dw); */
	}
}
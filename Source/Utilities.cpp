#include "stdafx.h"

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
	void GetFilesInDirectory(const string& _dir, vector<string>& _list, string* extToIgnore )
	{
		string toIgnore = "bad";
		if(extToIgnore)
			toIgnore = *extToIgnore;

		WIN32_FIND_DATA data;
		char dir[MAX_PATH + 1];
		if (_dir[_dir.length()] == '\\')
			sprintf_s(dir, MAX_PATH + 1, "%s*", _dir.c_str());
		else
			sprintf_s(dir, MAX_PATH + 1, "%s\\*", _dir.c_str());

		HANDLE h = FindFirstFile(dir, &data);
		if (h == NULL)
			return;

		while (FindNextFile(h, &data))
		{
			string name = data.cFileName;
			if (name != "Thumbs.db" && name != ".." && GetExtension(name) != toIgnore)
					_list.push_back(data.cFileName);
		}

		FindClose(h);
	}

	void GetFoldersInDirectory(const string& _dir, vector<string>& _list, string* folderToIgnore )
	{
		string toIgnore = "bad";
		if(folderToIgnore)
			toIgnore = *folderToIgnore;

		WIN32_FIND_DATA data;
		char dir[MAX_PATH + 1];
		if (_dir[_dir.length()] == '\\')
			sprintf_s(dir, MAX_PATH + 1, "%s*", _dir.c_str());
		else
			sprintf_s(dir, MAX_PATH + 1, "%s\\*", _dir.c_str());

		HANDLE h = FindFirstFile(dir, &data);
		if (h == NULL)
			return;

		while (FindNextFile(h, &data))
		{
			string name = data.cFileName;
			if (name != "Thumbs.db" && name != ".." && name != toIgnore)
				_list.push_back(data.cFileName);
		}

		FindClose(h);
	}
}
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
}
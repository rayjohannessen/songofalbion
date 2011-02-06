#pragma once

#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#define _AFXDLL
#include <afxwin.h>

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <list>
using std::list;
#include <queue>
using std::queue;

#include <fstream>
using std::ifstream;

#include ".\Wrappers\CSGD_Direct3D.h"
#include ".\Wrappers\CSGD_DirectInput.h"
#include ".\Wrappers\CSGD_TextureManager.h"

#include "Structs.h"
#include "Globals.h"
#include "Defines.h"
#include "Utilities.h"

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>
using std::vector;
#include <list>
using std::list;
#include <map>
using std::map;
#include <utility>
using std::pair;
#include <string>
using std::string;
#include <fstream>
using namespace std;
#include <deque>

#include <d3d9.h>
#include <cmath>
#include "Structs.h"

//////////////////////////////////////////////////////////////////////////
// wrappers
#include "Wrappers/CSGD_DirectInput.h"
#include "Wrappers/CSGD_Direct3D.h"
#include "Wrappers/CSGD_TextureManager.h"
#include "Wrappers/CSGD_FModManager.h"
#include "Wrappers/CSGD_Flags.h"

// debug
#ifdef _DEBUG
	#include "DebugWindow.h"
#endif // _DEBUG

// defines
#include "Defines.h"
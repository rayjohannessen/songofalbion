//*****************************************************************************//
// Filename:	DebugWindow.h												//
// Last Modified:	1/22/10		Ramon Johannessen							//
// Comment:	Provides functionality to easily add and use					//
//			a console window for debugging
//*****************************************************************************//
#pragma once

#include <d3dx9.h>
#include <string>
using std::string;

#define MAX_STRING 512

class DebugWindow
{
	DebugWindow(void);
	DebugWindow(DebugWindow&);
	~DebugWindow(void);

public:
	
	static DebugWindow* GetInstance();

	void Init();
	void Shutdown();
	void Clear();

	//////////////////////////////////////////////////////////////////////////

	void DebugString(const string _text);
	void DebugString(const char* _text);

	//////////////////////////////////////////////////////////////////////////

	void DebugVec3(const D3DXVECTOR3& _vec, const string* _vecName = NULL);
	void DebugFloat(float _float, const string* _floatName = NULL);
	void DebugInt(int _int, const string* _intName = NULL);
	void DebugPoint(const point& _pt, const string* _pointName = NULL);
	void DebugPointf(const pointf& _pt, const string* _pointName = NULL);
};

static DebugWindow* DebugWnd = DebugWindow::GetInstance();

//*****************************************************************************//
// Filename:	DebugWindow.h												//
// Last Modified:	1/22/10													//
// Comment:	Provides functionality to easily add and use					//
//			a console window for debugging
//*****************************************************************************//
#include "stdafx.h"

#include "DebugWindow.h"

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <tchar.h>
using namespace std;

DebugWindow::DebugWindow(void)
{
}

DebugWindow::~DebugWindow(void)
{
}

DebugWindow* DebugWindow::GetInstance()
{
	static DebugWindow instance;
	return &instance;
}

//////////////////////////////////////////////////////////////////////////
void DebugWindow::DebugVoidPtr(void* data)
{
	cout << data << endl;
}
void DebugWindow::DebugString(const std::string _text)
{
	cout << _text.c_str() << endl << endl;
}
void DebugWindow::DebugString(const char* _text)
{
	cout << _text << "\n\n";
}

//////////////////////////////////////////////////////////////////////////

void DebugWindow::DebugVec3(const D3DXVECTOR3& _vec, const string* _vecName /* = NULL */)
{
	char buff[MAX_STRING];
	if (_vecName)
		sprintf_s(buff, "%s =\n  X:%.6f\n  Y:%.6f\n  Z:%.6f", _vecName->c_str(), _vec.x, _vec.y, _vec.z);
	else
		sprintf_s(buff, "D3DXVECTOR3 = X:%.6f, Y:%.6f, Z:%.6f", _vec.x, _vec.y, _vec.z);

	DebugString(buff);
}

void DebugWindow::DebugFloat(float _float, const string* _floatName /*= NULL*/)
{
	char buff[MAX_STRING];
	if (_floatName)
		sprintf_s(buff, "%s = %.6ff", _floatName->c_str(), _float);
	else
		sprintf_s(buff, "float = %.6ff", _float);

	DebugString(buff);
}
void DebugWindow::DebugInt(int _int, const string* _intName /*= NULL*/)
{
	char buff[MAX_STRING];
	if (_intName)
		sprintf_s(buff, "%s = %i", _intName->c_str(), _int);
	else
		sprintf_s(buff, "int = %i", _int);

	DebugString(buff);
}

void DebugWindow::DebugPoint(const point& _pt, const string* _pointName /*= NULL*/)
{
	char buff[MAX_STRING];
	if (_pointName)
		sprintf_s(buff, "%s = %i, %i", _pointName->c_str(), _pt.x, _pt.y);
	else
		sprintf_s(buff, "point = %i, %i", _pt.x, _pt.y);

	DebugString(buff);
}

void DebugWindow::DebugPointf(const pointf& _pt, const string* _pointName /*= NULL*/)
{
	char buff[MAX_STRING];
	if (_pointName)
		sprintf_s(buff, "%s = %.2f, %.2f", _pointName->c_str(), _pt.x, _pt.y);
	else
		sprintf_s(buff, "point = %.2f, %.2f", _pt.x, _pt.y);

	DebugString(buff);
}
//////////////////////////////////////////////////////////////////////////

void DebugWindow::Init()
{
	AllocConsole();
	*stdout = *_tfdopen(_open_osfhandle((intptr_t) GetStdHandle(STD_OUTPUT_HANDLE), _O_WRONLY), _T("a"));
}
void DebugWindow::Shutdown()
{
	FreeConsole();
}

void DebugWindow::Clear()
{
	system("cls");
}

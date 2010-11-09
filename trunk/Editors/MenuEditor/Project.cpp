#include "StdAfx.h"
#include "Project.h"


CProject::CProject(void) :
	m_ClearClr(RAND_INT(0, 255), RAND_INT(0, 255), RAND_INT(0, 255))
{
}

CProject::~CProject(void)
{
}

//////////////////////////////////////////////////////////////////////////

void CProject::Render()
{

}
void CProject::Update(float fElapsed)
{

}

//////////////////////////////////////////////////////////////////////////

bool Load(const string& file)
{
	// load clear colors

	return true;
}
bool Save(const string& file)
{
	return true;
}
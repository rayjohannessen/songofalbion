#pragma once

//////////////////////////////////////////////////////////////////////////
//	Interface for all menu types
//////////////////////////////////////////////////////////////////////////

class IMenu
{
public:
	IMenu(void) {}
	virtual ~IMenu(void) = 0 {}

	virtual void Render() = 0 {}
	virtual void Update(float fElapsed) = 0 {}

	virtual void Save(const string& file) = 0 {}
	virtual void Load(const string& file) = 0 {}
};

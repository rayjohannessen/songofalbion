#pragma once

//////////////////////////////////////////////////////////////////////////
//	Defines a new menu project
//////////////////////////////////////////////////////////////////////////

class IMenu;

class CProject
{
	struct ClearClr 
	{
		unsigned char R, G, B; // clear color
		ClearClr(unsigned char r, unsigned char g, unsigned char b) : R(r), G(g), B(b) {}
	};
	ClearClr		m_ClearClr;
	vector<IMenu*>	m_vMenus;

public:
	CProject(void);
	~CProject(void);

	void Render();
	void Update(float fElapsed);

	bool Load(const string& file);
	bool Save(const string& file);

	//////////////////////////////////////////////////////////////////////////
	// ACCESSORS
	ClearClr GetClearClr() const { return m_ClearClr; }
};

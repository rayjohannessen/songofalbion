#pragma once

#include "ButtonDefines.h"
#include "Timer.h"

class CUIWindowBase;

class CButton
{
	int				m_nState;	// corresponds to the image ID of the current state
	eButtonName		m_eName;
	int				m_nImageIDup, m_nImageIDhover, m_nImageIDdown;
	ButtonActionFP  m_ActionFunc;
	string			m_strText;
	rect			m_Rect;
	CTriggerTimer	m_Timer;

public:
	~CButton() {}
	CButton() : m_eName(BN_NUM_BUTTONS), m_nImageIDup(-1), m_ActionFunc(NULL), m_nState(-1) {}

	void Update(double dTimeStep);
	// returns true if mouse is inside button (no need to check input for any other buttons then)
	bool Input(const POINT& mouse, CUIWindowBase*& window);
	void Render();

	// ACCESSORS
	inline int GetUpID()				{ return m_nImageIDup;	}
	inline eButtonName GetButtonName()	{ return m_eName;		}
	inline const point GetLocation()	{ return point(m_Rect.left, m_Rect.top); }
	inline const rect& GetRect()		{ return m_Rect; }

	// MUTATORS
	inline void SetVariables(eButtonName name, const ButtonActionFP fp, const string& text)
	{
		m_eName = name; m_ActionFunc = fp; m_strText = text;
	}
	inline void SetRect(const rect& r) { m_Rect = r; }
	inline void SetIDs(int up, int hover, int down) { m_nImageIDup = up; m_nImageIDhover = hover; m_nImageIDdown = down; m_nState = up; }
	inline void SetStateToUp()	{ m_nState = m_nImageIDup; }
	CUIWindowBase* SimulatePressed();
};
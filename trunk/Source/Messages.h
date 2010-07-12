#pragma once

typedef int MSGID;

class CBaseMessage
{
private:
	MSGID	m_msgID;

public:
	CBaseMessage(MSGID msgID)
	{
		m_msgID = msgID;
	}

	virtual ~CBaseMessage(void) {}

	inline MSGID GetID(void)	{ return m_msgID; }
};
////////////////////////////////////////////////////////
//   File Name	:	"MessageSystem.h"
//
//   Author		:	Ray Johannessen
//
//   Purpose	:	Handles sending and recieving of messages
////////////////////////////////////////////////////////

#ifndef MESSAGESYSTEM_H
#define MESSAGESYSTEM_H

#include "Messages.h"
#include <queue>
using std::queue;

typedef void (*MESSAGEPROC)(CBaseMessage*);

class MessageSystem
{
private:
	queue<CBaseMessage*>			m_qMsgQueue;		//	Stores my messages.
	MESSAGEPROC						m_pMsgProc;			//	Points to user defined function.

	MessageSystem() { m_pMsgProc = NULL;	}
	MessageSystem(const MessageSystem&);
	MessageSystem& operator=(const MessageSystem&);

	~MessageSystem() {}
public:
	static	MessageSystem* GetInstance(void);
	int		GetNumberMsgs(void) { return (int)m_qMsgQueue.size(); }
	void	InitMessageSystem(MESSAGEPROC pMsgProc);
	void	SendMsg(CBaseMessage* pMsg);
	void	ProcessMsgs(void);
	void	ClearMsgs(void);
	void	ShutdownMessageSystem(void);
	
};

#endif
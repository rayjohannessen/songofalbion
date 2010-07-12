////////////////////////////////////////////////////////
//   File Name	:	"MessageSystem.cpp"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	Handles sending and recieving of messages
////////////////////////////////////////////////////////
#include "stdafx.h"

#include "MessageSystem.h"


MessageSystem* MessageSystem::GetInstance(void)
{
	static MessageSystem instance;
	return &instance;
}

void MessageSystem::InitMessageSystem(MESSAGEPROC pfnMsgProc)
{
	if (!pfnMsgProc)	
		return;

	m_pMsgProc = pfnMsgProc;
}

void MessageSystem::SendMsg(CBaseMessage* pMsg)
{
	if (!pMsg)	
		return;

	m_qMsgQueue.push(pMsg);
}

void MessageSystem::ProcessMsgs()
{
	if(!m_pMsgProc)	
		return;

	while(!m_qMsgQueue.empty())
	{
		m_pMsgProc(m_qMsgQueue.front());		
		delete m_qMsgQueue.front();				
		m_qMsgQueue.pop();						
	}
}

void MessageSystem::ClearMsgs()
{
	while(!m_qMsgQueue.empty())
	{
		delete m_qMsgQueue.front();
		m_qMsgQueue.pop();
	}
}

void MessageSystem::ShutdownMessageSystem()
{
	ClearMsgs();
	m_pMsgProc = NULL;
}

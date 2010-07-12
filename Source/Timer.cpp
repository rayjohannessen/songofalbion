////////////////////////////////////////////////////////
//   File Name	:	"Timer.cpp"
//
//   Author		:	Ramon Johannessen (RJ)
//
//   Purpose	:	To provide an easy way to mark and use the passing of time
////////////////////////////////////////////////////////
#include "StdAfx.h"

#include "Timer.h"

//////////////////////////////////////////////////////////////////////////
///////////////////  CBaseTimer	//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CTimerBase::CTimerBase() :
	m_dFPS(0.0),
	m_dFPSElapsed(0.0),
	m_nFrame(0),
	m_dTotalTime(0.0)
{

}

void CTimerBase::UpdateBase(double dElapsed)
{
	++m_nFrame;
	m_dFPSElapsed += dElapsed;

	if (m_dFPSElapsed >= 1.0)	// if a second has passed, calculate the FPS
	{
		m_dFPS = m_nFrame;
		m_nFrame = 0;
		m_dFPSElapsed = 0.0;
	}
	m_dTotalTime += dElapsed;
}

double CTimerBase::GetTick() const
{
	LARGE_INTEGER curr; QueryPerformanceCounter(&curr);
	return double( (curr.QuadPart - m_liLastCount.QuadPart) / double(m_liCounterFreq.QuadPart));
}


//////////////////////////////////////////////////////////////////////////
////////////////////////// CTimer ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CTimer::CTimer() : CTimerBase()
{
	QueryPerformanceFrequency(&m_liCounterFreq);	
	QueryPerformanceCounter(&m_liLastCount);	// a high-precision timer
}

// this update assumes m_bAutoUpdate == true,
// returns time step
double CTimer::Update()
{
	LARGE_INTEGER curr; QueryPerformanceCounter(&curr);
	double elapsed = double( (curr.QuadPart - m_liLastCount.QuadPart) / double(m_liCounterFreq.QuadPart));

	UpdateBase(elapsed);
	m_liLastCount = curr;

	return elapsed;
}

//////////////////////////////////////////////////////////////////////////
////////////////////////// CTriggerTimer /////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CTriggerTimer::CTriggerTimer(bool autoUpdate, bool startTimer, double dEndTime) : 
	CTimerBase(),
	m_bAutoUpdate(autoUpdate), 
	m_dEndTime(dEndTime),
	m_bIsOn(startTimer), 
	m_dPauseDur(0.0), 
	m_dPauseTimer(0.0)
{
	if (autoUpdate)
	{
		QueryPerformanceFrequency(&m_liCounterFreq);	
		QueryPerformanceCounter(&m_liLastCount);	// a high-precision timer
	}
	ResetTimer(!startTimer);
}

bool CTriggerTimer::Update(double dElapsedTime)
{
	if (m_bIsOn)
	{
		if (m_bAutoUpdate)
		{
			LARGE_INTEGER curr; QueryPerformanceCounter(&curr);
			double elapsed = double( (curr.QuadPart - m_liLastCount.QuadPart) / double(m_liCounterFreq.QuadPart));
			UpdateBase(elapsed);
			m_liLastCount = curr;
		}
		else
		{
			UpdateBase(dElapsedTime);
		}
		if (m_dEndTime > 0.0 && m_dTotalTime >= m_dEndTime)
		{ ResetTimer(); return true; }
	}
	if (m_dPauseDur > 0.0)
	{
		if (m_bAutoUpdate)
		{
			LARGE_INTEGER _curr; QueryPerformanceCounter(&_curr);
			m_dPauseTimer += dElapsedTime;
			m_liLastCount = _curr;
		}
		else 
			m_dPauseTimer += dElapsedTime;
		if (m_dPauseTimer > m_dPauseDur)
		{m_bIsOn = true; m_dPauseTimer = m_dPauseDur = 0.0;}
	}
	return false;
}

void CTriggerTimer::StartTimer(double endTime, bool reset)
{
	if (reset)
		ResetTimer();
	m_bIsOn = true;
	if (endTime > 0.0)
		m_dEndTime = endTime;
}

void CTriggerTimer::ResetTimer(bool bStop)
{
	m_dTotalTime = m_dEndTime = m_dPauseDur = 0.0;
	m_bIsOn = !bStop;
	if (m_bAutoUpdate)
		QueryPerformanceCounter(&m_liLastCount);
}
#include "StdAfx.h"
#include "Timer.h"


CTimer::CTimer(double fps)
{
	m_dFPS=fps;
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_llPerformancePerSecond);
	m_dTimeScale=1.0f/m_llPerformancePerSecond;
	m_llFrameTime=(LONGLONG)m_llPerformancePerSecond/m_dFPS;
	m_dTimeElapsed=0;
	m_llLasttime=0;
	m_llNextTime=0;
}


CTimer::~CTimer(void)
{
}

void CTimer::Start()
{
	LONGLONG current;
	QueryPerformanceCounter((LARGE_INTEGER*)&current);
	m_llNextTime=current+m_llFrameTime;
	m_llLasttime=current;
}

bool CTimer::ReadyForNextTime()
{
	LONGLONG current;
	QueryPerformanceCounter((LARGE_INTEGER*)&current);
	if (current>m_llNextTime)
	{
		m_llNextTime=current+m_llFrameTime;
		m_dTimeElapsed=(current-m_llLasttime)*m_dTimeScale;
		m_llLasttime=current;
		return true;
	}
	return false;
}

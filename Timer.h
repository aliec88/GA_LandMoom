#pragma once
class CTimer
{
private:
	double m_dFPS;
	double m_dTimeScale;
	double m_dTimeElapsed;
	LONGLONG m_llPerformancePerSecond;
	LONGLONG m_llFrameTime;
	LONGLONG m_llNextTime;
	LONGLONG m_llLasttime;
public:
	CTimer(double fps);
	~CTimer(void);
	void Start();
	bool ReadyForNextTime();
	double TimeElapsed(){return m_dTimeElapsed;}
};


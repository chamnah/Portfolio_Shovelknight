#pragma once

class CTimeMgr
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llFrequency;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llOldCount;

	double          m_dAccTime;
	double			m_dt;	// 1프레임 시간
	UINT			m_iFPS;
	wchar_t			m_szFPS[255];

public:
	float DeltaTime() { return (float)m_dt; }

public:
	void init();
	int update();
	void render(HDC _dc);
};
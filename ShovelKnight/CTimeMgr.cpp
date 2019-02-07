#include "stdafx.h"
#include "CTimeMgr.h"

CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llCurCount{}
	, m_llOldCount{}
	, m_dt(0.)
	, m_szFPS{}
	, m_bStop(false)
{
}

CTimeMgr::~CTimeMgr()
{
}

float CTimeMgr::DeltaTime()
{
	if (m_dt > 0.2f)
		m_dt = 0.2f;

	if (m_bStop)
		m_dt = 0;
	return (float)m_dt;
}

void CTimeMgr::init()
{
	QueryPerformanceFrequency(&m_llFrequency); // 1초에 타이머가 셀수있는 진동수	
	QueryPerformanceCounter(&m_llOldCount);
}

int CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dt = (double)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (double)m_llFrequency.QuadPart;

	m_llOldCount = m_llCurCount;

	// 1초에 1번 계산
	m_dAccTime += m_dt;
	++m_iFPS;
	if (m_dAccTime > 1.)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_iFPS, 255);
		m_dAccTime = 0.;
		m_iFPS = 0;
	}

	return 0;
}

void CTimeMgr::render(HDC _dc)
{
	SetBkMode(_dc, TRANSPARENT);
	SetTextColor(_dc, RGB(255, 255, 255));
	TextOut(_dc, 10, 60, m_szFPS, wcslen(m_szFPS));
}
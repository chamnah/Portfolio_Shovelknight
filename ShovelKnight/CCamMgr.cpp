#include "stdafx.h"
#include "CCamMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CStageMgr.h"

CCamMgr::CCamMgr()
	:m_fSpeed(200),
	m_vPreLook(Vec2(0, 0))
	, m_vPlayerPos{}
	,m_bStop(false)
	, m_vLeftEnd{}
	, m_vRightEnd{}
{
	m_vLook = Vec2(CCore::GetInst()->GetResolution().x / 2, CCore::GetInst()->GetResolution().y / 2);
}


CCamMgr::~CCamMgr()
{
}

void CCamMgr::update()
{
	m_vPreLook = m_vLook;

	if (CStageMgr::GetInst()->GetCurState() == STAGE::LOGO)
		return;
	if (!m_bStop)
	{
		if (CStageMgr::GetInst()->GetCurState() == STAGE::START || CStageMgr::GetInst()->GetCurState() == STAGE::ONE)
		{
			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD))
				m_vLook.x -= m_fSpeed * DT;

			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
				m_vLook.x += m_fSpeed * DT;

			if (m_vLook.x <= (m_vLeftEnd.x + (CCore::GetInst()->GetResolution().x / 2)) || m_vPlayerPos.x <= (m_vLeftEnd.x + (CCore::GetInst()->GetResolution().x / 2)))
				m_vLook.x = m_vLeftEnd.x + (CCore::GetInst()->GetResolution().x / 2.f);
			else if (m_vLook.x >= m_vRightEnd.x - (CCore::GetInst()->GetResolution().x / 2.f) || m_vPlayerPos.x + 300 >= (m_vRightEnd.x - (CCore::GetInst()->GetResolution().x / 2)))
				m_vLook.x = m_vRightEnd.x - (CCore::GetInst()->GetResolution().x / 2.f);
		}
		else
		{
			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD))
				m_vLook.x -= m_fSpeed * DT;

			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
				m_vLook.x += m_fSpeed * DT;
			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_UP, KEY_STATE::HOLD))
				m_vLook.y -= m_fSpeed * DT;

			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD))
				m_vLook.y += m_fSpeed * DT;
		}
	}
	m_vDiff.x = m_vLook.x - (CCore::GetInst()->GetResolution().x / 2);
	m_vDiff.y = m_vLook.y - CCore::GetInst()->GetResolution().y / 2;
}

// 여기서 부터 확인을 하면 된다.
void CCamMgr::SetLook(float _x, float _y)
{
	m_vLook = Vec2(_x, _y);
}

void CCamMgr::ScrollCamera(Vec2& _vStart,Vec2& _vEnd,float _fTime)
{

}

bool CCamMgr::IsMove()
{
	if (m_vLook.x <= m_vLeftEnd.x + (CCore::GetInst()->GetResolution().x / 2))
		return true;
	else if (m_vLook.x >= m_vRightEnd.x - (CCore::GetInst()->GetResolution().x / 2))
		return true;
	return false;
}
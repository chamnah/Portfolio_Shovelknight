#include "stdafx.h"
#include "CCamMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CStageMgr.h"

CCamMgr::CCamMgr()
	:m_fSpeed(200),
	m_vPreLook(Vec2(0, 0))
{
	m_vLook = Vec2(CCore::GetInst()->GetResolution().x / 2,0.f);
}


CCamMgr::~CCamMgr()
{
}

void CCamMgr::update()
{
	m_vPreLook = m_vLook;

	if (CStageMgr::GetInst()->GetCurState() == STAGE::TOOL)
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

	m_vDiff.x = m_vLook.x - (CCore::GetInst()->GetResolution().x / 2);
	m_vDiff.y = m_vLook.y - CCore::GetInst()->GetResolution().y / 2;
}

// 여기서 부터 확인을 하면 된다.
void CCamMgr::SetLook(float _x, float _y)
{
	if((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) <= CCore::GetInst()->GetResolution().x)
		m_vLook.x = CCore::GetInst()->GetResolution().x / 2;
	else
	{
		if (_x > CCore::GetInst()->GetResolution().x / 2)
		{
			m_vLook.x = _x;
			if ((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) >= CCore::GetInst()->GetResolution().x &&
				(CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - (CCore::GetInst()->GetResolution().x / 2) < _x)
				m_vLook.x = (CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - (CCore::GetInst()->GetResolution().x / 2);
		}
		else
			m_vLook.x = CCore::GetInst()->GetResolution().x / 2;
	}
	m_vLook.y = _y;

}
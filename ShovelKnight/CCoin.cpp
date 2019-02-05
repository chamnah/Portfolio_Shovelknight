#include "stdafx.h"
#include "CCoin.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CGameMgr.h"

CCoin::CCoin(COIN_TYPE _eType)
	:m_bDeath(false)
{
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Coin", L"Image\\gems.bmp");
	m_eCoinType = _eType;
	m_eType = OBJ_TYPE::DROP;
	m_tSize = tSize(21,16);
	
	if (m_eCoinType == COIN_TYPE::ONE)
	{
		m_vScale = Vec2(16, 16);
		m_vOffset = Vec2(-2,15);
	}
	else if (m_eCoinType == COIN_TYPE::TWO)
		m_vScale = Vec2(6, 6);

	Init();
}

CCoin::~CCoin()
{
}

int CCoin::update()
{
	if (m_bDeath)
		return INT_MAX;
	CCamObj::update();
	return 0;
}

void CCoin::render(HDC _dc)
{
	TransparentBlt(_dc,m_vPos.x - m_tSize.x * 1.5f,m_vPos.y - m_tSize.y * 1.5, m_tSize.x * 3,m_tSize.y * 3,m_pTex->GetDC()
		,0 * m_tSize.x,0, m_tSize.x, m_tSize.y,RGB(0,255,0));

	CCamObj::render(_dc);
}

DIR CCoin::OnCollisionEnter(CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		m_bDeath = true;
		CGameMgr::GetInst()->SetGold((int)m_eCoinType + 1);
	}
	return DIR::NONE;
}
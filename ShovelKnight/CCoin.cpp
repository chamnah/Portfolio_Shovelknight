#include "stdafx.h"
#include "CCoin.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CGameMgr.h"
#include "CTimeMgr.h"
#include "SoundMgr.h"
#include "CCamMgr.h"

CCoin::CCoin(COIN_TYPE _eType)
	:m_bDeath(false)
{
	m_fSpeed = 0.f;
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Coin", L"Image\\gems.bmp");
	m_eCoinType = _eType;
	m_eType = OBJ_TYPE::DROP;
	m_tSize = tSize(21,16);
	m_vRealPos = m_vPos;
	m_fJump = -500.f;
	m_bJump = true;

	if (m_eCoinType == COIN_TYPE::ONE)
	{
		m_vScale = Vec2(16, 16);
		m_vOffset = Vec2(-2,15);
	}
	else if (m_eCoinType == COIN_TYPE::TWO)
	{
		m_vScale = Vec2(22, 20);
		m_vOffset = Vec2(-1, 13);
	}
	else if (m_eCoinType == COIN_TYPE::THREE)
	{
		m_vScale = Vec2(22, 25);
		m_vOffset = Vec2(-1, 8);
	}
	else if (m_eCoinType == COIN_TYPE::FOUR)
	{
		m_vScale = Vec2(30, 25);
		m_vOffset = Vec2(-1, 8);
	}
	else if (m_eCoinType == COIN_TYPE::FIVE)
	{
		m_vScale = Vec2(38, 32);
		m_vOffset = Vec2(-1, 5);
	}
	else if (m_eCoinType == COIN_TYPE::SIX)
	{
		m_vScale = Vec2(46, 39);
		m_vOffset = Vec2(-1, -1);
	}
}

CCoin::~CCoin()
{
}

void CCoin::Init()
{
	m_vRealPos = m_vPos;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	CCamObj::Init();
}

int CCoin::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_bJump)
	{
		m_vRealPos.x += m_fSpeed * DT;
	}

	Gravity(m_vRealPos);

	if (m_bDeath)
		return INT_MAX;
	CCamObj::update();
	return 0;
}

void CCoin::render(HDC _dc)
{
	TransparentBlt(_dc,m_vPos.x - m_tSize.x * 1.5f,m_vPos.y - m_tSize.y * 1.5, m_tSize.x * 3,m_tSize.y * 3,m_pTex->GetDC()
		, (int)m_eCoinType * m_tSize.x,0, m_tSize.x, m_tSize.y,RGB(0,255,0));

	CCamObj::render(_dc);
}

DIR CCoin::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		CSoundMgr::GetInst()->Play(L"Coin",false);
		m_bDeath = true;
		CGameMgr::GetInst()->AddGold((int)m_eCoinType + 1);
	}
	return DIR::NONE;
}
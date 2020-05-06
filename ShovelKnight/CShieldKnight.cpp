#include "stdafx.h"
#include "CShieldKnight.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"

CShieldKnight::CShieldKnight()
	:m_bFall(false)
{
	m_pTex = TEX_LOAD(L"ShieldKnight",L"Image\\ShieldKnight.bmp");
	m_tSize = tSize(57,47);
	m_pAnim->AddAnimation(L"Fall", m_pTex, RECT{0,m_tSize.y * 0,m_tSize.x,m_tSize.y},1,0.2f);
	m_pAnim->AddAnimation(L"Appear", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->PlayAnim(L"Appear",false);
	m_fSpeed = 200.f;
	m_vScale = Vec2(100,100);
	m_eType = OBJ_TYPE::SHIELD;
}

CShieldKnight::~CShieldKnight()
{
}

int CShieldKnight::update()
{
	if (m_bDeath)
		return INT_MAX;

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);

	if (m_bFall)
	{
		m_pAnim->PlayAnim(L"Fall", false);
		m_vRealPos.y += m_fSpeed * DT;
	}
	else
	{
		m_fAccTime += DT;
		if (m_fAccTime >= 3.f)
		{
			m_bFall = true;
		}
	}
	m_pAnim->update();
	CCamObj::update();
	return 0;
}

void CShieldKnight::render(HDC _dc)
{
	m_pAnim->render(_dc);
	CCamObj::render(_dc);
}

DIR CShieldKnight::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	return DIR::NONE;
}

void CShieldKnight::OnCollision(CCollider * _other)
{
}
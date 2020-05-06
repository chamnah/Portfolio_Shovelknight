#include "stdafx.h"
#include "CPurpleFire.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CCamMgr.h"

CPurpleFire::CPurpleFire()
{
	m_tSize = tSize(65,65);
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"BossAttack", L"Image\\BossAttack.bmp");
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"R_Attack", m_pTex, RECT{ 0,m_tSize.y * 0,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"L_Attack", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"U_Attack", m_pTex, RECT{ 0,m_tSize.y * 2,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"D_Attack", m_pTex, RECT{ 0,m_tSize.y * 3,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"RD_Attack", m_pTex, RECT{ 0,m_tSize.y * 4,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"LD_Attack", m_pTex, RECT{ 0,m_tSize.y * 5,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"RU_Attack", m_pTex, RECT{ 0,m_tSize.y * 6,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"LU_Attack", m_pTex, RECT{ 0,m_tSize.y * 7,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_eType = OBJ_TYPE::SKILL;
	m_pAnim->PlayAnim(L"R_Attack", true);
	m_vScale = Vec2(40.f, 40.f);
	m_fSpeed = 300.f;
}

CPurpleFire::~CPurpleFire()
{
}

int CPurpleFire::update()
{
	if (m_eDir == DIR::RIGHT)
	{
		m_pAnim->PlayAnim(L"R_Attack", true);
		m_vRealPos.x += m_fSpeed * DT;
	}
	else if (m_eDir == DIR::LEFT)
	{
		m_pAnim->PlayAnim(L"L_Attack", true);
		m_vRealPos.x -= m_fSpeed * DT;
	}
	else if (m_eDir == DIR::UP)
	{
		m_pAnim->PlayAnim(L"U_Attack", true);
		m_vRealPos.y -= m_fSpeed * DT;
	}
	else if (m_eDir == DIR::DOWN)
	{
		m_pAnim->PlayAnim(L"D_Attack", true);
		m_vRealPos.y += m_fSpeed * DT;
	}
	else if (m_eDir == DIR::RIGHT_UP)
	{
		m_pAnim->PlayAnim(L"RD_Attack", true);
		m_vRealPos.x += (m_fSpeed * DT) * cosf(PI / 4.f);
		m_vRealPos.y -= (m_fSpeed * DT) * sinf(PI / 4.f);
	}
	else if (m_eDir == DIR::LEFT_UP)
	{
		m_pAnim->PlayAnim(L"LD_Attack", true);
		m_vRealPos.x -= (m_fSpeed * DT) * cosf(PI / 4.f);
		m_vRealPos.y -= (m_fSpeed * DT) * sinf(PI / 4.f);
	}
	else if (m_eDir == DIR::RIGHT_DOWN)
	{
		m_pAnim->PlayAnim(L"RU_Attack", true);
		m_vRealPos.x += (m_fSpeed * DT) * cosf(PI / 4.f);
		m_vRealPos.y += (m_fSpeed * DT) * sinf(PI / 4.f);
	}
	else if (m_eDir == DIR::LEFT_DOWN)
	{
		m_pAnim->PlayAnim(L"LU_Attack", true);
		m_vRealPos.x -= (m_fSpeed * DT) * cosf(PI / 4.f);
		m_vRealPos.y += (m_fSpeed * DT) * sinf(PI / 4.f);
	}

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_bDeath)
	{
		return INT_MAX;
	}
	else if (CCore::GetInst()->GetResolution().x < m_vPos.x || m_vPos.x < 0 ||
		m_vPos.y < 0 || CCore::GetInst()->GetResolution().y < m_vPos.y)
		return INT_MAX;

	CSkill::update();

	return 0;
}

DIR CPurpleFire::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::BLOCK)
	{
		((CCamObj*)_other->GetOwner())->SetDeath(true);
	}
	return DIR();
}
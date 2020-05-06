#include "stdafx.h"
#include "CBubbleAttack.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CEffect.h"
#include "CStageMgr.h"
#include "CTimeMgr.h"
#include "CGameStage.h"
#include "CCamMgr.h"

CBubbleAttack::CBubbleAttack()
	:m_eDir(DIR::NONE)
	, m_fSpeed(0.f)
	, m_fAcc(0.f)
	, m_fPosY(0.f)
	, m_iID(0)
{
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Bubble",L"Image\\Bubble.bmp");
	m_eType = OBJ_TYPE::SKILL;
	m_eSkillType = SKILL_TYPE::BUBBLE_ATTACK;
	m_fSpeed = 300.f;
	m_vScale = Vec2(100, 100);
}

CBubbleAttack::~CBubbleAttack()
{
	if (CStageMgr::GetInst()->GetCurStage() != nullptr)
	{
		if(((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetSaveObj().size() > 0)
		((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetSaveObj()[m_iID].bDeath = true;
	}
}

void CBubbleAttack::Init()
{
	CCamObj::Init();
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"Idle", m_pTex, RECT{ 0,0,27,27 }, 4, 0.2f);
	m_pAnim->PlayAnim(L"Idle", true);
}

int CBubbleAttack::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_eDir == DIR::LEFT)
	{
		m_fAcc += m_fSpeed * DT;
		m_vRealPos.x -= m_fSpeed * DT;

		if (m_fAcc >= 500.f)
		{
			m_eDir = DIR::UP;
		}
	}
	else if (m_eDir == DIR::UP)
		m_vRealPos.y -= m_fSpeed * DT;
	else if (m_eDir == DIR::NONE)
	{
		m_vRealPos.y -= m_fSpeed * DT;

		if (m_vRealPos.y <= m_fPosY)
			m_vRealPos.y = m_fPosY;
	}

	if (m_vPos.x + m_pTex->GetWidth() < 0 || m_vPos.y + m_pTex->GetHeight() < 0)
		m_bDeath = true;

	if (m_bDeath)
	{
		if(m_eDir == DIR::NONE)
			((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetSaveObj()[m_iID].bDeath = true;
		CreateEffect(TEX_LOAD(L"BubbleEffect", L"Image\\BubbleEffect.bmp"), m_vRealPos, 2);
		return INT_MAX;
	}
	CSkill::update();
	return 0;
}

DIR CBubbleAttack::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
		if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
			m_bDeath = true;
		else if(_other->GetOwner()->GetType() == OBJ_TYPE::SKILL && ((CSkill*)_other->GetOwner())->GetOwner() != nullptr 
			&& ((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
			m_bDeath = true;	
	return DIR::NONE;
}
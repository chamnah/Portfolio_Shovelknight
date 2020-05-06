#include "stdafx.h"
#include "CBasic.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CDynamicObj.h"
#include "CMonster.h"
#include "SoundMgr.h"

CBasic::CBasic()
{
	m_vScale = Vec2(75, 40);
	m_fTime = 0.2f;
}

CBasic::~CBasic()
{
}

void CBasic::Init()
{
	m_pColl = new CCollider;
	m_pColl->SetOwner(this);
	m_pColl->SetScale(m_vScale);
	m_eType = OBJ_TYPE::SKILL;
}

int CBasic::update()
{
	m_vPos = m_pOwner->GetPos() + m_vOffset;

	CSkill::update();

	return 0;
}

void CBasic::render(HDC _dc)
{
	CSkill::render(_dc);
}

DIR CBasic::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	CSkill::OnCollisionEnter(_mine,_other);
	if ((_other->GetOwner()->GetType() == OBJ_TYPE::MONSTER || _other->GetOwner()->GetType() == OBJ_TYPE::KING) && m_eSkillType == SKILL_TYPE::JUMP_ATTACK)
	{
		CSoundMgr::GetInst()->Play(L"DigPile", false);
		((CDynamicObj*)m_pOwner)->SetJump(-900.f);
	}
	else if (_other->GetOwner()->GetType() == OBJ_TYPE::BLOCK)
	{
		((CCamObj*)_other->GetOwner())->SetDeath(true);
		if (m_eSkillType == SKILL_TYPE::JUMP_ATTACK)
		{
			CSoundMgr::GetInst()->Play(L"DigPile", false);
			((CDynamicObj*)m_pOwner)->SetJump(-900.f);
		}
	}
	else if ((_other->GetOwner()->GetType() == OBJ_TYPE::SKILL && ((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::BUBBLE_ATTACK))
	{
		if (m_eSkillType == SKILL_TYPE::JUMP_ATTACK)
		{
			CSoundMgr::GetInst()->Play(L"DigPile",false);
			((CDynamicObj*)m_pOwner)->SetJump(-1200.f);
		}
	}

	return DIR::NONE;
}
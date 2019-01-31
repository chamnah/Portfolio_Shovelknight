#include "stdafx.h"
#include "CBasic.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CDynamicObj.h"

CBasic::CBasic()
{
	m_vScale = Vec2(10,10);
	m_fTime = 0.2f;
}

CBasic::~CBasic()
{
}

void CBasic::Init()
{
	m_vScale = Vec2(25, 25);
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

int CBasic::OnCollisionEnter(CCollider * _other)
{
	CSkill::OnCollisionEnter(_other);
	if (m_eSkillType == SKILL_TYPE::JUMP_ATTACK)
	{
		((CDynamicObj*)m_pOwner)->SetJump(-500.f);
	}

	return 0;
}
#include "stdafx.h"
#include "CSkill.h"
#include "CCollider.h"
#include "CDynamicObj.h"

CSkill::CSkill()
	:m_pColl(nullptr)
	, m_fTime(0.f)
	, m_fAccTime(0.f)
	,m_pOwner(nullptr)
	, m_eSkillType(SKILL_TYPE::BASIC)
{
}

CSkill::~CSkill()
{
	delete m_pColl;
	m_pColl = nullptr;
}

int CSkill::update()
{
	if(m_pColl != nullptr)
	m_pColl->Update();
	return 0;
}

void CSkill::render(HDC _dc)
{
	if (m_pColl != nullptr)
		m_pColl->Render(_dc);
}

DIR CSkill::OnCollisionEnter(CCollider * _other)
{
	if (m_pOwner->GetType() == OBJ_TYPE::PLAYER && _other->GetOwner()->GetType() == OBJ_TYPE::MONSTER)
	{
		 ((CDynamicObj*)_other->GetOwner())->SetHP(((CDynamicObj*)_other->GetOwner())->GetHP() - 1);
	}

	return DIR::NONE;
}
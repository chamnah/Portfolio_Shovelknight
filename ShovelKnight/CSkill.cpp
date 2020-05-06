#include "stdafx.h"
#include "CSkill.h"
#include "CCollider.h"
#include "CDynamicObj.h"
#include "CAnimator.h"
#include "CMonster.h"
#include "CResMgr.h"
#include "CEffect.h"
#include "CStageMgr.h"
#include "CCore.h"

CSkill::CSkill()
	:
	m_fTime(0.f)
	, m_fAccTime(0.f)
	,m_pOwner(nullptr)
	, m_eSkillType(SKILL_TYPE::R_BASIC)
{
}

CSkill::~CSkill()
{
	if (m_pColl != nullptr)
	delete m_pColl;
	m_pColl = nullptr;

	if(m_pAnim != nullptr)
	delete m_pAnim;
	m_pAnim = nullptr;
}

void CSkill::InitColl()
{
	m_pColl = new CCollider;
	m_pColl->SetOwner(this);
	m_pColl->SetOffset(m_vOffset);
	m_pColl->SetScale(m_vScale);
}

int CSkill::update()
{
	if (m_pAnim != nullptr)
		m_pAnim->update();
	if(m_pColl != nullptr)
	m_pColl->Update();
	return 0;
}

void CSkill::render(HDC _dc)
{
	if (m_pAnim != nullptr)
		m_pAnim->render(_dc);
	if (m_pColl != nullptr)
		m_pColl->Render(_dc);
}

void CSkill::CreateEffect(CTexture * _pTex, const Vec2 & _Pos, int _iMaxSize)
{
	CObj* pObj = new CEffect;
	pObj->SetPos(_Pos);
	pObj->SetTexture(_pTex);
	((CEffect*)pObj)->SetMaxSize(_iMaxSize);
	pObj->Init();
	CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pObj);
}

DIR CSkill::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	if (m_pOwner->GetType() == OBJ_TYPE::PLAYER && (_other->GetOwner()->GetType() == OBJ_TYPE::MONSTER || _other->GetOwner()->GetType() == OBJ_TYPE::KING))
	{
		if (((CMonster*)_other->GetOwner())->GetMType() == M_TYPE::BUBBLE_DRAGON)
			return DIR::NONE;
		if(m_eSkillType == SKILL_TYPE::R_BASIC)
		 ((CDynamicObj*)_other->GetOwner())->TakeDamage(1,DIR::RIGHT);
		else if(m_eSkillType == SKILL_TYPE::L_BASIC)
			((CDynamicObj*)_other->GetOwner())->TakeDamage(1,DIR::LEFT);
		else if(m_eSkillType == SKILL_TYPE::SMASH)
			((CDynamicObj*)_other->GetOwner())->TakeDamage(2, DIR::LEFT);
		else if (m_eSkillType == SKILL_TYPE::JUMP_ATTACK)
			((CDynamicObj*)_other->GetOwner())->TakeDamage(1, DIR::NONE);
		else if (m_eSkillType == SKILL_TYPE::DART_ATTACK)
		{
			m_bDeath = true;
			((CDynamicObj*)_other->GetOwner())->TakeDamage(1, DIR::NONE);
		}
		else if (m_eSkillType == SKILL_TYPE::LIGHTNIG)
		{
			m_bDeath = true;
			((CDynamicObj*)_other->GetOwner())->TakeDamage(1, DIR::NONE);
		}
		else
		{
			m_bDeath = true;
			((CDynamicObj*)_other->GetOwner())->TakeDamage();
		}
	}
	else if ((m_pOwner->GetType() == OBJ_TYPE::MONSTER || m_pOwner->GetType() == OBJ_TYPE::KING) && _other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		m_bDeath = true;
		//((CDynamicObj*)_other->GetOwner())->SetHP(((CDynamicObj*)_other->GetOwner())->GetHP() - 1);
	}

	return DIR::NONE;
}
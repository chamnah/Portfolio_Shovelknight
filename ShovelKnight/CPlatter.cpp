#include "stdafx.h"
#include "CPlatter.h"
#include "CCamMgr.h"
#include "CCollider.h"
#include "CSkill.h"
#include "CDynamicObj.h"
#include "CResMgr.h"
#include "CParticle.h"
#include "CStageMgr.h"
#include "CPotion.h"
#include "CGameStage.h"

CPlatter::CPlatter()
	:m_iID(0)
{
	m_pTex = TEX_LOAD(L"Platter",L"Image\\Platter.bmp");
	m_tSize = tSize(m_pTex->GetWidth(), m_pTex->GetHeight());
	m_vScale = Vec2(m_tSize.x * 4,m_tSize.y * 4);
	m_eType = OBJ_TYPE::OBJECT;
}

CPlatter::~CPlatter()
{
}

int CPlatter::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);

	if (m_bDeath)
	{
		CPotion* pPotion = new CPotion;
		pPotion->SetRealPos(m_vRealPos);
		pPotion->Init();
		CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER].push_back(pPotion);

		CTexture* pTex = TEX_LOAD(L"PlatterCover", L"Image\\PlatterCover.bmp");
		CParticle* pParticle = nullptr;

		pParticle =	new CParticle;
		pParticle->SetRealPos(m_vRealPos);
		pParticle->SetTexture(pTex);
		pParticle->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
		CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pParticle);

		pTex = TEX_LOAD(L"PlatterBottom", L"Image\\PlatterBottom.bmp");

		pParticle = new CParticle;
		pParticle->SetRealPos(m_vRealPos);
		pParticle->SetTexture(pTex);
		pParticle->SetDir(DIR::LEFT);
		pParticle->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
		CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pParticle);
		return INT_MAX;
	}

	CCamObj::update();

	return 0;
}

void CPlatter::render(HDC _dc)
{
	TransparentBlt(_dc, m_vPos.x - m_tSize.x * 2, m_vPos.y - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4, m_pTex->GetDC(), 0, 0, m_tSize.x, m_tSize.y, RGB(0, 255, 0));
	CCamObj::render(_dc);
}

DIR CPlatter::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::SKILL)
	{
		if(((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::L_BASIC || ((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::R_BASIC)
		{
			((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetFiniteObj()[m_iID].bDeath = true;
			m_bDeath = true;
		}
		else if (((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::JUMP_ATTACK)
			((CDynamicObj*)(((CSkill*)_other->GetOwner())->GetOwner()))->SetJump(-800.f);
	}
	return DIR::NONE;
}
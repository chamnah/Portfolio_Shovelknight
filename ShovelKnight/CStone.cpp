#include "stdafx.h"
#include "CStone.h"
#include "CCamMgr.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CSkill.h"
#include "CStageMgr.h"
#include "CGameStage.h"
#include "CCoin.h"
#include "CImageObj.h"
#include "CParticle.h"

CStone::CStone()
	:m_iIdx(0)
	,m_iID(0)
{
	m_pTex = TEX_LOAD(L"Stone",L"Image\\Stone.bmp");
	m_tSize = tSize(33,15);
	m_vScale = Vec2(50,50);
	m_eType = OBJ_TYPE::OBJECT;
	srand(time(NULL));
}

CStone::~CStone()
{
}

void CStone::Init()
{
	CCamObj::Init();
}

int CStone::update()
{
	if (m_bDeath)
		return INT_MAX;

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);
	CCamObj::update();	
	return 0;
}

void CStone::render(HDC _dc)
{
	TransparentBlt(_dc,m_vPos.x - m_tSize.x * 2,m_vPos.y - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4,m_pTex->GetDC(),m_iIdx * m_tSize.x,0,m_tSize.x,m_tSize.y,RGB(0,255,0));
	CCamObj::render(_dc);
}

DIR CStone::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::SKILL && (((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::R_BASIC || ((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::L_BASIC))
	{
		m_iIdx += 1;

		if(((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::R_BASIC)
		{
			CCoin* pCoin = new CCoin(COIN_TYPE(rand() % 6));
				pCoin->SetPos(m_vRealPos);
				pCoin->SetSpeed(rand() % 151 + 50);
				pCoin->Init();
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pCoin);

				pCoin = new CCoin(COIN_TYPE(rand() % 6));
				pCoin->SetPos(m_vRealPos);
				pCoin->SetSpeed(rand() % 151 + 50);
				pCoin->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pCoin);

			pCoin = new CCoin(COIN_TYPE(rand() % 6));
			pCoin->SetPos(m_vRealPos);
			pCoin->SetSpeed(rand() % 151 + 50);
			pCoin->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pCoin);

			CTexture* pTex = TEX_LOAD(L"PlatterCover", L"Image\\StoneEffect.bmp");
			CParticle* pParticle = nullptr;

			for (int i = 0; i < 3; ++i)
			{
				pParticle = new CParticle;
				pParticle->SetRealPos(m_vRealPos);
				pParticle->SetTexture(pTex);
				pParticle->SetDir(DIR::RIGHT);
				pParticle->SetJump(-800.f);
				pParticle->SetSpeed((rand() % 151 + 100));
				pParticle->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pParticle);
			}
		}
		else if (((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::L_BASIC)
		{
			CCoin* pCoin = new CCoin(COIN_TYPE(rand() % 6));
			pCoin->SetPos(m_vRealPos);
			pCoin->SetSpeed(-(rand() % 151 + 50));
			pCoin->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pCoin);

			pCoin = new CCoin(COIN_TYPE(rand() % 6));
			pCoin->SetPos(m_vRealPos);
			pCoin->SetSpeed(-(rand() % 151 + 50));
			pCoin->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pCoin);

			pCoin = new CCoin(COIN_TYPE(rand() % 6));
			pCoin->SetPos(m_vRealPos);
			pCoin->SetSpeed(-(rand() % 151 + 50));
			pCoin->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pCoin);


			CTexture* pTex = TEX_LOAD(L"PlatterCover", L"Image\\StoneEffect.bmp");
			CParticle* pParticle = nullptr;

			for (int i = 0; i < 3; ++i)
			{
				pParticle = new CParticle;
				pParticle->SetRealPos(m_vRealPos);
				pParticle->SetTexture(pTex);
				pParticle->SetDir(DIR::LEFT);
				pParticle->SetSpeed((rand() % 151 + 100));
				pParticle->SetJump(-800.f);
				pParticle->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pParticle);
			}
		}
		if (m_iIdx > 4)
		{
			// 여기다가 이제 아이템 추가시키고 돌맹이 날라가는 이펙트를 추가하면 된다.
			m_iIdx = 4;
			m_bDeath = true;
			((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetFiniteObj()[m_iID].bDeath = true;
		}
		((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetFiniteObj()[m_iID].iIdx = m_iIdx;
	}

	return DIR::NONE;
}
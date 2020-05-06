#include "stdafx.h"
#include "CFireBlock.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CBlock.h"
#include "CStageMgr.h"
#include "CCamMgr.h"

CFireBlock::CFireBlock()
	:m_fSpeed(0.f)
	, m_bMoving(false)
	, m_fArr(0.f)
	, m_bArr(false)
{
	m_tSize = tSize(16,31);
	m_vScale = Vec2(62,64);
	m_vOffset = Vec2(0,32);
	m_pTex = TEX_LOAD(L"FireBlock",L"Image\\FireBlock.bmp");
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"Idle", m_pTex, RECT{ 0,m_tSize.y * 0,m_tSize.x,m_tSize.y }, 4, 0.1f);
	m_pAnim->AddAnimation(L"Create", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 8, 0.1f);
	m_pAnim->AddAnimation(L"Death", m_pTex, RECT{ 0,m_tSize.y * 2,m_tSize.x,m_tSize.y }, 2, 0.1f);
	m_pAnim->PlayAnim(L"Create",false);
	m_eType = OBJ_TYPE::SKILL;
	m_eSkillType = SKILL_TYPE::FIRE_BLOCK;
	m_fSpeed = 500.f;
}

CFireBlock::~CFireBlock()
{
}

void CFireBlock::Init()
{
	m_vRealPos = m_vPos;
	CCamObj::Init();
}

int CFireBlock::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_pAnim->GetCurFinish() == true && m_pAnim->GetCurKey() == L"Create")
	{
		m_fArr = m_vRealPos.y - 260;
		m_pAnim->PlayAnim(L"Idle", true);
	}

	if (m_bMoving)
		Moving();

	if (m_bArr)
	{
		m_fTime += DT;

		if (m_fTime >= 1.f)
		{
			Vec2 vPos = CCamMgr::GetInst()->GetCamMousePos(m_vPos);
			vPos.y += 32.f;
			CBlock* pBlock = new CBlock;
			pBlock->SetPos(vPos);
			pBlock->SetRealPos(vPos);
			pBlock->SetScale(Vec2(TILE_SIZE, TILE_SIZE));
			pBlock->SetTexture(TEX_LOAD(L"BlackBlock", L"Image\\BlackBlock.bmp"));
			pBlock->SetEffectTexture(TEX_LOAD(L"BlackBlockDeath", L"Image\\BlackBlockDeath.bmp"));
			pBlock->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::BLOCK].push_back(pBlock);
			return INT_MAX;
		}
	}

	CCamObj::update();
	m_pAnim->update();
	return 0;
}

void CFireBlock::render(HDC _dc)
{
	m_pAnim->render(_dc);
	CCamObj::render(_dc);
}

void CFireBlock::Moving()
{
	m_vRealPos.y -= m_fSpeed * DT;
	if (m_fArr >= m_vRealPos.y)
	{
		m_vRealPos.y = m_fArr;
		m_bMoving = false;
		m_bArr = true;
	}
}

DIR CFireBlock::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::BLOCK)
	{
		((CCamObj*)_other->GetOwner())->SetDeath(true);
	}

	return DIR::NONE;
}
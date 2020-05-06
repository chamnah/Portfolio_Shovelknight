#include "stdafx.h"
#include "CSlime.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CStageMgr.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CGameStage.h"
#include "CCoin.h"
#include "CDCMgr.h"
#include "CCollider.h"
#include "CTile.h"

CSlime::CSlime()
	:m_bMoving(true)
	, m_eDirFirst(DIR::RIGHT)
{
	m_pTex = TEX_LOAD(L"Slime",L"Image\\Slime.bmp");
	m_tSize = tSize(20,12);
	m_pAnim->AddAnimation(L"Idle", m_pTex, RECT{0,m_tSize.y * 0,m_tSize.x,m_tSize.y},4,0.2f);
	m_pAnim->PlayAnim(L"Idle",true);
	m_fSpeed = 200.f;
	m_vScale = Vec2(80,40);
	m_iHP = 2;
	srand(time(NULL));
}

CSlime::~CSlime()
{
	delete m_pColl;
	m_pColl = nullptr;

	delete  m_pAnim;
	m_pAnim = nullptr;
}

int CSlime::update()
{
	if (m_iHP <= 0)
	{
		CreateEffect(m_vRealPos, (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Death", L"Image\\Death.bmp"), 5);

		if (((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetMonster()[m_iID].bItem == true)
		{
			CObj* pObj = new CCoin((COIN_TYPE)(rand() % 6));
			pObj->SetPos(m_vRealPos);
			pObj->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pObj);
		}
		((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetMonster()[m_iID].bDeath = true;
		((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetMonster()[m_iID].bItem = false;

		return INT_MAX;
	}

	Damage(false);

 	float fDis = Distance(((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos(),m_vRealPos);

	if (((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().x < m_vRealPos.x)
		m_eDir = DIR::LEFT;
	else
		m_eDir = DIR::RIGHT;

	if (fDis < 400.f)
	{
		if (!m_bJump)
		{
			m_eDirFirst = m_eDir;
		}
		if(m_bMoving)
			Moving();

		if(m_bJump)
		{
			if (m_eDirFirst == DIR::LEFT)
				m_vRealPos.x -= m_fSpeed * DT;
			else
				m_vRealPos.x += m_fSpeed * DT;
		}
		else
		{
			if (m_eDir == DIR::LEFT)
				m_vRealPos.x -= (m_fSpeed - 100.f) * DT;
			else
				m_vRealPos.x += (m_fSpeed - 100.f) * DT;
		}
		

		if (m_fTime > 5)
		{
			m_fTime = 0.f;
			m_bMoving = true;
		}
		m_fTime += DT;
	}
	Gravity(m_vRealPos);
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_vPos.x < 0 || m_vPos.y < 0 ||
		m_vPos.x > CCore::GetInst()->GetResolution().x || m_vPos.y > CCore::GetInst()->GetResolution().y)
	{
		((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetMonster()[m_iID].bDeath = true;
		return INT_MAX;
	}

	m_pAnim->update();
	CCamObj::update();
	return 0;
}

void CSlime::render(HDC _dc)
{
	m_pAnim->render(_dc);
	CCamObj::render(_dc);
}

DIR CSlime::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	CDynamicObj::OnCollisionEnter(_mine,_other);

	if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE && ((CTile*)(_other->GetOwner()))->GetTileType() == TILE_TYPE::TRAP)
		m_iHP = 0;

	return DIR::NONE;
}

void CSlime::Moving()
{
	m_fJump -= 800.f;
	m_bJump = true;

	m_bMoving = false;
}

void CSlime::TakeDamage(int iDamage, DIR _eDir)
{
	m_iHP -= iDamage;
	m_bDamage = true;
}

void CSlime::Damage(bool _bRepeat)
{
	if (m_bDamage)
	{
		if (m_fAccTime <= 0.05f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 0);
		else if (m_fAccTime <= 0.10f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 1);
		else if (m_fAccTime <= 0.15f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 2);
		else if (m_fAccTime <= 0.20f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 3);
		else if (m_fAccTime <= 0.25f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 4);
		else if (m_fAccTime <= 0.30f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 5);
		else if (m_fAccTime <= 0.35f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::SLIME, 6);
		else if (!_bRepeat)
		{
			m_pTex = TEX_LOAD(L"Slime", L"Image\\Slime.bmp");
			m_fAccTime = 0.f;
			m_bDamage = false;
		}
		m_fAccTime += DT;
	}
}
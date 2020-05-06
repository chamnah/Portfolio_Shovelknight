#include "stdafx.h"
#include "CBeeto.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CStageMgr.h"
#include "CEffect.h"
#include "CCollider.h"
#include "CGameStage.h"
#include "CCore.h"
#include "CCoin.h"
#include "CDCMgr.h"
#include "CSkill.h"
#include "CTile.h"
#include "SoundMgr.h"
#include "CCamMgr.h"

CBeeto::CBeeto(float _fX, float _fY)
	:vTemp{}
{
	m_vRealPos = Vec2(_fX, _fY);
}

CBeeto::CBeeto(Vec2 _vPos)
	: vTemp{}
{
	m_vRealPos = _vPos;
}

CBeeto::~CBeeto()
{
	delete m_pAnim;
	m_pAnim = nullptr;
}

void CBeeto::Init()
{
	m_eMType = M_TYPE::BEETO;
	m_iID = 0;
	m_eDir = DIR::LEFT;
	m_fTime = 0.f;
	m_tSize = tSize(26,16);

	m_iHP = 1;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Beeto", L"Image\\Beeto.bmp");
	//m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BEETO, 0);
	m_vScale = Vec2(80,50);
	m_eType = OBJ_TYPE::MONSTER;
	m_fSpeed = 100.f;

	CCamObj::Init();

	m_pAnim->AddAnimation(L"R_Beeto_Walk", m_pTex, RECT{ 0,m_tSize.y * 0,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_Beeto_Walk", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_Beeto_Death", m_pTex, RECT{ 0,m_tSize.y * 2,m_tSize.x,m_tSize.y }, 2, 0.2f);
	m_pAnim->AddAnimation(L"R_Beeto_Death", m_pTex, RECT{ 0,m_tSize.y * 3,m_tSize.x,m_tSize.y }, 2, 0.2f);
	m_pAnim->PlayAnim(L"R_Beeto_Walk", true);

	srand(time(NULL));

}

int CBeeto::update()
{
	m_vPrePos = m_vPos;

	if (m_iHP <= 0)
	{
		if (m_bDeath || m_fTime >= 1.f)
		{
			CreateEffect(m_vRealPos, (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Death", L"Image\\Death.bmp"), 5);
			CSoundMgr::GetInst()->Play(L"Land",false);

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
		else
		{ 
			if (m_bJump)
			{
				if (m_eDir == DIR::RIGHT)
					m_vRealPos.x += m_fSpeed * DT;
				else if (m_eDir == DIR::LEFT)
					m_vRealPos.x -= m_fSpeed * DT;
			}
		}

		if (m_fTime == 0.f)
		{
			if (m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_Beeto_Death", true);
			else if (m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_Beeto_Death", true);

			CSoundMgr::GetInst()->Play(L"Flip", false);
		}

		m_fTime += DT;
	}
	else
	{
		if (m_eDir == DIR::RIGHT)
		{
			m_pAnim->PlayAnim(L"R_Beeto_Walk", true);
			m_vRealPos.x += m_fSpeed * DT;
		}

		if (m_eDir == DIR::LEFT)
		{
			m_vRealPos.x -= m_fSpeed * DT;
			m_pAnim->PlayAnim(L"L_Beeto_Walk", true);
		}

		if (m_pColl->GetRealPos().x - m_pColl->GetScale().x / 2.f <= 0)
			m_eDir = DIR::RIGHT;
		else if (m_pColl->GetRealPos().x + m_pColl->GetScale().x / 2.f >= CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE)
			m_eDir = DIR::LEFT;

		int iX = (m_vRealPos.x + m_vScale.x / 2.f) / TILE_SIZE;
		int iY = (m_vRealPos.y / TILE_SIZE) + 1;

		if (((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iY * CStageMgr::GetInst()->GetTileSizeX() + iX - 1])->GetTileType() != TILE_TYPE::COLL)
			m_eDir = DIR::RIGHT;

		iX = (m_vRealPos.x - m_vScale.x / 2.f) / TILE_SIZE;

		if (((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iY * CStageMgr::GetInst()->GetTileSizeX() + iX + 1])->GetTileType() != TILE_TYPE::COLL)
			m_eDir = DIR::LEFT;
	}

	Gravity(m_vRealPos);

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_vPos.x < 0 || m_vPos.y < 0 ||
		m_vPos.x > CCore::GetInst()->GetResolution().x || m_vPos.y > CCore::GetInst()->GetResolution().y)
	{
		((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetMonster()[m_iID].bDeath = true;
		return INT_MAX;
	}

	CCamObj::update();
	m_pAnim->update();

	return 0;
}

void CBeeto::render(HDC _dc)
{
	m_pAnim->render(_dc);
	CCamObj::render(_dc);
	//Rectangle(_dc, vTemp.x,vTemp.y, vTemp.x + 64, vTemp.y + 64);
	
}

DIR CBeeto::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	DIR eDir = CDynamicObj::OnCollisionEnter(_mine,_other);

	if (_other->GetOwner()->GetType() == OBJ_TYPE::SKILL && ((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::JUMP_ATTACK)
		m_bDeath = true;

	if (eDir != DIR::NONE && eDir != m_eDir)
		m_eDir = eDir;

	return DIR::NONE;
}

void CBeeto::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollision(_other);
}

void CBeeto::TakeDamage(int iDamage, DIR _eDir)
{
	m_iHP -= 1;
	if (_eDir == DIR::NONE)
		return;
	m_bJump = true;
	m_fJump -= 400.f;
	m_eDir = _eDir;
}
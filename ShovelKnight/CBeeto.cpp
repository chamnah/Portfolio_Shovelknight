#include "stdafx.h"
#include "CBeeto.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CStageMgr.h"
#include "CEffect.h"
#include "CCollider.h"

CBeeto::CBeeto(float _fX, float _fY)
{
	m_vRealPos = Vec2(_fX, _fY);
	Init();
}

CBeeto::CBeeto(Vec2 _vPos)
{
	m_vRealPos = _vPos;
	Init();
}

CBeeto::~CBeeto()
{
	delete m_pAnim;
	m_pAnim = nullptr;
}

void CBeeto::Init()
{
	m_fTime = 0.f;
	m_iWSize = 26;
	m_iHSize = 16;

	m_iHP = 3;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Beeto", L"Image\\Beeto.bmp");
	m_vScale = Vec2(40,50);
	m_eType = OBJ_TYPE::MONSTER;
	m_fSpeed = 100.f;

	CCamObj::Init();

	m_pAnim->AddAnimation(L"R_Beeto_Walk", m_pTex, RECT{ 0,m_iHSize * 0,m_iWSize,m_iHSize }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_Beeto_Walk", m_pTex, RECT{ 0,m_iHSize * 1,m_iWSize,m_iHSize }, 4, 0.2f);
	m_pAnim->AddAnimation(L"R_Beeto_Death", m_pTex, RECT{ 0,m_iHSize * 2,m_iWSize,m_iHSize }, 2, 0.2f);
	m_pAnim->AddAnimation(L"L_Beeto_Death", m_pTex, RECT{ 0,m_iHSize * 2,m_iWSize,m_iHSize }, 2, 0.2f);
	m_pAnim->PlayAnim(L"R_Beeto_Walk", true);
}

int CBeeto::update()
{
	m_vPrePos = m_vPos;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	Gravity(m_vRealPos);
	
	if (m_iHP <= 0)
	{
		m_fTime += DT;
		if (m_iDir == RIGHT)
			m_pAnim->PlayAnim(L"R_Beeto_Death", true);
		else if (m_iDir == LEFT)
			m_pAnim->PlayAnim(L"L_Beeto_Death",true);
		
		if (m_fTime >= 1.f)
		{
			CObj* pObj = new CEffect;
			pObj->SetPos(m_vPos);
			pObj->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pObj);
			return INT_MAX;
		}
	}
	else
	{
		if (m_iDir == RIGHT)
		{
			m_pAnim->PlayAnim(L"R_Beeto_Walk", true);
			m_vRealPos.x += m_fSpeed * DT;
		}

		if (m_iDir == LEFT)
		{
			m_vRealPos.x -= m_fSpeed * DT;
			m_pAnim->PlayAnim(L"L_Beeto_Walk", true);
		}
	}

	CCamObj::update();
	m_pAnim->update();

	return 0;
}

void CBeeto::render(HDC _dc)
{
	m_pAnim->render(_dc);
	CCamObj::render(_dc);
}

int CBeeto::OnCollisionEnter(CCollider * _other)
{
	int iDir = CDynamicObj::OnCollisionEnter(_other);
	if (iDir != 0 && iDir != m_iDir)
		m_iDir = iDir;
	return 0;
}

void CBeeto::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollision(_other);
}
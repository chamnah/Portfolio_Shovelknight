#include "stdafx.h"
#include "CSavePoint.h"
#include "CCollider.h"
#include "CGameMgr.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CCamMgr.h"

CSavePoint::CSavePoint()
	:m_eStage(STAGE::END)
	, m_vChildPos{}
	, m_bSave(false)
	, m_bNoFirst(false)
{
	m_tSize = tSize(21,48);
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"SavePoint", L"Image\\SavePoint.bmp");
	m_pFrontTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Broken", L"Image\\broken.bmp");
	m_pChildTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Rod", L"Image\\rod.bmp");
	m_vScale = Vec2(100,190);
	m_eType = OBJ_TYPE::OBJECT;

	m_pAnim = new CAnimator(this);

	m_pAnim->AddAnimation(L"Idle", m_pTex, RECT{ 0,(int)m_tSize.y * 0,(int)m_tSize.x,(int)m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"Flame", m_pTex, RECT{ 0,(int)m_tSize.y * 1,(int)m_tSize.x,(int)m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"Flaming", m_pTex, RECT{ 0,(int)m_tSize.y * 2,(int)m_tSize.x,(int)m_tSize.y }, 3, 0.2f);
	m_pAnim->PlayAnim(L"Idle",false);

	Test(Vec2(1.f, 1.f));
}

CSavePoint::~CSavePoint()
{
	if (m_pAnim != nullptr)
		delete m_pAnim;
	m_pAnim = nullptr;
}

void CSavePoint::Init()
{
	if (CGameMgr::GetInst()->IsSaveStage(m_eStage))
	{
		m_bSave = true;
		m_bNoFirst = true;
	}
	CCamObj::Init();
	m_vRealPos = m_vPos;
}

DIR CSavePoint::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		if (!m_bSave)
		{
			m_pAnim->PlayAnim(L"Flame", false);
			m_bSave = true;
			CGameMgr::GetInst()->AddSaveStage(tSave(m_eStage, m_vRealPos));
		}
	}
	return DIR::NONE;
}

int CSavePoint::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pAnim->update();
	m_pColl->Update();

	if (m_bNoFirst || (m_bSave && m_pAnim->GetCurFinish()))
	{
		m_pAnim->PlayAnim(L"Flaming", true);
	}
	return 0;
}
	//m_vChildPos = m_vPos + Vec2(0,95);

void CSavePoint::render(HDC _dc)
{
	//TransparentBlt(_dc, m_vChildPos.x - m_pChildTex->GetWidth() * 2, m_vChildPos.y - m_pChildTex->GetHeight() * 2, m_pChildTex->GetWidth() * 4, m_pChildTex->GetHeight() * 4, m_pChildTex->GetDC(), 0, 0, m_pChildTex->GetWidth(), m_pChildTex->GetHeight(), RGB(0, 255, 0));
	//TransparentBlt(_dc,m_vPos.x - m_tSize.x * 2,m_vPos.y - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4,m_pFrontTex->GetDC(),0,0, m_tSize.x, m_tSize.y,RGB(0,255,0));
	m_pAnim->render(_dc);
	m_pColl->Render(_dc);
}

void CSavePoint::Test(const Vec2 && test)
{
	m_vChildPos = test;
}
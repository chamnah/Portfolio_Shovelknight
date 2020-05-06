#include "stdafx.h"
#include "CEffect.h"
#include "CCamMgr.h"
#include "CResMgr.h"
#include "CAnimator.h"

CEffect::CEffect()
	:m_vRealPos{}
	, m_ptSize{}
	, m_iMaxSize(0)
{
	
}

CEffect::~CEffect()
{
	delete m_pAnim;
	m_pAnim = nullptr;
}

void CEffect::Init()
{
	m_vRealPos = m_vPos;
	m_ptSize.x = m_pTex->GetWidth() / m_iMaxSize;
	RECT rt1 = { 0,0,m_ptSize.x,(long)m_pTex->GetHeight() };
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"Death", m_pTex, rt1, m_iMaxSize, 0.1f);
	m_pAnim->PlayAnim(L"Death", false);
}

int CEffect::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pAnim->update();

	if (m_pAnim->GetCurFinish())
	{
		return INT_MAX;
	}
	return 0;
}

void CEffect::render(HDC _dc)
{
	m_pAnim->render(_dc);
}
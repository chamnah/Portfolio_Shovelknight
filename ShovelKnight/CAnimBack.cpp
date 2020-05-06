#include "stdafx.h"
#include "CAnimBack.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CCamMgr.h"

CAnimBack::CAnimBack()
{
}

CAnimBack::~CAnimBack()
{
	delete m_pAnim;
}

void CAnimBack::Init()
{
	m_tSize = tSize(48,48);
	m_pTex = TEX_LOAD(L"Candles",L"Image\\Candles.bmp");
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"Idle", m_pTex, RECT{0,0 * m_tSize.y,m_tSize.x,m_tSize.y},3,0.2f);
	m_pAnim->AddAnimation(L"Blow", m_pTex, RECT{ 0,1 * m_tSize.y,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->PlayAnim(L"Idle",true);
}

int CAnimBack::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);
	m_pAnim->update();
	return 0;
}

void CAnimBack::render(HDC _dc)
{
	m_pAnim->render(_dc);
}

void CAnimBack::Blow()
{
	m_pAnim->PlayAnim(L"Blow", false);
}
#include "stdafx.h"
#include "CBeeto.h"
#include "CResMgr.h"
#include "CAnimator.h"

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
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Beeto", L"Image\\Beeto.bmp");
	m_vScale = Vec2(10,50);
	m_eType = OBJ_TYPE::MONSTER;

	CCamObj::Init();

	m_pAnim->AddAnimation(L"Beeto_Walk", m_pTex, RECT{ 0,0,26,16 }, 4, 0.2f);
	m_pAnim->PlayAnim(L"Beeto_Walk", true);
}

int CBeeto::update()
{
	m_vPrePos = m_vPos;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_vRealPos.y += m_fAccTemp;
	CCamObj::update();
	m_pAnim->update();
	
	return 0;
}

void CBeeto::render(HDC _dc)
{
	/*
	int iWidth = m_pTex->GetWidth() / m_tAnim.horizontal;
	int iHeight = m_pTex->GetHeight() / m_tAnim.vertical;
	TransparentBlt(_dc, m_vPos.x - iWidth, m_vPos.y - iHeight, iWidth * 2, iHeight * 2, m_pTex->GetDC(), 0, 0, iWidth, iHeight, RGB(0, 255, 0));
	*/
	m_pAnim->render(_dc);
	CCamObj::render(_dc);

}
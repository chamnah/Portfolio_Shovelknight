#include "stdafx.h"
#include "CTextUI.h"
#include "CResMgr.h"

CTextUI::CTextUI()
{
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"HUB_Text", L"Image\\HUD_Text.bmp");
	m_vScale = Vec2(180.f, 20.f);
}

CTextUI::~CTextUI()
{
}

void CTextUI::render(HDC _dc)
{
	TransparentBlt(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, m_vScale.x, m_vScale.y, m_pTex->GetDC(),0, m_iType * 20, m_pTex->GetWidth(), 20, RGB(0,255,0));
	ChildRender(_dc);
}
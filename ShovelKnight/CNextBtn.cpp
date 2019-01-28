#include "stdafx.h"
#include "CNextBtn.h"
#include "CResMgr.h"
#include "CPanelUI.h"

CNextBtn::CNextBtn()
{
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Next", L"Image\\DOWN.bmp");
	m_vScale = Vec2((int)m_pTex->GetWidth(), (int)m_pTex->GetHeight());
}


CNextBtn::~CNextBtn()
{
}


void CNextBtn::MouseUp()
{
	((CPanelUI*)m_pParent)->NextPage();
}
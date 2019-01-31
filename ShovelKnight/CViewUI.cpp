#include "stdafx.h"
#include "CViewUI.h"

CViewUI::CViewUI()
{
}

CViewUI::~CViewUI()
{
}

void CViewUI::render(HDC _dc)
{
	if (m_pTex == nullptr)
		return;
	TransparentBlt(_dc,m_vFinalPos.x,m_vFinalPos.y,m_vScale.x,m_vScale.y,m_pTex->GetDC(),0,0,m_pTex->GetWidth(),m_pTex->GetHeight(),RGB(0,255,0));
	ChildRender(_dc);
}
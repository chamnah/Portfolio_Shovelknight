#include "stdafx.h"
#include "CViewUI.h"

CViewUI::CViewUI()
	:m_iType(0)
{
}

CViewUI::~CViewUI()
{
}

void CViewUI::render(HDC _dc)
{
	if (m_pTex == nullptr)
		return;
	TransparentBlt(_dc,m_vFinalPos.x,m_vFinalPos.y,m_vScale.x,m_vScale.y,m_pTex->GetDC(),0, m_tSize.y * m_iType,m_tSize.x,m_tSize.y,RGB(0,255,0));
	ChildRender(_dc);
}
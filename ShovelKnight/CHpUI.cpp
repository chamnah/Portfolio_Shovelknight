#include "stdafx.h"
#include "CHpUI.h"

CHpUI::CHpUI()
	:m_iState(FULL)
{
}

CHpUI::~CHpUI()
{
}

void CHpUI::render(HDC _dc)
{
	TransparentBlt(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, m_vScale.x, m_vScale.y, m_pTex->GetDC(), m_iState * 8, 0, 8, 8, RGB(0, 255, 0));
	ChildRender(_dc);
}
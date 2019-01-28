#include "stdafx.h"
#include "CImageObj.h"

CImageObj::CImageObj()
{
}

CImageObj::~CImageObj()
{
}

int CImageObj::update()
{
	return 0;
}

void CImageObj::render(HDC _dc)
{
	TransparentBlt(_dc, (int)m_vPos.x, (int)m_vPos.y, m_pTex->GetWidth(), m_pTex->GetHeight(), m_pTex->GetDC(), 0, 0, m_pTex->GetWidth(), m_pTex->GetHeight(), RGB(0, 255, 0));
}
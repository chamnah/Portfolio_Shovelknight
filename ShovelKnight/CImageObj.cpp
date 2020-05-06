#include "stdafx.h"
#include "CImageObj.h"
#include "CCamMgr.h"

CImageObj::CImageObj()
	:m_iIndex(0)
{
}

CImageObj::~CImageObj()
{
}

int CImageObj::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	
	return 0;
}

void CImageObj::render(HDC _dc)
{
	TransparentBlt(_dc, (int)m_vPos.x - (m_vScale.x / 2.f), (int)m_vPos.y - (m_vScale.y / 2.f), m_vScale.x, m_vScale.y, m_pTex->GetDC(), m_iIndex * m_tSize.x, 0, m_tSize.x, m_pTex->GetHeight(), RGB(0, 255, 0));
}
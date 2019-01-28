#include "stdafx.h"
#include "CObj.h"
#include "CCamMgr.h"

void CObj::CollisionRender(HDC _dc)
{
	HPEN Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN Old = (HPEN)SelectObject(_dc, Pen);

	MoveToEx(_dc, int(m_vPos.x - m_vScale.x), int(m_vPos.y - m_vScale.y), NULL);
	LineTo(_dc, int(m_vPos.x + m_vScale.x), int(m_vPos.y - m_vScale.y));
	LineTo(_dc, int(m_vPos.x + m_vScale.x), int(m_vPos.y + m_vScale.y));
	LineTo(_dc, int(m_vPos.x - m_vScale.x), int(m_vPos.y + m_vScale.y));
	LineTo(_dc, int(m_vPos.x - m_vScale.x), int(m_vPos.y - m_vScale.y));

	DeleteObject(Pen);
	SelectObject(_dc, Old);
}

CObj::CObj()
	:m_vPos{ 0.f,0.f }
	, m_vOffset(0,0)
{
}

CObj::~CObj()
{
}
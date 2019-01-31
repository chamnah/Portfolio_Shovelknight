#include "stdafx.h"
#include "CDragUI.h"
#include "CKeyMgr.h"

CDragUI::CDragUI()
{
	m_Pen = CreatePen(PS_NONE, 0, RGB(255, 255, 255));
	m_Brush = CreateSolidBrush(RGB(30, 30, 30));
	m_vCurMousePos = Vec2(0, 0);
}

CDragUI::~CDragUI()
{
	DeleteObject(m_Brush);
	DeleteObject(m_Pen);
}

void CDragUI::MouseDown()
{
	m_vCurMousePos = CKeyMgr::GetInst()->GetMousePos();
}

void CDragUI::Init()
{
	m_vScale = Vec2(m_pParent->GetScale().x, 25.f);
}

int CDragUI::update()
{
	if (m_bMouseOnLB) // Stage cpp에서 체크를 하고 있다. 계속 누르고 있는 상태이다.
	{
		Vec2 CurPos = CKeyMgr::GetInst()->GetMousePos();
		Vec2 ParentPos = m_pParent->GetPos();
		m_pParent->SetPos(ParentPos + (CurPos - m_vCurMousePos));
		m_vCurMousePos = CurPos;
	}
	CUI::update();
	return 0;
}

void CDragUI::render(HDC _dc)
{
	HPEN   OldPen = (HPEN)SelectObject(_dc, m_Pen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

	Rectangle(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

	SelectObject(_dc, OldBrush);
	SelectObject(_dc, OldPen);
}
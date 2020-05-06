#include "stdafx.h"
#include "CCollBtnUI.h"
#include "CMouseEventMgr.h"

CCollBtnUI::CCollBtnUI(TILE_TYPE _State)
	:m_eMode(MOUSE_MODE::NONE)
{
	m_Pen = CreatePen(PS_NONE, 2, RGB(0, 255, 0));
	m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	m_TempBrush = CreateSolidBrush(RGB(30, 255, 30));;
	m_Font = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0,NULL);
	m_eState = _State;
	m_vScale = Vec2(48,48);
}

CCollBtnUI::~CCollBtnUI()
{
	DeleteObject(m_Pen);
	DeleteObject(m_Font);
	DeleteObject(m_TempBrush);
}

void CCollBtnUI::render(HDC _dc)
{
	HPEN   OldPen = NULL;	
	HBRUSH OldBrush = NULL; ;
	HFONT  OldFont = NULL;

	if (m_eState == TILE_TYPE::NONE)
	{
		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

		Ellipse(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
	else if(m_eState == TILE_TYPE::COLL)
	{
		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

		MoveToEx(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, NULL);
		LineTo(_dc, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

		MoveToEx(_dc, int(m_vFinalPos.x + m_vScale.x), (int)m_vFinalPos.y, NULL);
		LineTo(_dc, (int)m_vFinalPos.x, int(m_vFinalPos.y + m_vScale.y));

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
	else if (m_eState == TILE_TYPE::COPY)
	{
		if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_COPY)
			m_Brush = m_TempBrush;
		else if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_PICK)
			m_Brush = (HBRUSH)GetStockObject(NULL_BRUSH);

		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);
		OldFont = (HFONT)SelectObject(_dc, m_Font);

		Rectangle(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

		RECT rt = { (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y) };
		DrawText(_dc, L"COPY", -1, &rt, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		SelectObject(_dc, OldFont);
		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
	else if (m_eState == TILE_TYPE::LADDER)
	{
		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

		MoveToEx(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, NULL);
		LineTo(_dc, (int)m_vFinalPos.x, int(m_vFinalPos.y + m_vScale.y));

		MoveToEx(_dc, int(m_vFinalPos.x + m_vScale.x), (int)m_vFinalPos.y, NULL);
		LineTo(_dc, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

		MoveToEx(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y + m_vScale.y / 2.f, NULL);
		LineTo(_dc, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y / 2.f));

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
	else if (m_eState == TILE_TYPE::NODE)
	{
		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

		Rectangle(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
	else if (m_eState == TILE_TYPE::TRAP)
	{
		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

		MoveToEx(_dc, (int)m_vFinalPos.x + m_vScale.x / 2.f, (int)m_vFinalPos.y, NULL);
		LineTo(_dc, (int)m_vFinalPos.x, int(m_vFinalPos.y + m_vScale.y));
		LineTo(_dc, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));
		LineTo(_dc, (int)m_vFinalPos.x + m_vScale.x / 2.f, (int)m_vFinalPos.y);

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
	else if (m_eState == TILE_TYPE::HIDDEN)
	{
		OldPen = (HPEN)SelectObject(_dc, m_Pen);
		OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

		MoveToEx(_dc, (int)m_vFinalPos.x + m_vScale.x / 2.f, (int)m_vFinalPos.y, NULL);
		LineTo(_dc, (int)m_vFinalPos.x, int(m_vFinalPos.y + m_vScale.y / 2.f));
		LineTo(_dc, (int)m_vFinalPos.x + m_vScale.x / 2.f, int(m_vFinalPos.y + m_vScale.y));
		LineTo(_dc, (int)m_vFinalPos.x + m_vScale.x, int(m_vFinalPos.y + m_vScale.y / 2.f));
		LineTo(_dc, (int)m_vFinalPos.x + m_vScale.x / 2.f, (int)m_vFinalPos.y);

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}
}

void CCollBtnUI::MouseUp()
{
	if (m_eState == TILE_TYPE::NONE)
	{
		CMouseEventMgr::GetInst()->SetTileType(TILE_TYPE::NONE);
	}
	else if (m_eState == TILE_TYPE::COLL)
	{
		CMouseEventMgr::GetInst()->SetTileType(TILE_TYPE::COLL);
	}
	else if (m_eState == TILE_TYPE::LADDER)
	{
		CMouseEventMgr::GetInst()->SetTileType(TILE_TYPE::LADDER);
	}
	else if (m_eState == TILE_TYPE::COPY)
	{
		if(CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_COPY) // 이미 카피 모드였는데 다시 누르면 꺼야 된다.
			CMouseEventMgr::GetInst()->SetMode(MOUSE_MODE::TILE_PICK);
		else
			CMouseEventMgr::GetInst()->SetMode(MOUSE_MODE::TILE_COPY);
	}
	else if (m_eState == TILE_TYPE::NODE)
	{
		CMouseEventMgr::GetInst()->SetTileType(TILE_TYPE::NODE);
	}
	else if (m_eState == TILE_TYPE::TRAP)
	{
		CMouseEventMgr::GetInst()->SetTileType(TILE_TYPE::TRAP);
	}
	else if (m_eState == TILE_TYPE::HIDDEN)
	{
		CMouseEventMgr::GetInst()->SetTileType(TILE_TYPE::HIDDEN);
	}
}
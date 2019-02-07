#include "stdafx.h"
#include "CLayerPanelUI.h"

CLayerPanelUI::CLayerPanelUI()
{
	m_eUIType = UI_TYPE::LAYER;
	m_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	m_Brush = CreateSolidBrush(RGB(45, 45, 48));
}

CLayerPanelUI::~CLayerPanelUI()
{
	DeleteObject(m_Brush);
	DeleteObject(m_Pen);
}

void CLayerPanelUI::render(HDC _dc)
{
	HPEN   OldPen = (HPEN)SelectObject(_dc, m_Pen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

	Rectangle(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

	SelectObject(_dc, OldBrush);
	SelectObject(_dc, OldPen);

	ChildRender(_dc);
}
#include "stdafx.h"
#include "CCategoryUI.h"
#include "CPanelUI.h"
#include "CResMgr.h"

CCategoryUI::CCategoryUI(UI_CATEGORY _eCategory)
{
	m_eCategory = _eCategory;
	m_strName = L"";
	m_Pen = CreatePen(PS_NONE, 0, RGB(255, 255, 255));
	m_Brush = CreateSolidBrush(RGB(30, 30, 30));
	m_vScale = Vec2(70,25);
}

CCategoryUI::~CCategoryUI()
{
	DeleteObject(m_Brush);
	DeleteObject(m_Pen);
}

void CCategoryUI::render(HDC _dc)
{
	HPEN   OldPen = (HPEN)SelectObject(_dc, m_Pen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(_dc, m_Brush);

	Rectangle(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

	SelectObject(_dc, OldBrush);
	SelectObject(_dc, OldPen);

	SetBkMode(_dc,TRANSPARENT);
	SetTextColor(_dc, RGB(255, 255, 255));
	TextOut(_dc, (int)m_vFinalPos.x + 5, (int)m_vFinalPos.y + 5, m_strName.c_str(),m_strName.size());
}

void CCategoryUI::MouseUp()
{
	if (m_eCategory == UI_CATEGORY::TILE1)
	{
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::LIST);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::BUTTON);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::ARROW);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::VIEW);
		((CPanelUI*)m_pParent)->SetArrow(60);
		((CPanelUI*)m_pParent)->SetCollBtn(70);
		((CPanelUI*)m_pParent)->SetBtn((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level1"));
	}
	else if (m_eCategory == UI_CATEGORY::TILE2)
	{
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::LIST);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::BUTTON);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::ARROW);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::VIEW);
		((CPanelUI*)m_pParent)->SetArrow(60);
		((CPanelUI*)m_pParent)->SetCollBtn(70);
		((CPanelUI*)m_pParent)->SetBtn((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level2"));
	}
	else if (m_eCategory == UI_CATEGORY::OBJECT)
	{
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::BUTTON);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::COLL);
		((CPanelUI*)m_pParent)->ClearUI(UI_TYPE::ARROW);
		((CPanelUI*)m_pParent)->SetList();
	}
}
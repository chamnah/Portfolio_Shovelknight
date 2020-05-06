#include "stdafx.h"
#include "CListUI.h"
#include "CKeyMgr.h"
#include "CMouseEventMgr.h"

CListUI::CListUI()
	:m_strName(L"")
	, m_bSelect(false)
{
	m_vScale = Vec2(250,30);
}

CListUI::~CListUI()
{
}

int CListUI::update()
{
	CUI::update();

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY))
		m_bSelect = false;
	return 0;
}

void CListUI::render(HDC _dc)
{
	if (m_bSelect)
	{
		HPEN OldPen = (HPEN)SelectObject(_dc, GetStockObject(DC_PEN));
		HBRUSH OldBrush = (HBRUSH)SelectObject(_dc, GetStockObject(DC_BRUSH));
		SetDCPenColor(_dc, RGB(255, 255, 255));
		SetDCBrushColor(_dc, RGB(30, 30, 30));

		Rectangle(_dc, m_vFinalPos.x, m_vFinalPos.y, m_vFinalPos.x + m_vScale.x, m_vFinalPos.y + m_vScale.y);

		SelectObject(_dc, OldPen);
		SelectObject(_dc, OldBrush);
	}

	RECT rt = { m_vFinalPos.x + 10,m_vFinalPos.y + 5,m_vFinalPos.x + 290,m_vFinalPos.y + 25 };

	DrawText(_dc,m_strName.c_str(),-1,&rt,DT_LEFT | DT_VCENTER);
	//TextOut(_dc,m_vFinalPos.x,m_vFinalPos.y, m_strName.c_str(),wcslen(m_strName.c_str()));
}

void CListUI::MouseUp()
{
	m_bSelect = true;

	for (int i = 0; i < m_pParent->GetChildUI(UI_TYPE::VIEW).size(); ++i)
	{
		m_pParent->GetChildUI(UI_TYPE::VIEW)[i]->SetSize(m_pTex->GetWidth(),m_pTex->GetHeight());
		m_pParent->GetChildUI(UI_TYPE::VIEW)[i]->SetTexture(m_pTex);
	}
	CMouseEventMgr::GetInst()->SetMode(MOUSE_MODE::OBJ_PICK);
	CMouseEventMgr::GetInst()->SetObjectName(m_strName);
	CMouseEventMgr::GetInst()->SetTexture(m_pTex);
}

// 현재 이 리스트가 선택이 되었다면 배경 색깔이 바뀌어야 한다. 
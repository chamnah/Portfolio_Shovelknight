#include "stdafx.h"
#include "CInvenUI.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CItem.h"
#include "CTimeMgr.h"
#include "CGameMgr.h"

CInvenUI::CInvenUI()
	:m_iTab(0)
	, m_iCursorPos(0)
{
	m_eUIType = UI_TYPE::INVEN;
	m_pTex = TEX_LOAD(L"Inven",L"Image\\inven.bmp");
	m_pCursorTex = TEX_LOAD(L"Cursor",L"Image\\InvenCursor.bmp");
	m_vScale = Vec2(m_pTex->GetWidth() / 2,m_pTex->GetHeight());
}

CInvenUI::~CInvenUI()
{
	CTimeMgr::GetInst()->SetStop(false);
}

void CInvenUI::Init()
{
	CTimeMgr::GetInst()->SetStop(true);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			CItem Item;
			Item.SetPos(m_vPos.x + 120 + ((m_pCursorTex->GetWidth() * 3.6f) + 50) * j, m_vPos.y + 190 + (m_pCursorTex->GetHeight() * 3.6f) * i);
			Item.SetItemType((int)ITEM_TYPE::NONE);
			m_vecLeft.push_back(Item);
		}
	}

	m_vecLeft[0].SetItemType((int)ITEM_TYPE::WAND);
	CTexture* pTex = TEX_LOAD(L"Armor", L"Image\\Armor.bmp");

	for (int j = 0; j < 5; ++j)
	{
		CItem Item;
		Item.SetTexture(pTex);
		Item.SetPos(m_vPos.x + 120 + ((m_pCursorTex->GetWidth() * 3.6f) + 50) * j, m_vPos.y + 230);
		Item.SetItemType((int)EQUIP_TYPE::END);
		Item.SetScale(Vec2((UINT)84, pTex->GetHeight()));
		Item.SetOriRender(true);
		m_vecRight.push_back(Item);
	}

	m_vecRight[0].SetItemType((int)CGameMgr::GetInst()->GetEquipment());
}

int CInvenUI::update()
{
	CUI::update();

	if (KEY(KEY_TYPE::KEY_Z, KEY_STATE::TAB))
	{
		m_iCursorPos = 0;
		m_iTab += 1;
		if (m_iTab > 1)
			m_iTab = 0;
	}

	if (KEY(KEY_TYPE::KEY_RIGHT, KEY_STATE::TAB))
	{
		m_iCursorPos += 1;

		if (m_iTab == 0 && m_iCursorPos > 9)
			m_iCursorPos = 0;
		else if(m_iTab == 1 && m_iCursorPos > 4)
			m_iCursorPos = 0;
	}
	else if (KEY(KEY_TYPE::KEY_LEFT, KEY_STATE::TAB))
	{
		m_iCursorPos -= 1;
		
		if (m_iTab == 0 && m_iCursorPos < 0)
			m_iCursorPos = 9;
		else if (m_iTab == 1 && m_iCursorPos < 0)
			m_iCursorPos = 4;
	}
	else if (m_iTab == 0 && KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB))
	{
		m_iCursorPos -= 5;
		if (m_iCursorPos < 0)
			m_iCursorPos += 10;
	}
	else if (m_iTab == 0 && KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
	{
		m_iCursorPos += 5;
		if (m_iCursorPos > 9)
			m_iCursorPos -= 10;
	}

	if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
	{
		if(!m_iTab)
			CGameMgr::GetInst()->SetItemType((ITEM_TYPE)m_vecLeft[m_iCursorPos].GetItemType());
	}

	return 0;
}

void CInvenUI::render(HDC _dc)
{
	BitBlt(_dc,m_vFinalPos.x,m_vFinalPos.y,m_vScale.x,m_vScale.y,m_pTex->GetDC(), m_vScale.x * m_iTab, 0,SRCCOPY);
	if (m_iTab)
	{
		for (UINT i = 0; i < m_vecRight.size(); ++i)
			m_vecRight[i].render(_dc);
		TransparentBlt(_dc, m_vecRight[m_iCursorPos].GetPos().x - m_pCursorTex->GetWidth() * 1.8f, m_vecRight[m_iCursorPos].GetPos().y - m_pCursorTex->GetHeight() * 1.8f, m_pCursorTex->GetWidth() * 3.6f, m_pCursorTex->GetHeight() * 3.6f, m_pCursorTex->GetDC(),
			0, 0, m_pCursorTex->GetWidth(), m_pCursorTex->GetHeight(), RGB(0, 255, 0));
	}
	else
	{
		for (UINT i = 0; i < m_vecLeft.size(); ++i)
			m_vecLeft[i].render(_dc);
		TransparentBlt(_dc, m_vecLeft[m_iCursorPos].GetPos().x - m_pCursorTex->GetWidth() * 1.8f, m_vecLeft[m_iCursorPos].GetPos().y - m_pCursorTex->GetHeight() * 1.8f, m_pCursorTex->GetWidth() * 3.6f, m_pCursorTex->GetHeight() * 3.6f, m_pCursorTex->GetDC(),
			0, 0, m_pCursorTex->GetWidth(), m_pCursorTex->GetHeight(), RGB(0, 255, 0));
	}
}
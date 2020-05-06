#include "stdafx.h"
#include "CStoreUI.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CItem.h"
#include "CGameMgr.h"
#include "CScoreUI.h"
#include "CArmorer.h"
#include "CCamMgr.h"

CStoreUI::CStoreUI()
	:m_iCursorPos(0)
	, m_pCursorTex(nullptr)
	, m_pNPC(nullptr)
{
	m_pTex = TEX_LOAD(L"Buy", L"Image\\Buy.bmp");
	m_pCursorTex = TEX_LOAD(L"StoreSelect", L"Image\\StoreSelect.bmp");
	m_hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"DungGeunMo");


}

CStoreUI::~CStoreUI()
{
	CCamMgr::GetInst()->SetStop(false);
	CGameMgr::GetInst()->SetPlayerStop(false);

	for (UINT i = 0; i < m_vecStore.size(); ++i)
	{
		delete m_vecStore[i];
		m_vecStore[i] = nullptr;
	}
	DeleteObject(m_hFont);
}

int CStoreUI::update()
{
	CUI::update();
	if (KEY(KEY_TYPE::KEY_LEFT, KEY_STATE::TAB))
	{
		m_iCursorPos -= 1;
		if (m_iCursorPos < 0)
			m_iCursorPos = 7;
	}
	else if (KEY(KEY_TYPE::KEY_RIGHT, KEY_STATE::TAB))
	{
		m_iCursorPos += 1;
		if (m_iCursorPos > 7)
			m_iCursorPos = 0;

	}
	else if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB))
	{
		m_iCursorPos -= 4;
		if (m_iCursorPos < 0)
			m_iCursorPos += 8;
	}
	else if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
	{
		m_iCursorPos += 4;
		if (m_iCursorPos > 7)
			m_iCursorPos -= 8;
	}
	else if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
	{
		if (m_iCursorPos == 7)
		{
			((CArmorer*)m_pNPC)->SetView(false);
			CCamMgr::GetInst()->SetStop(false);
			CGameMgr::GetInst()->SetPlayerStop(false);
			return INT_MAX;
		}
		else if (m_iCursorPos == 6)
			return 0;
		if (CGameMgr::GetInst()->GetGold() >= m_vecStore[m_iCursorPos]->GetPrice())
		{
			CGameMgr::GetInst()->SetGold(CGameMgr::GetInst()->GetGold() - m_vecStore[m_iCursorPos]->GetPrice());
			CGameMgr::GetInst()->SetEquipment((EQUIP_TYPE)m_vecStore[m_iCursorPos]->GetItemType());
			CGameMgr::GetInst()->SetEquipSetting();
			CGameMgr::GetInst()->SetEquipPrice(m_iCursorPos,0);
			m_vecStore[m_iCursorPos]->SetPrice(0);
		}
	}
	return 0;
}

void CStoreUI::render(HDC _dc)
{
	HFONT OldFont = (HFONT)SelectObject(_dc, m_hFont);
	BitBlt(_dc, m_vFinalPos.x, m_vFinalPos.y, m_pTex->GetWidth(), m_pTex->GetHeight(), m_pTex->GetDC(), 0, 0, SRCCOPY);

	for (UINT i = 0; i < m_vecStore.size() - 2; ++i)
	{
		m_vecStore[i]->render(_dc);

		wchar_t wcsPrice[20];
		_itow_s(m_vecStore[i]->GetPrice(), wcsPrice,20,10 );

		RECT rt = { m_vecStore[i]->GetPos().x - m_vecStore[i]->GetScale().x / 2.f,m_vecStore[i]->GetPos().y + m_vecStore[i]->GetScale().y / 2.f,
			m_vecStore[i]->GetPos().x - m_vecStore[i]->GetScale().x / 2.f + m_vecStore[i]->GetScale().x,m_vecStore[i]->GetPos().y + m_vecStore[i]->GetScale().y / 2.f + 40 };

		DrawText(_dc, wcsPrice, -1,&rt ,DT_CENTER);
	}

	TransparentBlt(_dc, m_vecStore[m_iCursorPos]->GetPos().x - m_pCursorTex->GetWidth() / 2.f, m_vecStore[m_iCursorPos]->GetPos().y - m_pCursorTex->GetHeight() / 2.f + 15, 
		m_pCursorTex->GetWidth(), m_pCursorTex->GetHeight(), m_pCursorTex->GetDC(),
		0, 0, m_pCursorTex->GetWidth(), m_pCursorTex->GetHeight(), RGB(0, 255, 0));

	RECT rt = { m_vecStore[m_vecStore.size() - 1]->GetPos().x - m_pCursorTex->GetWidth() / 2.f, m_vecStore[m_vecStore.size() - 1]->GetPos().y - m_pCursorTex->GetHeight() / 2.f + 15,
		m_vecStore[m_vecStore.size() - 1]->GetPos().x - m_pCursorTex->GetWidth() / 2.f + m_pCursorTex->GetWidth(), m_vecStore[m_vecStore.size() - 1]->GetPos().y - m_pCursorTex->GetHeight() / 2.f + 15 + m_pCursorTex->GetHeight() };

	SetTextColor(_dc,RGB(255,0,0));
	DrawText(_dc, L"EXIT", -1, &rt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	SetTextColor(_dc, RGB(255, 255, 255));
	SelectObject(_dc, OldFont);

	ChildRender(_dc);
}

void CStoreUI::SetArmorStore()
{
	CTexture* pTex = TEX_LOAD(L"Armor", L"Image\\Armor.bmp");
	
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			CItem* pItem = new CItem;
			pItem->SetOriRender(true);
			pItem->SetTexture(pTex);
			if(((i * 4) + j) <= 5)
			pItem->SetPrice(CGameMgr::GetInst()->GetEquipPrice(((i * 4) + j)));
			pItem->SetPos(Vec2(400 + (j * 130), 120 + (i * 130)));
			pItem->SetItemType((i * 4 + j));
			pItem->SetScale(Vec2((UINT)84, pTex->GetHeight()));
			m_vecStore.push_back(pItem);
		}
	}

	CScoreUI* pScore = new CScoreUI;
	pScore->SetPos(80,150);
	m_vecChildUI[(UINT)UI_TYPE::NONE].push_back(pScore);

	CUI* pUI = new CUI;
	pUI->SetPos(30, 150);
	pUI->SetTexture(TEX_LOAD(L"Gold", L"Image\\Gold.bmp"));
	m_vecChildUI[(UINT)UI_TYPE::NONE].push_back(pUI);

	pUI = new CUI;
	pUI->SetPos(100,200);
	pUI->SetTexture(TEX_LOAD(L"Face1",L"Image\\Face1.bmp"));
	m_vecChildUI[(UINT)UI_TYPE::NONE].push_back(pUI);
}
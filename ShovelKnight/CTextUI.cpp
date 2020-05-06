#include "stdafx.h"
#include "CTextUI.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CArmorer.h"
#include "CCamMgr.h"
#include "CGameMgr.h"
#include "CStageMgr.h"
#include "CViewUI.h"
#include "CStoreUI.h"

CTextUI::CTextUI()
	:m_iNext(0)
	, m_fTime(0.f)
	, m_iText(0)
	, m_bBuy(false)
	, m_bSelect(true)
	, m_bView(false)
	, m_tYesPos{}
	, m_tNoPos{}
	, m_pNPC(nullptr)
	, m_pFSM(nullptr)
	, m_pFace(nullptr)
{
	m_hFont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"DungGeunMo");
	m_vOffset = Vec2(250,50);
	m_pTex = TEX_LOAD(L"SelectCursor",L"Image\\SelectCursor.bmp");

	m_tYesPos = tSize(300, 130);
	m_tNoPos = tSize(500, 130);
	//CCamMgr::GetInst()->SetStop(true);
	CGameMgr::GetInst()->SetPlayerStop(true);
}

CTextUI::~CTextUI()
{
	DeleteObject(m_hFont);
	if (!m_bView || !m_bSelect)
	{
		if(m_pNPC != nullptr)
		m_pNPC->SetView(false);
		if (m_pFSM != nullptr)
			*m_pFSM = BOSS_FSM::BLOW;
		CCamMgr::GetInst()->SetStop(false);
		CGameMgr::GetInst()->SetPlayerStop(false);
	}
}

int CTextUI::update()
{
	m_fTime += DT;

	CUI::update();

	if (m_vecText.empty())
		return 0;

	if (m_vecText.size() <= m_iNext)
	{
		m_iNext = 0;
		return INT_MAX;
	}

	if (m_pFace != nullptr)
	{
		m_pFace->SetTexture(m_vecText[m_iNext].pTex);
	}

	if (m_vecText[m_iNext].wcsText.length() <= m_iText)
	{
		if (m_bBuy &&m_vecText.size() - 1 == m_iNext)
		{
			m_bView = true;
			if (KEY(KEY_TYPE::KEY_LEFT, KEY_STATE::TAB))
				m_bSelect = !m_bSelect;
			else if (KEY(KEY_TYPE::KEY_RIGHT, KEY_STATE::TAB))
				m_bSelect = !m_bSelect;
			
			if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
			{
				m_wcsText = L"";
				m_iNext += 1;
				m_iText = 0;

				if (m_bSelect)
				{
					CStoreUI* pStore = new CStoreUI;
					pStore->SetArmorStore();
					CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI].push_back(pStore);
					if (m_pNPC != nullptr)
					pStore->SetNPC(m_pNPC);

					return SKIP;
				}
			}
		}
		else
		{
			if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
			{
				m_wcsText = L"";
				m_iNext += 1;
				m_iText = 0;
			}
		}
		return 0;
	}
	else
	{
		if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
		{
			m_wcsText = m_vecText[m_iNext].wcsText.c_str();
			m_iText = m_vecText[m_iNext].wcsText.length();
		}
	}
	if (m_fTime > 0.1f)
	{
		m_wcsText += m_vecText[m_iNext].wcsText.c_str()[m_iText];
		++m_iText;
		m_fTime = 0.f;
	}

	return 0;
}

void CTextUI::render(HDC _dc)
{
	if (m_vecText.empty())
		return;
	HFONT OldFont = (HFONT)SelectObject(_dc, m_hFont);
	
	RECT rt = RECT{int(m_vFinalPos.x),int(m_vFinalPos.y),1200,1000};
	DrawText(_dc, m_wcsText.c_str(),-1,&rt, DT_LEFT | DT_WORDBREAK);
	
	if (m_bView)
	{
		TextOut(_dc, m_tYesPos.x, m_tYesPos.y,L"네",1);
		TextOut(_dc, m_tNoPos.x, m_tNoPos.y, L"아니오", 3);

		tSize tPos = {};
		if (m_bSelect)
		{
			tPos = m_tYesPos;
			BitBlt(_dc, tPos.x + 40, tPos.y - 20, 24, 24, m_pTex->GetDC(), 0, 0, SRCCOPY);
			BitBlt(_dc, tPos.x + 40, tPos.y + 30, 24, 24, m_pTex->GetDC(), 48, 0, SRCCOPY);
		}
		else
		{
			tPos = m_tNoPos;
			BitBlt(_dc, tPos.x + 120, tPos.y - 20, 24, 24, m_pTex->GetDC(), 0, 0, SRCCOPY);
			BitBlt(_dc, tPos.x + 120, tPos.y + 30, 24, 24, m_pTex->GetDC(), 48, 0, SRCCOPY);
		}

		BitBlt(_dc, tPos.x - 20, tPos.y - 20, 24, 24, m_pTex->GetDC(), 24, 0, SRCCOPY);
		BitBlt(_dc, tPos.x - 20, tPos.y + 30, 24, 24, m_pTex->GetDC(), 72, 0, SRCCOPY);
	}
	
	SelectObject(_dc, OldFont);

	ChildRender(_dc);
}

void CTextUI::AddText(const wstring & _wcsText, CTexture * _pTex)
{
	tText Text = { _wcsText ,_pTex };
	m_vecText.push_back(Text);
}
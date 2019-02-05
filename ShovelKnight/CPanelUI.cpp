#include "stdafx.h"
#include "CPanelUI.h"
#include "CResMgr.h"
#include "CBtnUI.h"
#include "CPrevBtn.h"
#include "CKeyMgr.h"
#include "CNextBtn.h"
#include "CDragUI.h"
#include "CCategoryUI.h"
#include "CCollBtnUI.h"
#include "CListUI.h"
#include "CViewUI.h"
#include "CStageMgr.h"

CPanelUI::CPanelUI()
	:m_pBtnTex(nullptr)
{
	m_vScale = Vec2(500, 600);
	m_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	m_Brush = CreateSolidBrush(RGB(45, 45, 48));

	m_ptCurBtnSize = POINT{0,0};
	m_tPadding = tRect(10, 60, 60, 10);
}

CPanelUI::~CPanelUI()
{
	DeleteObject(m_Brush);
	DeleteObject(m_Pen);
}

void CPanelUI::render(HDC _dc)
{
	HPEN   OldPen = (HPEN)SelectObject(_dc,m_Pen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(_dc,m_Brush);
	
	Rectangle(_dc,(int)m_vFinalPos.x, (int)m_vFinalPos.y, int(m_vFinalPos.x + m_vScale.x), int(m_vFinalPos.y + m_vScale.y));

	SelectObject(_dc, OldBrush);
	SelectObject(_dc, OldPen);

	ChildRender(_dc);

}

void CPanelUI::PrevPage()
{
	int iCurPageMax = m_ptCurBtnSize.x * m_ptCurBtnSize.y;

	int iCol = m_pBtnTex->GetWidth() / TILE_SIZE;
	int iRow = (m_pBtnTex->GetHeight() / TILE_SIZE);

	int iMaxPage = (iCol * iRow);
	m_iCurPage -= iCurPageMax;

	if (m_iCurPage < 0)
		m_iCurPage = 0;

	for (int i = 0; i < iCurPageMax; ++i)
	{
		((CBtnUI*)m_vecChildUI[(UINT)UI_TYPE::BUTTON][i])->SetIndex(m_iCurPage + i);
	}
}

void CPanelUI::NextPage()
{
	int iCurPageMax = m_ptCurBtnSize.x * m_ptCurBtnSize.y;

	int iCol = m_pBtnTex->GetWidth() / TILE_SIZE;
	int iRow = m_pBtnTex->GetHeight() / TILE_SIZE;

	int iMaxPage = iCol * iRow;
	m_iCurPage += iCurPageMax;

	if (m_iCurPage > iMaxPage)
		m_iCurPage -= iCurPageMax;

	for (int i = 0; i < iCurPageMax; ++i)
	{
		((CBtnUI*)m_vecChildUI[(UINT)UI_TYPE::BUTTON][i])->SetIndex(m_iCurPage + i);
	}
}

void CPanelUI::Init()
{
	CDragUI* pDrag = new CDragUI;
	pDrag->SetPos(0, 0);
	AddChildUI(UI_TYPE::NONE, pDrag);
	pDrag->Init();

	CCategoryUI* pCategory = new CCategoryUI(UI_CATEGORY::TILE1);
	pCategory->SetPos(10, 30);
	pCategory->SetName(L"Tile1");
	AddChildUI(UI_TYPE::NONE, pCategory);

	pCategory = new CCategoryUI(UI_CATEGORY::TILE2);
	pCategory->SetPos(10 + pCategory->GetScale().x + 20, 30);
	pCategory->SetName(L"Tile2");
	AddChildUI(UI_TYPE::NONE, pCategory);

	pCategory = new CCategoryUI(UI_CATEGORY::OBJECT);
	pCategory->SetPos(10 + (pCategory->GetScale().x + 20) * 2, 30);
	pCategory->SetName(L"Object");
	AddChildUI(UI_TYPE::NONE, pCategory);

	SetArrow(60.f);
	SetCollBtn(70.f);
}

void CPanelUI::SetBtn(CTexture* _pTex)
{
	m_iCurPage = 0;
	m_pBtnTex = _pTex;
	int iWidth = int(m_vScale.x - (m_tPadding.right + m_tPadding.left)) / TILE_SIZE;
	int iHeight = int(m_vScale.y - (m_tPadding.bottom + m_tPadding.top)) / TILE_SIZE;
	
	for (int i = 0; i < iHeight; ++i)
	{
		for (int j = 0; j < iWidth; ++j)
		{
			CBtnUI* pBtn = new CBtnUI;
			pBtn->SetTexture(_pTex);
			pBtn->Init();
			pBtn->SetPos(m_tPadding.left + (TILE_SIZE * j), m_tPadding.top + (TILE_SIZE * i));
			pBtn->SetIndex(i * iWidth + j);
			AddChildUI(UI_TYPE::BUTTON,pBtn);
		}
	}

	m_ptCurBtnSize.x = iWidth;
	m_ptCurBtnSize.y = iHeight;
}

void CPanelUI::SetList()
{
	wstring Path = L"Image\\Tool_View\\";
	wstring Name = L"";

	for(UINT i = 0; i < m_vecFile.size(); ++i)
	{ 
		CListUI* pList = new CListUI;
		Name = L"";
		pList->SetPos(30,80 + i * pList->GetScale().y);

		for (size_t j = 0; j < m_vecFile[i].length(); ++j)
		{
			if (m_vecFile[i].c_str()[j] == L'.')
				break;

			Name += m_vecFile[i].c_str()[j];
		}

		pList->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(Name, Path + m_vecFile[i]));
		pList->SetText(Name);
		AddChildUI(UI_TYPE::LIST, pList);
	}
	CViewUI* pView = new CViewUI;
	pView->SetPos(380,100);
	pView->SetScale(Vec2(100.f, 100.f));
	AddChildUI(UI_TYPE::VIEW, pView);

	SetArrow(210.f);
}

void CPanelUI::SetArrow(float _fPosX)
{
	CPrevBtn* pPrev = new CPrevBtn;
	pPrev->SetPos(m_vScale.x - _fPosX, 70.f);
	AddChildUI(UI_TYPE::ARROW, pPrev);

	CNextBtn* pNext = new CNextBtn;
	pNext->SetPos(m_vScale.x - _fPosX, m_vScale.y - 110);
	AddChildUI(UI_TYPE::ARROW, pNext);
}

void CPanelUI::SetCollBtn(float _fPosY)
{
	CCollBtnUI* pColl = new CCollBtnUI(TILE_TYPE::NONE);
	pColl->SetPos(m_vScale.x - 70.f, _fPosY + 80.f);
	AddChildUI(UI_TYPE::COLL, pColl);

	pColl = new CCollBtnUI(TILE_TYPE::COLL);
	pColl->SetPos(m_vScale.x - 70.f, _fPosY + 80.f + pColl->GetScale().y + 10.f);
	AddChildUI(UI_TYPE::COLL, pColl);

	pColl = new CCollBtnUI(TILE_TYPE::COPY);
	pColl->SetPos(m_vScale.x - 90.f, _fPosY + 80.f + pColl->GetScale().y + 10.f + pColl->GetScale().y + 10.f);
	pColl->SetScale(Vec2(85, 50));
	AddChildUI(UI_TYPE::COLL, pColl);
}
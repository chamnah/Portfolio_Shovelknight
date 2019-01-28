#include "stdafx.h"
#include "CBtnUI.h"
#include "CResMgr.h"
#include "CMouseEventMgr.h"
#include "CCore.h"

CBtnUI::CBtnUI()
	:m_iIdx(0)
	, m_bSelect(false)
{
}

CBtnUI::~CBtnUI()
{
	DeleteObject(m_Pen);
}

void CBtnUI::Init()
{
	m_vScale = Vec2(TILE_SIZE, TILE_SIZE);
	m_iMaxIdx = (m_pTex->GetWidth() / TILE_SIZE) * ((m_pTex->GetHeight() / TILE_SIZE));
	m_Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
}

void CBtnUI::render(HDC _dc)
{ 
	if (m_iIdx == INT_MAX)
		return;
	int iTileCountX = (int)(m_pTex->GetWidth() / (int)TILE_SIZE);

	int iCol = m_iIdx % iTileCountX;
	int iLow = m_iIdx / iTileCountX;

	BitBlt(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, TILE_SIZE, TILE_SIZE, m_pTex->GetDC(), iCol * TILE_SIZE, iLow * TILE_SIZE, SRCCOPY);

	if (m_bIsMouse) // 이건 쓸 코드이다 지우지 말고 여기다가 선택되면 나올것이나 정하자
	{	
		HPEN Old = (HPEN)SelectObject(_dc, m_Pen);
		
		MoveToEx(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, NULL);
		LineTo(_dc, (int)m_vFinalPos.x + TILE_SIZE, (int)m_vFinalPos.y);
		LineTo(_dc, (int)m_vFinalPos.x + TILE_SIZE, (int)m_vFinalPos.y + TILE_SIZE);
		LineTo(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y + TILE_SIZE);
		LineTo(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y);

		SelectObject(_dc, Old);
	}
}

void CBtnUI::SetIndex(int _iIdx)
{
	if (_iIdx < 0 || _iIdx >= m_iMaxIdx)
		m_iIdx = INT_MAX;
	else
		m_iIdx = _iIdx;
}

void CBtnUI::MouseOn()
{
}

void CBtnUI::MouseOut()
{
}

void CBtnUI::MouseUp()
{
	CMouseEventMgr::GetInst()->SetTileIndex(m_iIdx);
	CMouseEventMgr::GetInst()->SetMode(MOUSE_MODE::TILE_PICK);
	CMouseEventMgr::GetInst()->SetTexture(m_pTex);
}

void CBtnUI::MouseDown()
{
}
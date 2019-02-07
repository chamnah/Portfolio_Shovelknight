#include "stdafx.h"
#include "CLayerUI.h"
#include "CCore.h"
#include "CStageMgr.h"

CLayerUI::CLayerUI()
	:m_bSelect(false)
{
	m_vScale = Vec2(300.f,100.f);
	m_hBit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE, CStageMgr::GetInst()->GetTileSizeY() * TILE_SIZE);
	m_hDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	HBITMAP OldBit = (HBITMAP)SelectObject(m_hDC,m_hBit);
	DeleteObject(OldBit);
}

CLayerUI::~CLayerUI()
{
	DeleteDC(m_hDC);
}

void CLayerUI::MouseUp()
{
	m_bSelect = true;
}

void CLayerUI::ChangeDCSize(tSize _Size)
{
	m_hBit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), _Size.x * TILE_SIZE, _Size.y * TILE_SIZE);
	HBITMAP OldBit = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(OldBit);
}

void CLayerUI::render(HDC _dc)
{
	RECT rt = RECT{(int)m_vFinalPos.x,(int)m_vFinalPos.y,int(m_vFinalPos.x + m_vScale.x),int(m_vFinalPos.y + m_vScale.y)};
	DrawText(_dc,L"Layer 0",-1,&rt, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}
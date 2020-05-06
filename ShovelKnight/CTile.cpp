#include "stdafx.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CCore.h"
#include "CCamMgr.h"
#include "CKeyMgr.h"
#include "CCollider.h"
#include "CStageMgr.h"
#include "CStageKing.h"

CTile::CTile(Vec2 _vPos)
{
	m_bDebug = false;
	m_vPos = _vPos;
	Init();

	m_eAttribute = TILE_TYPE::NONE;
	m_eType = OBJ_TYPE::TILE;
	m_iIdx = 0;
	m_vScale = Vec2(TILE_SIZE, TILE_SIZE);
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level1", L"Image\\Level1.bmp");
	m_vRealPos = m_vPos;
	m_TileWidthNum = m_pTex->GetWidth() / TILE_SIZE;
	m_vOffset = Vec2(m_vScale.x / 2, m_vScale.y / 2);
}

CTile::~CTile()
{
}

void CTile::Init()
{
	if(m_eAttribute == TILE_TYPE::COLL || m_eAttribute == TILE_TYPE::LADDER || m_eAttribute == TILE_TYPE::TRAP)
		CCamObj::Init();
}

int CTile::update()
{
	m_vPrePos = m_vPos;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	CCamObj::update();
	return 0;
}

void CTile::DCRender(HDC _dc,Vec2 _vPos)
{
	int iCol = m_iIdx % m_TileWidthNum;
	int iLow = m_iIdx / m_TileWidthNum;

	TransparentBlt(_dc, (int)m_vPos.x + _vPos.x, (int)m_vPos.y + _vPos.y, (int)m_vScale.x, (int)m_vScale.y, m_pTex->GetDC(), TILE_SIZE * iCol, TILE_SIZE * iLow, (int)m_vScale.x, (int)m_vScale.y, RGB(0, 0, 0));
}

void CTile::render(HDC _dc)
{
	Vec2 vScreen = CCamMgr::GetInst()->GetRealPos(0, 0);
	//BitBlt(_dc, 0, 0, (int)CCore::GetInst()->GetResolution().x, (int)CCore::GetInst()->GetResolution().y, m_hDC, -int(vScreen.x), -int(vScreen.y), SRCCOPY);
	if (CStageMgr::GetInst()->GetCurState() == STAGE::KING && (int)(((CStageKing*)CStageMgr::GetInst()->GetCurStage())->GetCourse()) >= (int)STAGE_COURSE::TWO)
		TransparentBlt(_dc, -int(vScreen.x), 0, (int)CCore::GetInst()->GetResolution().x, (int)CCore::GetInst()->GetResolution().y, m_hDC, 0, -int(vScreen.y), (int)CCore::GetInst()->GetResolution().x, (int)CCore::GetInst()->GetResolution().y, RGB(0, 255, 0));
	else
		TransparentBlt(_dc, 0, 0, (int)CCore::GetInst()->GetResolution().x, (int)CCore::GetInst()->GetResolution().y, m_hDC, -int(vScreen.x), -int(vScreen.y), (int)CCore::GetInst()->GetResolution().x, (int)CCore::GetInst()->GetResolution().y, RGB(0, 255, 0));
}

void CTile::renderBit(HDC _dc)
{
	Vec2 vScreen = CCamMgr::GetInst()->GetRealPos(0, 0);
	BitBlt(_dc, 0, 0, (int)CCore::GetInst()->GetResolution().x, (int)CCore::GetInst()->GetResolution().y, m_hDC, -int(vScreen.x), -int(vScreen.y), SRCCOPY);
}

void CTile::BitRender(HDC _dc)
{
	int iCol = m_iIdx %  m_TileWidthNum;
	int iLow = m_iIdx /  m_TileWidthNum;

	BitBlt(_dc, (int)m_vPos.x, (int)m_vPos.y, TILE_SIZE, TILE_SIZE, m_pTex->GetDC(), TILE_SIZE * iCol, TILE_SIZE * iLow, SRCCOPY);
}

void CTile::TypeRender(HDC _dc)
{
	HBRUSH Brush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH OldBrush = (HBRUSH)SelectObject(_dc, Brush);
	HPEN Pen = 0;
	HPEN   OldPen = 0;
	if (m_eAttribute == TILE_TYPE::NONE)
	{
		Pen = CreatePen(PS_NONE, 5, RGB(0, 255, 0));
		OldPen = (HPEN)SelectObject(_dc, Pen);
	}
	else if (m_eAttribute == TILE_TYPE::COLL)
	{
		Pen = CreatePen(PS_NONE, 5, RGB(255, 0, 0));
		OldPen = (HPEN)SelectObject(_dc, Pen);
	}
	else if (m_eAttribute == TILE_TYPE::LADDER)
	{
		Pen = CreatePen(PS_NONE, 5, RGB(0, 0, 255));
		OldPen = (HPEN)SelectObject(_dc, Pen);
	}
	else if (m_eAttribute == TILE_TYPE::NODE)
	{
		Pen = CreatePen(PS_NONE, 5, RGB(255, 228, 0));
		OldPen = (HPEN)SelectObject(_dc, Pen);
	}
	else if (m_eAttribute == TILE_TYPE::TRAP)
	{
		Pen = CreatePen(PS_NONE, 5, RGB(0, 0, 0));
		OldPen = (HPEN)SelectObject(_dc, Pen);
	}
	else if (m_eAttribute == TILE_TYPE::HIDDEN)
	{
		Pen = CreatePen(PS_NONE, 5, RGB(255, 0, 221));
		OldPen = (HPEN)SelectObject(_dc, Pen);
	}

	Ellipse(_dc, (int)m_vPos.x, (int)m_vPos.y, int(m_vPos.x + m_vScale.x), int(m_vPos.y + m_vScale.y));

	SelectObject(_dc, OldPen);
	SelectObject(_dc, OldBrush);

	DeleteObject(Pen);
	DeleteObject(Brush);
}

void CTile::CollRender(HDC _dc)
{
	if (m_pColl != nullptr)
	{
		HPEN OldPen = (HPEN)SelectObject(_dc, m_pColl->GetPen());
		MoveToEx(_dc, int(m_pColl->GetPos().x - m_vScale.x / 2.f), int(m_pColl->GetPos().y - m_vScale.y / 2.f), NULL);
		LineTo(_dc, int(m_pColl->GetPos().x + m_vScale.x / 2.f), int(m_pColl->GetPos().y - m_vScale.y / 2.f));
		LineTo(_dc, int(m_pColl->GetPos().x + m_vScale.x / 2.f), int(m_pColl->GetPos().y + m_vScale.y / 2.f));
		LineTo(_dc, int(m_pColl->GetPos().x - m_vScale.x / 2.f), int(m_pColl->GetPos().y + m_vScale.y / 2.f));
		LineTo(_dc, int(m_pColl->GetPos().x - m_vScale.x / 2.f), int(m_pColl->GetPos().y - m_vScale.y / 2.f));
		SelectObject(_dc, OldPen);
	}
	if (m_bDebug)
		Rectangle(_dc,m_vPos.x, m_vPos.y, m_vPos.x + 64, m_vPos.y + 64);
}

// 현재 Tool에서만 출력을 하고 있다. 최적화 방법은 원본 크기를 키워서 BitBlt로 출력하는 것이다.
//TransparentBlt(_dc, (int)m_vPos.x, (int)m_vPos.y, TILE_SIZE, TILE_SIZE, m_pTex->GetDC(), 0, 0, m_pTex->GetWidth(), m_pTex->GetHeight(), RGB(255, 0, 255));
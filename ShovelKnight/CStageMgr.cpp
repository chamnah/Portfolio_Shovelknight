#include "stdafx.h"
#include "CStageMgr.h"
#include "CStageLogo.h"
#include "CStageStart.h"
#include "CStageTool.h"
#include "CCore.h"
#include "CCollisionMgr.h"
#include "CStageOne.h"
#include "CStageTwo.h"

CStageMgr::CStageMgr()
	:m_arrDelCheck{}
{
}

CStageMgr::~CStageMgr()
{
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);

	for (UINT i = 0; i < (UINT)STAGE::END; ++i)
	{
		delete m_pArrStage[i];
	}
}

void CStageMgr::Init()
{
	m_pArrStage[(UINT)STAGE::LOGO] = new CStageLogo;
	m_pArrStage[(UINT)STAGE::START] = new CStageStart;
	m_pArrStage[(UINT)STAGE::TOOL] = new CStageTool;
	m_pArrStage[(UINT)STAGE::ONE] = new CStageOne;
	m_pArrStage[(UINT)STAGE::TWO] = new CStageTwo;
	m_pCurStage = m_pArrStage[(UINT)STAGE::LOGO];
	m_eCurState = STAGE::LOGO;
	m_pCurStage->Enter();
}

int CStageMgr::Update()
{
	return m_pCurStage->Progress();
}

void CStageMgr::Render(HDC _dc)
{
	m_pCurStage->Render(_dc);
}

void CStageMgr::ChangeStage(STAGE _eStage,DIR _eDir)
{
	if (m_eCurState == _eStage)
		return;

	m_eCurState = _eStage;
	m_pCurStage->Exit();
	ClearCheck();
	m_pCurStage = m_pArrStage[(UINT)_eStage];
	//CCollisionMgr::GetInst()->ResetCheck();
	m_pCurStage->Enter();
}

vector<vector<CObj*>>& CStageMgr::GetObjVector()
{
	return m_pCurStage->GetObjVector();
}

void CStageMgr::EraseStageObj(OBJ_TYPE _Type, CObj* _pObj)
{
	vector<CObj*>::iterator iter = CStageMgr::GetInst()->GetObjVector()[(UINT)_Type].begin();
	for (; iter != CStageMgr::GetInst()->GetObjVector()[(UINT)_Type].end(); ++iter)
	{
		if ((*iter) == _pObj)
		{
			delete _pObj;
			_pObj = nullptr;
			CStageMgr::GetInst()->GetObjVector()[(UINT)_Type].erase(iter);
			break;
		}
	}
}

void CStageMgr::CreateBackGround()
{
	m_iPreTileSize = tSize(m_iTileSizeX, m_iTileSizeY);
	m_hBit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), m_iTileSizeX * TILE_SIZE, m_iTileSizeY * TILE_SIZE);
	m_hDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
    HBITMAP OldBit = (HBITMAP)SelectObject(m_hDC,m_hBit);
	DeleteObject(OldBit);
}

// 이 함수는 꼭 새로운 스테이지에 타일을 읽어오고나서 사용해야 한다.
void CStageMgr::AddBackGround(DIR _eDir)
{
	BITMAP bm{};
	GetObject(m_hBit, sizeof(bm), &bm); // 이거 한 이유는 간단히 크기 알려고

	if (_eDir == DIR::RIGHT)
	{
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), (m_iPreTileSize.x * TILE_SIZE) + (m_iTileSizeX * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE));
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, 0,0, (m_iPreTileSize.x * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE),m_hDC,0,0,SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}
	else if (_eDir == DIR::LEFT)
	{
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), (m_iPreTileSize.x * TILE_SIZE) + (m_iTileSizeX * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE));
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, m_iTileSizeX * TILE_SIZE, 0, (m_iPreTileSize.x * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE), m_hDC, 0, 0, SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}
	else if (_eDir == DIR::UP)
	{
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), bm.bmWidth, bm.bmHeight);
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, 0, 0, bm.bmWidth, bm.bmHeight, m_hDC, 0, 0, SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}
	else if (_eDir == DIR::DOWN)
	{
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), bm.bmWidth + (m_iTileSizeX * TILE_SIZE), bm.bmHeight);
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, 0, 0, bm.bmWidth, bm.bmHeight, m_hDC, 0, 0, SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}

	m_iPreTileSize = tSize(m_iTileSizeX, m_iTileSizeY);
}

void CStageMgr::ArriveBackGround(DIR _eDir)
{
	HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE));
	HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
	DeleteObject(Old);
	if(_eDir == DIR::RIGHT)
		BitBlt(DC, 0, 0, (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE), m_hDC, (m_iTileSizeX * TILE_SIZE), 0, SRCCOPY);
	else if (_eDir == DIR::LEFT)
		BitBlt(DC, 0, 0, (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE), m_hDC, 0, 0, SRCCOPY);
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);
	m_hBit = Bit;
	m_hDC = DC;
}

void CStageMgr::TileRender(HDC _dc) // 내꺼 DC를 출력하는 용도이다.
{
	BitBlt(_dc,0,0,CCore::GetInst()->GetResolution().x, CCore::GetInst()->GetResolution().y,m_hDC,0,0,SRCCOPY);
}

void CStageMgr::CopyRender(HDC _dc,Vec2& _vPos,Vec2& _vTexPos) // 여기서 TransBlt 한 녀석들을 내가 쓸 dc로 복사를 한다.
{
	TransparentBlt(m_hDC,_vPos.x,_vPos.y,TILE_SIZE,TILE_SIZE,_dc, _vTexPos.x, _vTexPos.y,TILE_SIZE,TILE_SIZE,RGB(0,255,0));
}

void CStageMgr::CopyStage(STAGE _eStage, OBJ_TYPE _eType)
{
	if (m_eCurState == _eStage)
		return;
	for (UINT i = 0; i < m_pCurStage->GetObjVector()[(UINT)_eType].size(); ++i)
	{
		m_pArrStage[(UINT)_eStage]->GetObjVector()[(UINT)_eType].push_back(m_pCurStage->GetObjVector()[(UINT)_eType][i]);
	}
}

void CStageMgr::SetStageDir(STAGE _eStage,DIR _eDir)
{
	((CGameStage*)m_pArrStage[(UINT)_eStage])->SetDir(_eDir);
}

void CStageMgr::ClearCheck()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
		m_arrDelCheck[i] = false;
}
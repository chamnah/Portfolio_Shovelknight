#include "stdafx.h"
#include "CStageMgr.h"
#include "CStageLogo.h"
#include "CStageStart.h"
#include "CStageTool.h"
#include "CCore.h"
#include "CCollisionMgr.h"

CStageMgr::CStageMgr()
{
}

CStageMgr::~CStageMgr()
{
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

void CStageMgr::ChangeStage(STAGE _eStage)
{
	if (m_eCurState == _eStage)
		return;

	m_eCurState = _eStage;
	m_pCurStage->Exit();
	m_pCurStage = m_pArrStage[(UINT)_eStage];
	CCollisionMgr::GetInst()->ResetCheck();
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
	m_hBit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), m_iTileSizeX * TILE_SIZE, m_iTileSizeY * TILE_SIZE);
	m_hDC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
    HBITMAP OldBit = (HBITMAP)SelectObject(m_hDC,m_hBit);
	DeleteObject(OldBit);

}

// 이 함수는 꼭 새로운 스테이지에 타일을 읽어오고나서 사용해야 한다.
void CStageMgr::AddBackGround(int _iDir)
{
	BITMAP bm{};
	GetObject(m_hBit, sizeof(bm), &bm); // 이거 한 이유는 간단히 크기 알려고

	if (_iDir == RIGHT)
	{
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), bm.bmWidth + (m_iTileSizeX * TILE_SIZE), bm.bmHeight);
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, 0,0, bm.bmWidth, bm.bmHeight,m_hDC,0,0,SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}
}
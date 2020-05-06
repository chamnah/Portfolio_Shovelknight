#include "stdafx.h"
#include "CStageMgr.h"
#include "CStageLogo.h"
#include "CStageStart.h"
#include "CStageTool.h"
#include "CCore.h"
#include "CCollisionMgr.h"
#include "CStageOne.h"
#include "CStageTwo.h"
#include "CGameMgr.h"
#include "CStageKing.h"
#include "CStageThree.h"
#include "CStageMap.h"
#include "CStageFive.h"
#include "CTimeMgr.h"
#include "CMainStageOne.h"
#include "CMainStageTwo.h"
#include "CMainStageThree.h"
#include "CMainStageFour.h"
#include "CMainStageFive.h"
#include "CMainStageSix.h"
#include "CMainStageSeven.h"
#include "CMainStageEight.h"
#include "CMainStageNine.h"
#include "SoundMgr.h"
#include "CResMgr.h"
#include "CEndingStage.h"

CStageMgr::CStageMgr()
	:m_arrDelCheck{}
	,m_bChange(false)
	, m_bHidden(false)
	, m_bHiddenFirst(false)
	, m_eDir(DIR::NONE)
	, m_fTime(0.f)
	, m_iNext(0)
	, m_eHiddenType(OBJ_TYPE::HIDDEN)
	, m_bFade(false)
	, m_fFadeTime(0.f)
	, m_bFirst(false)
	, m_fOutTime(0.f)
	, m_bFadeOut(false)
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
	m_pArrStage[(UINT)STAGE::THREE] = new CStageThree;
	m_pArrStage[(UINT)STAGE::KING] = new CStageKing;
	m_pArrStage[(UINT)STAGE::MAP] = new CStageMap;
	m_pArrStage[(UINT)STAGE::FIVE] = new CStageFive;
	m_pArrStage[(UINT)STAGE::M_ONE] = new CMainStageOne;
	m_pArrStage[(UINT)STAGE::M_TWO] = new CMainStageTwo;
	m_pArrStage[(UINT)STAGE::M_THREE] = new CMainStageThree;
	m_pArrStage[(UINT)STAGE::M_FOUR] = new CMainStageFour;
	m_pArrStage[(UINT)STAGE::M_FIVE] = new CMainStageFive;
	m_pArrStage[(UINT)STAGE::M_SIX] = new CMainStageSix;
	m_pArrStage[(UINT)STAGE::M_SEVEN] = new CMainStageSeven;
	m_pArrStage[(UINT)STAGE::M_EIGHT] = new CMainStageEight;
	m_pArrStage[(UINT)STAGE::M_NINE] = new CMainStageNine;
	m_pArrStage[(UINT)STAGE::ENDING] = new CEndingStage;
	m_pCurStage = m_pArrStage[(UINT)STAGE::LOGO];
	m_eCurState = STAGE::LOGO;
	m_pCurStage->Enter();
}

int CStageMgr::Update()
{
	if (m_bHidden)
	{
		if (!m_bHiddenFirst)
		{
			CSoundMgr::GetInst()->Play(L"Explode", false);

			if ((m_eHiddenType == OBJ_TYPE::HIDDEN && m_wcsPath != L"") || (m_eHiddenType == OBJ_TYPE::HIDDEN_TWO && m_wcsPathTwo != L""))
			{
				m_bHiddenFirst = true;

				int iObjSize = ((CGameStage*)m_pCurStage)->GetFiniteObj().size();
				int iMonSize = ((CGameStage*)m_pCurStage)->GetMonster().size();

				if (m_eHiddenType == OBJ_TYPE::HIDDEN)
				{
					m_pCurStage->LoadObj(m_wcsPath);
					m_wcsPath = L"";
				}
				else if (m_eHiddenType == OBJ_TYPE::HIDDEN_TWO)
				{
					m_pCurStage->LoadObj(m_wcsPathTwo);
					m_wcsPathTwo = L"";
				}
				((CGameStage*)m_pCurStage)->CreateMonster(iMonSize);
				((CGameStage*)m_pCurStage)->CreateFiniteObj(iObjSize);
			}
		}

		if (m_eDir == DIR::RIGHT)
			R_HiddenRemove();
		else if(m_eDir == DIR::LEFT)
			L_HiddenRemove();
	}

	return m_pCurStage->Progress();
}

void CStageMgr::Render(HDC _dc)
{
	m_pCurStage->Render(_dc);

	if (m_bFade)
	{
		m_fFadeTime += (255.f / 2.f) * DT;

		if (m_fFadeTime >= 255.f)
		{
			m_fOutTime += DT;
			m_fFadeTime = 255.f;

			if (m_fOutTime > 1.f)
			{
				m_bFadeOut = true;
				m_bFade = false;
			}
			if (!m_bFirst)
			{
				m_bFirst = true;
				SetStageDir(STAGE::ENDING,DIR::NONE);
				ChangeStage(STAGE::ENDING);
				
			}
		}
		CTexture* pTex = TEX_LOAD(L"Fade",L"Image\\Fade.bmp");

		BLENDFUNCTION BF = {};

		BF.BlendOp = AC_SRC_OVER;
		BF.SourceConstantAlpha = m_fFadeTime;
		BF.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(_dc, 0,0, CCore::GetInst()->GetResolution().x, CCore::GetInst()->GetResolution().y, pTex->GetDC(),
			0, 0, pTex->GetWidth(), pTex->GetHeight(), BF);
	}

	if (m_bFadeOut)
	{
		m_fFadeTime -= (255.f / 2.f) * DT;

		if (m_fFadeTime <= 0)
		{
			m_fFadeTime = 0.f;
			m_bFadeOut = false;
		}
		CTexture* pTex = TEX_LOAD(L"Fade", L"Image\\Fade.bmp");

		BLENDFUNCTION BF = {};

		BF.BlendOp = AC_SRC_OVER;
		BF.SourceConstantAlpha = m_fFadeTime;
		BF.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(_dc, 0, 0, CCore::GetInst()->GetResolution().x, CCore::GetInst()->GetResolution().y, pTex->GetDC(),
			0, 0, pTex->GetWidth(), pTex->GetHeight(), BF);

	}
}

void CStageMgr::ChangeStage(STAGE _eStage)
{
	if (m_eCurState == _eStage)
		return;
	
	
	m_pCurStage->Exit();
	ClearCheck();
	m_pCurStage = m_pArrStage[(UINT)_eStage];
	//CCollisionMgr::GetInst()->ResetCheck();
	if (m_eCurState == STAGE::MAP)
		m_pCurStage->ReStart();
	m_eCurState = _eStage;
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
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), (m_iTileSizeX * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE) + (m_iTileSizeY * TILE_SIZE));
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, 0, m_iTileSizeY * TILE_SIZE, (m_iPreTileSize.x * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE), m_hDC, 0, 0, SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}
	else if (_eDir == DIR::DOWN)
	{
		HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), (m_iTileSizeX * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE) + (m_iTileSizeY * TILE_SIZE));
		HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
		HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
		DeleteObject(Old);
		BitBlt(DC, 0, 0, (m_iPreTileSize.x * TILE_SIZE), (m_iPreTileSize.y * TILE_SIZE), m_hDC, 0, 0, SRCCOPY);
		DeleteObject(m_hBit);
		DeleteDC(m_hDC);
		m_hBit = Bit;
		m_hDC = DC;
	}

}

void CStageMgr::ArriveBackGround(DIR _eDir)
{
	HBITMAP Bit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE));
	HDC DC = CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	HBITMAP Old = (HBITMAP)SelectObject(DC, Bit);
	DeleteObject(Old);
	if(_eDir == DIR::RIGHT)
		BitBlt(DC, 0, 0, (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE), m_hDC, (m_iPreTileSize.x * TILE_SIZE), 0, SRCCOPY);
	else if (_eDir == DIR::LEFT)
		BitBlt(DC, 0, 0, (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE), m_hDC, 0, 0, SRCCOPY);
	else if(_eDir == DIR::UP)
		BitBlt(DC, 0, 0, (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE), m_hDC, 0, 0, SRCCOPY);
	else if(_eDir == DIR::DOWN)
		BitBlt(DC, 0, 0, (m_iTileSizeX * TILE_SIZE), (m_iTileSizeY * TILE_SIZE), m_hDC, 0, (m_iPreTileSize.y * TILE_SIZE), SRCCOPY);
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

void CStageMgr::ResetStage()
{
	m_pCurStage->Exit();
	ClearCheck();
	if (CGameMgr::GetInst()->LastSave().eStage != m_eCurState)
	{
		m_eCurState = CGameMgr::GetInst()->LastSave().eStage;
		m_pCurStage = m_pArrStage[(UINT)m_eCurState];
	}
	m_pCurStage->ReStart();
	dynamic_cast<CGameStage*>(m_pCurStage)->SetDir(DIR::NONE);
	m_pCurStage->Enter();
}

void CStageMgr::SetStageDir(STAGE _eStage,DIR _eDir)
{
	((CGameStage*)m_pArrStage[(UINT)_eStage])->SetDir(_eDir);
}

void CStageMgr::L_HiddenRemove()
{
	int iPrev = 0;

	if (m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].size() == 0)
		return;

	int iHeight = ((CCamObj*)m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].back())->GetRealPos().y - ((CCamObj*)m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].front())->GetRealPos().y;

	iHeight = (iHeight / TILE_SIZE) + 1;

	int iWidth = m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].size() / iHeight;

	iPrev = iWidth - 1;

	m_fTime += DT;

	if (m_fTime > 0.2f)
	{
		++m_iNext;

		iPrev -= m_iNext;
		m_fTime = 0;
	}
	
	for (int i = 0; i < iHeight; ++i)
		((CCamObj*)m_pCurStage->GetObjVector()[(UINT)m_eHiddenType][i * iWidth + iPrev])->SetDeath(true);
	
	if (m_iNext >= iWidth - 1)
	{
		m_bHiddenFirst = false;
		m_bHidden = false;
		m_fTime = 0.f;
		m_iNext = 0;
	}
}

void CStageMgr::R_HiddenRemove()
{
	if (m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].size() == 0)
		return;

	m_fTime += DT;

	if (m_fTime > 0.2f)
	{
		++m_iNext;
		m_fTime = 0;
	}

	int iHeight = ((CCamObj*)m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].back())->GetRealPos().y - ((CCamObj*)m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].front())->GetRealPos().y;

	iHeight = (iHeight / TILE_SIZE) + 1;

	int iWidth = m_pCurStage->GetObjVector()[(UINT)m_eHiddenType].size() / iHeight;

	for (int i = 0; i < iHeight; ++i)
		((CCamObj*)m_pCurStage->GetObjVector()[(UINT)m_eHiddenType][i * iWidth + m_iNext])->SetDeath(true);
	
	if (m_iNext >= iWidth - 1)
	{
		m_bHiddenFirst = false;
		m_bHidden = false;
		m_fTime = 0.f;
		m_iNext = 0;
	}
}

void CStageMgr::ReStart()
{
	m_pCurStage->ReStart();
}

void CStageMgr::ClearCheck()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
		m_arrDelCheck[i] = false;
}
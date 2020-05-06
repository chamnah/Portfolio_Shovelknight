#include "stdafx.h"
#include "CGameStage.h"
#include "CStageMove.h"
#include "CBeeto.h"
#include "CCore.h"
#include "CStageMgr.h"
#include "CTimeMgr.h"
#include "CTile.h"
#include "CBubbleDragon.h"
#include "CBubbleAttack.h"
#include <afxdlgs.h>
#include "CImageObj.h"
#include "CCollider.h"
#include "CAnimBack.h"
#include "CPlayer.h"
#include "CSavePoint.h"
// 여기서 부터 인벤 구현 헤더이다.
#include "CResMgr.h"
#include "CInvenUI.h"
#include "CKeyMgr.h"
#include "CStone.h"
#include "CPlatter.h"
#include "CArmorer.h"
#include "CMoveTile.h"
#include "CMapObj.h"
#include "CBlock.h"
#include "CCamMgr.h"
#include "CHiddenTile.h"
#include "CSlime.h"

CGameStage::CGameStage()
	:m_bInven(false), m_eDir(DIR::NONE)
	, m_vStart{}
	, m_vPlayerStart{}
	, m_vPlayerPos{}
	, m_fAccTime(0.f)
	, m_bInvenOn(false)
	, m_bInvenOff(false)
	, m_bInit(false)
{
}

CGameStage::~CGameStage()
{
	m_vMonster.clear();

	for (UINT i = 0; i < m_vNextStage.size(); ++i)
		delete m_vNextStage[i];

	m_vNextStage.clear();
}

int CGameStage::Progress()
{
	if (m_eDir != DIR::NONE && m_fAccTime < 1.f)
	{
		m_fAccTime += DT;

		if (m_eDir == DIR::RIGHT)
		{
			m_vPlayerStart.y = m_vPlayerPos.y;
			CCamMgr::GetInst()->SetLook(m_vStart.x + (m_fAccTime * (((CStageMgr::GetInst()->GetPreTileSize().x * TILE_SIZE) + CCore::GetInst()->GetResolution().x / 2.f) - m_vStart.x)),
				m_vStart.y);
			m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->SetPos(m_vPlayerPos.x + (m_fAccTime * (m_vPlayerStart.x - m_vPlayerPos.x)), m_vPlayerPos.y);
		}
		else if (m_eDir == DIR::LEFT)
		{
			m_vPlayerStart.y = m_vPlayerPos.y;
			CCamMgr::GetInst()->SetLook(m_vStart.x + (m_fAccTime * ((((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - CCore::GetInst()->GetResolution().x / 2.f) - m_vStart.x))), // 여기
				m_vStart.y);
			m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->SetPos(m_vPlayerPos.x + (m_fAccTime * (m_vPlayerStart.x - m_vPlayerPos.x)), m_vPlayerPos.y);
		}
		else if (m_eDir == DIR::UP)
		{
			m_vPlayerStart.x = m_vPlayerPos.x;
			CCamMgr::GetInst()->SetLook(m_vStart.x,	m_vStart.y + (m_fAccTime * ((CCore::GetInst()->GetResolution().y / 2.f) - m_vStart.y)));
			m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->SetPos(m_vPlayerPos.x, m_vPlayerPos.y + (m_fAccTime * (m_vPlayerStart.y - m_vPlayerPos.y)));
		}
		else if (m_eDir == DIR::DOWN)
		{
			m_vPlayerStart.x = m_vPlayerPos.x;
			CCamMgr::GetInst()->SetLook(m_vStart.x, m_vStart.y + (m_fAccTime * (((CStageMgr::GetInst()->GetPreTileSize().y * TILE_SIZE) + CCore::GetInst()->GetResolution().y / 2.f) - m_vStart.y)));
			m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->SetPos(m_vPlayerPos.x, m_vPlayerPos.y + (m_fAccTime * (m_vPlayerStart.y - m_vPlayerPos.y)));
		}

		if (m_fAccTime > 1.f)
		{
			((CPlayer*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SpeedMovingOff(m_eDir);
			m_bInit = true;
		}
	}
	else
	{
		if (m_bInit)
		{
			if (CStageMgr::GetInst()->GetCurState() == STAGE::KING)
			{
				CCamMgr::GetInst()->SetLook(((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - CCore::GetInst()->GetResolution().x / 2.f), CCore::GetInst()->GetResolution().y + CCore::GetInst()->GetResolution().y / 2.f);
				m_eDir = DIR::NONE;
				((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos(m_vPlayerStart);
				CCamMgr::GetInst()->SetPlayerPos(m_vPlayerStart.x, CCamMgr::GetInst()->GetLook().y);
				CCamMgr::GetInst()->ScrollInit();
				CCamMgr::GetInst()->update();
				StageMoveInit(L"Tile\\Boss.tile");
			}
			else
			{
				if (m_eDir == DIR::LEFT)
				{
					CCamMgr::GetInst()->SetLook(((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - CCore::GetInst()->GetResolution().x / 2.f), CCore::GetInst()->GetResolution().y / 2.f);
					((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - 90.f, m_vPlayerStart.y - 5);
					CCamMgr::GetInst()->SetPlayerPos(((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().x, CCamMgr::GetInst()->GetLook().y);
				}
				else if (m_eDir == DIR::RIGHT)
				{
					CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);
					((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos(m_vPlayerStart);
					CCamMgr::GetInst()->SetPlayerPos(m_vPlayerStart.x, CCamMgr::GetInst()->GetLook().y);
				}
				else if (m_eDir == DIR::UP)
				{
					CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);
					((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos(m_vPlayerStart);
					CCamMgr::GetInst()->SetPlayerPos(CCamMgr::GetInst()->GetLook().x, CCore::GetInst()->GetResolution().y / 2.f);
				}
				else if (m_eDir == DIR::DOWN)
				{
					CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);
					((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos(m_vPlayerStart);
					//CCamMgr::GetInst()->SetPlayerPos(CCamMgr::GetInst()->GetLook().x, CCore::GetInst()->GetResolution().y / 2.f);
				}
				CStageMgr::GetInst()->ArriveBackGround(m_eDir);
				((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][0])->SetDC(CStageMgr::GetInst()->GetDC());
				ArriveTile();
				CCamMgr::GetInst()->update();
				CCamMgr::GetInst()->ScrollInit();
				Init();
			}
			CCamMgr::GetInst()->SetStop(false);
			CStageMgr::GetInst()->SetPreTileSize();

			m_bInit = false;
		}
		for (UINT i = 0; i < m_vNextStage.size(); ++i)
		{
			if (m_vNextStage[i]->update() == CHANGE_STAGE)
				return 0;
		}

		KeyInput();

		if (CStageMgr::GetInst()->GetCurState() != STAGE::KING && CStageMgr::GetInst()->GetCurState() != STAGE::MAP && CStageMgr::GetInst()->GetCurState() != STAGE::ENDING)
		{
			CCamMgr::GetInst()->SetPlayerPos(((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetColl()->GetRealPos().x, CCamMgr::GetInst()->GetLook().y);
			CCamMgr::GetInst()->SetLook(((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().x, CCamMgr::GetInst()->GetLook().y);
		}

		CStage::Update();

		MonsterRegen();
		ObjRegen();

		if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_0, KEY_STATE::TAB))
		{
			CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2, CCore::GetInst()->GetResolution().y / 2);
			CCamMgr::GetInst()->ScrollInit();
			CStageMgr::GetInst()->SetTileSizeX(0);
			CStageMgr::GetInst()->SetTileSizeY(0);
			CStageMgr::GetInst()->ChangeStage(STAGE::LOGO);
		}
		return 1;
	}

	return 0;
}

void CGameStage::Render(HDC _hdc)
{
	CStage::Render(_hdc);
	for (UINT i = 0; i < m_vNextStage.size(); ++i)
		m_vNextStage[i]->render(_hdc);
}

void CGameStage::MonsterRegen()
{
	for (UINT i = 0; i < m_vMonster.size(); ++i)
	{
		if (m_vMonster[i].bDeath == true && m_vMonster[i].bExit)
		{
			if (m_vMonster[i].vPos.x > CCamMgr::GetInst()->GetDifference().x &&
				m_vMonster[i].vPos.x < CCamMgr::GetInst()->GetDifference().x + CCore::GetInst()->GetResolution().x)
			{
				m_vMonster[i].bDeath = false;
				m_vMonster[i].bExit = false;
				CObj* pObj = NULL;
				if (m_vMonster[i].eType == M_TYPE::BEETO)
					pObj = new CBeeto(m_vMonster[i].vPos);
				else if (m_vMonster[i].eType == M_TYPE::SLIME)
				{
					pObj = new CSlime;
					pObj->SetPos(m_vMonster[i].vPos);
					((CCamObj*)pObj)->SetRealPos(m_vMonster[i].vPos);
				}
				pObj->Init();
				((CMonster*)pObj)->SetID(i);
				m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);
			}
		}

		if (m_vMonster[i].vPos.x < CCamMgr::GetInst()->GetDifference().x ||
			m_vMonster[i].vPos.x > CCamMgr::GetInst()->GetDifference().x + CCore::GetInst()->GetResolution().x)
		{
			m_vMonster[i].bExit = true;
		}
		else
		{
			m_vMonster[i].bExit = false;
		}
	}
}

void CGameStage::ObjRegen()
{
	for (UINT i = 0; i < m_vecSaveObj.size(); ++i)
	{
		if (m_vecSaveObj[i].bDeath)
		{
			m_vecSaveObj[i].fTimer += DT;
			if (m_vecSaveObj[i].fTimer > 3.f)
			{
				m_vecSaveObj[i].fTimer = 0.f;
				m_vecSaveObj[i].bDeath = false;
				CBubbleAttack* pBubble = new CBubbleAttack;
				pBubble->SetRealPos(m_vecSaveObj[i].vPos.x,CCore::GetInst()->GetResolution().y + 100);
				pBubble->SetPos(m_vecSaveObj[i].vPos.x, CCore::GetInst()->GetResolution().y + 100);
				pBubble->SetPosY(m_vecSaveObj[i].vPos.y);
				pBubble->SetID(i);
				pBubble->Init();
				m_vObj[(UINT)OBJ_TYPE::SKILL].push_back(pBubble);
			}
		}
	}
}

void CGameStage::KeyInput()
{
	if (KEY(KEY_TYPE::KEY_I, KEY_STATE::TAB))
	{
		if (!m_bInven)
		{
			m_bInvenOn = true;
		}
		else
		{
			m_bInvenOff = true;
		}
		m_bInven = !m_bInven;
	}

	if (m_bInvenOn)
	{
		m_bInvenOn = false;
		CInvenUI* pInven = new CInvenUI;
		pInven->SetPos(300,200);
		pInven->Init();
		m_vObj[(UINT)OBJ_TYPE::UI].push_back(pInven);
	}
	else if(m_bInvenOff)
	{
		m_bInvenOff = false;
		vector<CObj*>::iterator iter = m_vObj[(UINT)OBJ_TYPE::UI].begin();
		for (; iter != m_vObj[(UINT)OBJ_TYPE::UI].end(); ++iter)
		{
			if (((CUI*)*iter)->GetUIType() == UI_TYPE::INVEN)
			{
				delete *iter;
				m_vObj[(UINT)OBJ_TYPE::UI].erase(iter);
				break;
			}
		}
	}
}

void CGameStage::StageMoveInit(wstring _wstrFileName)
{
	if (m_eDir == DIR::NONE)
	{
		m_vStartPos = Vec2(0, 0);
		ClearObj((int)OBJ_TYPE::TILE);
		LoadTile(_wstrFileName); // 내가 지정한 스테이지에 타일들을 생성한다. // StartPos에다가 더한 위치에 다가 로드하므로 pos 아래에 함수를 호출해줘야한다.
		CStageMgr::GetInst()->CreateBackGround();
		
	}
	else
	{
		m_fAccTime = 0.f;
		m_vPlayerPos = m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos();
		m_vStartPos = Vec2(0, 0);

		if (m_eDir == DIR::RIGHT)
		{
			m_vPlayerStart = Vec2(100.f, m_vPlayerPos.y);
			m_vStartPos.x = CStageMgr::GetInst()->GetPreTileSize().x * TILE_SIZE;
			LoadTile(_wstrFileName);
		}
		else if (m_eDir == DIR::LEFT)
		{
			m_vStartPos.x = 0;
			LoadTile(_wstrFileName);
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x + (CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE), CCamMgr::GetInst()->GetLook().y);
			m_vPlayerStart = Vec2(CCore::GetInst()->GetResolution().x - 90.f, m_vPlayerPos.y);
		}
		else if (m_eDir == DIR::UP)
		{
			m_vPlayerStart = Vec2(m_vPlayerPos.x, CCore::GetInst()->GetResolution().y - 100.f);
			//m_vStartPos.y = -((int)(CStageMgr::GetInst()->GetPreTileSize().y * TILE_SIZE));
			m_vStartPos.y = 0.f;
			LoadTile(_wstrFileName);
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, (CStageMgr::GetInst()->GetTileSizeY() * TILE_SIZE) + CCamMgr::GetInst()->GetLook().y);
			
		}
		else if (m_eDir == DIR::DOWN)
		{
			m_vPlayerStart = Vec2(m_vPlayerPos.x, 120.f);
			m_vStartPos.y = ((int)(CStageMgr::GetInst()->GetPreTileSize().y * TILE_SIZE));
			LoadTile(_wstrFileName);
		}

		CStageMgr::GetInst()->AddBackGround(m_eDir);

		m_vStart = CCamMgr::GetInst()->GetLook();
		CCamMgr::GetInst()->SetStop(true);
	}

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->Init();

	/*CTexture* pTex = TEX_LOAD(L"BackGround", L"Image\\Sky.bmp");
	BitBlt(CStageMgr::GetInst()->GetDC(), m_vStartPos.x,0, pTex->GetWidth(), pTex->GetHeight(), pTex->GetDC(),0,0,SRCCOPY);*/
	TileDCRender(CStageMgr::GetInst()->GetDC());

	CamRange();

	if (m_eDir == DIR::NONE/* && CStageMgr::GetInst()->GetCurState() != STAGE::KING*/)
		Init();
}

void CGameStage::CamRange()
{
	Vec2 vPos = Vec2(0.f, 0.f);
	CCamMgr::GetInst()->SetLeftEnd(vPos);

	vPos = Vec2(float(CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE), 0.f);
	CCamMgr::GetInst()->SetRightEnd(vPos);
}

void CGameStage::CreateMonster(int iIdx)
{
	CObj* pObj = NULL;
	for (UINT i = iIdx; i < m_vMonster.size(); ++i)
	{
		if (m_vMonster[i].eType == M_TYPE::BEETO)
			pObj = new CBeeto(m_vMonster[i].vPos.x, m_vMonster[i].vPos.y);
		else if (m_vMonster[i].eType == M_TYPE::BUBBLE_DRAGON)
		{
			pObj = new CBubbleDragon(m_vMonster[i].vPos.x, m_vMonster[i].vPos.y);
			((CDynamicObj*)pObj)->SetDir(m_vMonster[i].eDir);
		}
		else if (m_vMonster[i].eType == M_TYPE::SLIME)
		{
			pObj = new CSlime;
			pObj->SetPos(CCamMgr::GetInst()->GetRealPos(m_vMonster[i].vPos.x, m_vMonster[i].vPos.y));
			((CCamObj*)pObj)->SetRealPos(m_vMonster[i].vPos);
		}
		pObj->Init();
		((CMonster*)pObj)->SetID(i);
		m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);
	}
}

void CGameStage::CreateFiniteObj(int iIdx)
{
	CObj* pObj = nullptr;

	for (UINT i = iIdx; i < m_vecFiniteObj.size(); ++i)
	{
		if (!m_vecFiniteObj[i].bDeath)
		{
			if (m_vecFiniteObj[i].wcsName == L"Stone")
			{
				pObj = new CStone;
				((CStone*)pObj)->SetID(i);
				((CStone*)pObj)->SetIndex(m_vecFiniteObj[i].iIdx);
			}
			else if (m_vecFiniteObj[i].wcsName == L"Platter")
			{
				pObj = new CPlatter;
				((CPlatter*)pObj)->SetID(i);
			}
			else if (m_vecFiniteObj[i].wcsName == L"Armorer")
				pObj = new CArmorer;
			else if (m_vecFiniteObj[i].wcsName == L"SavePoint")
			{
				pObj = new CSavePoint;
				((CSavePoint*)pObj)->SetStage(CStageMgr::GetInst()->GetCurState());
			}
			else if (m_vecFiniteObj[i].wcsName == L"MoveTile")
			{
				pObj = new CMoveTile;
				((CMoveTile*)pObj)->SetStartPos(m_vecFiniteObj[i].vPos);
			}
			else if (m_vecFiniteObj[i].wcsName == L"DirtySand")
			{
				pObj = new CBlock;
				pObj->SetScale(Vec2(TILE_SIZE * 2, TILE_SIZE * 2));
				pObj->SetTexture(TEX_LOAD(L"DirtySand", L"Image\\DirtySand.bmp"));
				((CBlock*)pObj)->SetEffectTexture(TEX_LOAD(L"DirtySandEffect", L"Image\\DirtySandEffect.bmp"));
			}
			else if (m_vecFiniteObj[i].wcsName == L"DirtySand_S")
			{
				pObj = new CBlock;
				pObj->SetScale(Vec2(TILE_SIZE , TILE_SIZE ));
				pObj->SetTexture(TEX_LOAD(L"DirtySand_S", L"Image\\DirtySand_S.bmp"));
				((CBlock*)pObj)->SetEffectTexture(TEX_LOAD(L"DirtySand_S_Effect", L"Image\\DirtySand_S_Effect.bmp"));
			}
			else if (m_vecFiniteObj[i].wcsName == L"Hidden")
			{
				pObj = new CHiddenTile;
				((CHiddenTile*)pObj)->SetIndex(m_vecFiniteObj[i].iIdx);
				((CHiddenTile*)pObj)->SetID(i);
				pObj->SetType(OBJ_TYPE::HIDDEN);
			}
			else if (m_vecFiniteObj[i].wcsName == L"HiddenTwo")
			{
				pObj = new CHiddenTile;
				((CHiddenTile*)pObj)->SetIndex(m_vecFiniteObj[i].iIdx);
				((CHiddenTile*)pObj)->SetID(i);
				pObj->SetType(OBJ_TYPE::HIDDEN_TWO);
			}

			pObj->SetPos(m_vecFiniteObj[i].vPos);
			((CCamObj*)pObj)->SetRealPos(m_vecFiniteObj[i].vPos);
			pObj->Init();
			if (m_vecFiniteObj[i].wcsName == L"DirtySand" || m_vecFiniteObj[i].wcsName == L"DirtySand_S")
				m_vObj[(UINT)OBJ_TYPE::BLOCK].push_back(pObj);
			else if (m_vecFiniteObj[i].wcsName == L"Hidden")
				m_vObj[(UINT)OBJ_TYPE::HIDDEN].push_back(pObj);
			else if (m_vecFiniteObj[i].wcsName == L"HiddenTwo")
				m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO].push_back(pObj);
			else
				m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
		}
	}
}

void CGameStage::Exit()
{
	CStage::Exit();

	for (UINT i = 0; i < m_vNextStage.size(); ++i)
		delete m_vNextStage[i];

	m_vNextStage.clear();
}

int CGameStage::LoadObj(wstring _strPath)
{
	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");
	Vec2 vPos = Vec2(0, 0);
	int iNameLength = 0;
	int iSize = 0;
	wstring wcsName;

	fread(&iSize, sizeof(int), 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		wchar_t wcsFileName[255] = L"";

		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(wcsFileName, sizeof(wchar_t), iNameLength, pFile);
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		wcsName = wcsFileName;

		if (wcsName == L"Beeto")
			m_vMonster.push_back(tMonster(true, vPos, false, false, M_TYPE::BEETO));
		else if (wcsName == L"R_BubbleDragon")
			m_vMonster.push_back(tMonster(true, vPos, false, false, M_TYPE::BUBBLE_DRAGON, DIR::RIGHT));
		else if (wcsName == L"L_BubbleDragon")
			m_vMonster.push_back(tMonster(true, vPos, false, false, M_TYPE::BUBBLE_DRAGON, DIR::LEFT));
		else if(wcsName == L"Slime")
			m_vMonster.push_back(tMonster(true, vPos, false, false, M_TYPE::SLIME));
		else if (wcsName == L"Bubble")
			m_vecSaveObj.push_back(tSaveObj(vPos, true));
		else if (wcsName == L"Start")
		{
			CTexture* pTex = TEX_LOAD(L"Start", L"Image\\Start.bmp");
			CMapObj* pObj = new CMapObj;
			pObj->SetTexture(pTex);
			pObj->SetPos(vPos);
			pObj->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
			pObj->SetScale(Vec2(30,30));
			pObj->SetStage(STAGE::START);
			pObj->Init();
			m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
		}
		else if (wcsName == L"BossMap")
		{
			CTexture* pTex = TEX_LOAD(L"BossMap", L"Image\\BossMap.bmp");
			CMapObj* pObj = new CMapObj;
			pObj->SetTexture(pTex);
			pObj->SetPos(vPos);
			pObj->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
			pObj->SetScale(Vec2(30, 30));
			pObj->SetStage(STAGE::KING);
			pObj->Init();
			m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
		}
		else if (wcsName == L"Stage1")
		{
			CTexture* pTex = TEX_LOAD(L"Stage1", L"Image\\Stage1.bmp");
			CMapObj* pObj = new CMapObj;
			pObj->SetTexture(pTex);
			pObj->SetPos(vPos);
			pObj->SetSize(tSize(pTex->GetWidth(), pTex->GetHeight()));
			pObj->SetScale(Vec2(30, 30));
			pObj->SetStage(STAGE::M_ONE);
			pObj->Init();
			m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
		}
			
		else if (wcsName == L"Breaks")
		{
			CTexture* pTex = TEX_LOAD(L"Breaks", L"Image\\Breaks.bmp");
			CImageObj* pObj = new CImageObj;
			pObj->SetRealPos(vPos);
			pObj->SetSize(tSize(19, 18));
			pObj->SetTexture(pTex);
			pObj->SetScale(Vec2(19 * 4, 18 * 4));
			m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
		}

		else if (wcsName == L"Candles")
		{
			CAnimBack* pBack = new CAnimBack;
			pBack->SetRealPos(vPos);
			pBack->Init();
			m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pBack);
		}
		else if (wcsName == L"SavePoint")
			m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName));
		else if (wcsName == L"Stone")
			m_vecFiniteObj.push_back(tFiniteObj(vPos,wcsName));
		else if (wcsName == L"Platter")
			m_vecFiniteObj.push_back(tFiniteObj(vPos,wcsName));
		else if(wcsName == L"DirtySand")
			m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName));
		else if (wcsName == L"DirtySand_S")
			m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName));
		else if(wcsName == L"Armorer")
			m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName));
		else if (wcsName == L"MoveTile")
			m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName));
	}

	fread(&iSize, sizeof(int), 1, pFile);
	

	for (UINT i = 0; i < iSize; ++i)
	{
		wchar_t wcsFileName2[255] = L"";

		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(wcsFileName2, sizeof(wchar_t), iNameLength, pFile);

		wcsName = wcsFileName2;

		Vec2 vPos = Vec2(0, 0);
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		int iIdx = 0;

		fread(&iIdx, sizeof(int), 1, pFile);

		m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName,iIdx));
	}

	fread(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		wchar_t wcsFileName3[255] = L"";

		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(wcsFileName3, sizeof(wchar_t), iNameLength, pFile);

		wcsName = wcsFileName3;

		Vec2 vPos = Vec2(0, 0);
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		int iIdx = 0;

		fread(&iIdx, sizeof(int), 1, pFile);

		m_vecFiniteObj.push_back(tFiniteObj(vPos, wcsName, iIdx));
	}

	fclose(pFile);

	return 0;
}

void CGameStage::ChangeTile(wstring _wstrFileName,CTexture* _pTex)
{
	m_vStartPos = Vec2(0, 0);
	ClearObj((int)OBJ_TYPE::TILE);
	LoadTile(_wstrFileName); // 내가 지정한 스테이지에 타일들을 생성한다. // StartPos에다가 더한 위치에 다가 로드하므로 pos 아래에 함수를 호출해줘야한다.
	CStageMgr::GetInst()->CreateBackGround();

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->Init();
		if(_pTex != nullptr)
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->SetTexture(_pTex);
	}
	TileDCRender(CStageMgr::GetInst()->GetDC());
	CamRange();
}
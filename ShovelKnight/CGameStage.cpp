#include "stdafx.h"
#include "CGameStage.h"
#include "CStageMove.h"
#include "CBeeto.h"
#include "CCore.h"
#include "CStageMgr.h"
#include "CTimeMgr.h"
#include "CTile.h"
// 여기서 부터 인벤 구현 헤더이다.
#include "CResMgr.h"
#include "CUI.h"
#include "CKeyMgr.h"

CGameStage::CGameStage()
	:m_bInven(false), m_eDir(DIR::NONE)
	, m_vStart{}
	, m_vPlayerStart{}
	, m_vPlayerPos{}
	, m_fAccTime(0.f)
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
		m_fAccTime += DT / 1.f;

		if (m_eDir == DIR::RIGHT)
			CCamMgr::GetInst()->SetLook(m_vStart.x + (m_fAccTime * (((CStageMgr::GetInst()->GetPreTileSize().x * TILE_SIZE) + CCore::GetInst()->GetResolution().x / 2.f) - m_vStart.x)),
				m_vStart.y);
		else if (m_eDir == DIR::LEFT)
			CCamMgr::GetInst()->SetLook(m_vStart.x + (m_fAccTime * ((((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - CCore::GetInst()->GetResolution().x / 2.f) - m_vStart.x))), // 여기
				m_vStart.y);

		m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->SetPos(m_vPlayerPos.x + (m_fAccTime * (m_vPlayerStart.x - m_vPlayerPos.x)), m_vPlayerPos.y);

		if (m_fAccTime >= 1.f)
		{
			CCamMgr::GetInst()->SetLook(((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - CCore::GetInst()->GetResolution().x / 2.f), CCore::GetInst()->GetResolution().y / 2.f);
			CStageMgr::GetInst()->ArriveBackGround(m_eDir);
			((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][0])->SetDC(CStageMgr::GetInst()->GetDC());
			ArriveTile();
			CCamMgr::GetInst()->SetStop(false);
			CCamMgr::GetInst()->SetPlayerPos(m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x, CCamMgr::GetInst()->GetLook().y);
			Init();
		}
	}
	else
	{
		for (UINT i = 0; i < m_vNextStage.size(); ++i)
		{
			if (m_vNextStage[i]->update() == CHANGE_STAGE)
				return 0;
		}

		CStage::Update();
		CCamMgr::GetInst()->SetPlayerPos(m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x, CCamMgr::GetInst()->GetLook().y);

		Regen();
	}

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_0, KEY_STATE::TAB))
		CStageMgr::GetInst()->ChangeStage(STAGE::LOGO);
	return 0;
}

void CGameStage::Render(HDC _hdc)
{
	CStage::Render(_hdc);
	for (UINT i = 0; i < m_vNextStage.size(); ++i)
		m_vNextStage[i]->render(_hdc);
}

void CGameStage::Regen()
{
	for (UINT i = 0; i < m_vMonster.size(); ++i)
	{
		if (m_vMonster[i].vPos.x < CCamMgr::GetInst()->GetDifference().x ||
			m_vMonster[i].vPos.x > CCamMgr::GetInst()->GetDifference().x + CCore::GetInst()->GetResolution().x)
		{
			m_vMonster[i].bExit = true;
		}
		if (m_vMonster[i].bDeath == true)
		{
			if (m_vMonster[i].vPos.x > CCamMgr::GetInst()->GetDifference().x &&
				m_vMonster[i].vPos.x < CCamMgr::GetInst()->GetDifference().x + CCore::GetInst()->GetResolution().x &&
				m_vMonster[i].bExit)
			{
				m_vMonster[i].bDeath = false;
				m_vMonster[i].bExit = false;
				CObj* pObj = NULL;
				if (m_vMonster[i].eType == M_TYPE::BEETO)
					pObj = new CBeeto(m_vMonster[i].vPos);

				((CBeeto*)pObj)->SetID(i);
				m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);
			}
		}
	}
}

void CGameStage::KeyInput()
{
	if (KEY(KEY_TYPE::KEY_I, KEY_STATE::TAB))
		m_bInven = !m_bInven;

	/*if (m_bInven)
	{
		CObj* pUI = new CUI;
		pUI->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Inven", L"Image\\inventory.bmp"));
		pUI->SetPos(350, 200);
		m_vObj[(UINT)OBJ_TYPE::UI].push_back(pUI);
	}*/
}

void CGameStage::StageMoveInit(wstring _wstrFileName)
{
	if (m_eDir == DIR::NONE)
	{
		ClearObj((int)OBJ_TYPE::TILE);
		LoadTile(_wstrFileName); // 내가 지정한 스테이지에 타일들을 생성한다. // StartPos에다가 더한 위치에 다가 로드하므로 pos 아래에 함수를 호출해줘야한다.
		CStageMgr::GetInst()->CreateBackGround();
	}
	else
	{
		m_fAccTime = 0.f;
		m_vPlayerPos = m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos();

		if (m_eDir == DIR::RIGHT)
		{
			m_vPlayerStart = Vec2(100.f, m_vPlayerPos.y);
			m_vStartPos.x = CStageMgr::GetInst()->GetPreTileSize().x * TILE_SIZE;
		}
		else if (m_eDir == DIR::LEFT)
		{
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x + (CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE), CCamMgr::GetInst()->GetLook().y);
			m_vPlayerStart = Vec2(CCore::GetInst()->GetResolution().x - 300.f, m_vPlayerPos.y);
			m_vStartPos.x = 0;
		}
		else if (m_eDir == DIR::UP)
		{
			m_vPlayerStart = Vec2(m_vPlayerPos.x, CCore::GetInst()->GetResolution().y - 100.f);
			m_vStartPos.y = -((int)(CStageMgr::GetInst()->GetPreTileSize().y * TILE_SIZE));
		}
		else if (m_eDir == DIR::DOWN)
		{
			m_vPlayerStart = Vec2(m_vPlayerPos.x, 200.f);
			m_vStartPos.y = ((int)(CStageMgr::GetInst()->GetPreTileSize().y * TILE_SIZE));
		}

		LoadTile(_wstrFileName);
		CStageMgr::GetInst()->AddBackGround(m_eDir);

		m_vStart = CCamMgr::GetInst()->GetLook();
		CCamMgr::GetInst()->SetStop(true);
	}

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->Init();

	TileDCRender(CStageMgr::GetInst()->GetDC());

	CamRange();

	if (m_eDir == DIR::NONE)
		Init();
}

void CGameStage::CamRange()
{
	Vec2 vPos = Vec2(0.f, 0.f);
	CCamMgr::GetInst()->SetLeftEnd(vPos);

	vPos = Vec2(float(CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE), 0.f);
	CCamMgr::GetInst()->SetRightEnd(vPos);
}

void CGameStage::Exit()
{
	CStage::Exit();
	m_vMonster.clear();

	for (UINT i = 0; i < m_vNextStage.size(); ++i)
		delete m_vNextStage[i];

	m_vNextStage.clear();
}
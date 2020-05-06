#include "stdafx.h"
#include "CMainStageFour.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"
#include "CCore.h"

CMainStageFour::CMainStageFour()
{
	LoadObj(L"Object\\Main\\Stage4.obj");
}

CMainStageFour::~CMainStageFour()
{
}

void CMainStageFour::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::M_FIVE);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove; // 4
	pMove->SetRealPos(1184, CCore::GetInst()->GetResolution().y + 20.f);
	pMove->SetScale(Vec2(64, 64));
	pMove->Init();
	pMove->SetStage(STAGE::M_THREE);
	pMove->SetDir(DIR::DOWN);
	m_vNextStage.push_back(pMove);
}

void CMainStageFour::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage4.tile");
}
#include "stdafx.h"
#include "CMainStageOne.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"

CMainStageOne::CMainStageOne()
{
	LoadObj(L"Object\\Main\\Stage1.obj");
}

CMainStageOne::~CMainStageOne()
{
}

void CMainStageOne::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::M_TWO);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);
}

void CMainStageOne::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage1.tile");
}
#include "stdafx.h"
#include "CMainStageTwo.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"
CMainStageTwo::CMainStageTwo()
{
	LoadObj(L"Object\\Main\\Stage2.obj");
}

CMainStageTwo::~CMainStageTwo()
{
}

void CMainStageTwo::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::M_THREE);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(0, 480);
	pMove->SetScale(Vec2(10, 700));
	pMove->Init();
	pMove->SetStage(STAGE::M_ONE);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);
}

void CMainStageTwo::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage2.tile");
}
#include "stdafx.h"
#include "CMainStageNine.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"
#include "CCore.h"

CMainStageNine::CMainStageNine()
{
	LoadObj(L"Object\\Main\\Stage9.obj");
}

CMainStageNine::~CMainStageNine()
{
}

void CMainStageNine::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::MAP);
	pMove->SetDir(DIR::NONE);
	m_vNextStage.push_back(pMove);
}

void CMainStageNine::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage9.tile");
}
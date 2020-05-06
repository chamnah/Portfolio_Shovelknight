#include "stdafx.h"
#include "CStageThree.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"

CStageThree::CStageThree()
{
	LoadObj(L"Object\\Stage4.obj");
}

CStageThree::~CStageThree()
{
}

void CStageThree::Init()
{
	CreateFiniteObj();
	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos(1440, 910);
	pMove->SetScale(Vec2(64, 64));
	pMove->Init();
	pMove->SetStage(STAGE::TWO);
	pMove->SetDir(DIR::DOWN);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::FIVE);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);
}

void CStageThree::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Test4.tile");
}
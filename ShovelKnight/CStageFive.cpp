#include "stdafx.h"
#include "CStageFive.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"

CStageFive::CStageFive()
{
	LoadObj(L"Object\\Stage3.obj");
}

CStageFive::~CStageFive()
{
}

void CStageFive::Init()
{
	CreateFiniteObj();


	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::MAP);
	pMove->SetDir(DIR::NONE);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(-64, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::THREE);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);
}

void CStageFive::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);

	StageMoveInit(L"Tile\\Test5.tile");
}
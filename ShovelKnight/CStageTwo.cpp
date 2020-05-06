#include "stdafx.h"
#include "CStageTwo.h"
#include "CStageMove.h"
#include "CResMgr.h"
#include "CBackGroundObj.h"

CStageTwo::CStageTwo()
{
	LoadObj(L"Object\\Stage2.obj");
}

CStageTwo::~CStageTwo()
{
}

void CStageTwo::Init()
{
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos(1440, 50);
	pMove->SetScale(Vec2(64, 64));
	pMove->Init();
	pMove->SetStage(STAGE::THREE);
	pMove->SetDir(DIR::UP);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(-32, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::START);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);
}

void CStageTwo::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Test3.tile");
}
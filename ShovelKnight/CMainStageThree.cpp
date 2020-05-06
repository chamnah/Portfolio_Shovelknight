#include "stdafx.h"
#include "CMainStageThree.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"

CMainStageThree::CMainStageThree()
{
	LoadObj(L"Object\\Main\\Stage3.obj");
}

CMainStageThree::~CMainStageThree()
{
}

void CMainStageThree::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos(0, 480);
	pMove->SetScale(Vec2(10, 700));
	pMove->Init();
	pMove->SetStage(STAGE::M_TWO);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(1184, 40);
	pMove->SetScale(Vec2(64, 64));
	pMove->Init();
	pMove->SetStage(STAGE::M_FOUR);
	pMove->SetDir(DIR::UP);
	m_vNextStage.push_back(pMove);
}

void CMainStageThree::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage3.tile");
}
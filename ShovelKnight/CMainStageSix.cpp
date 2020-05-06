#include "stdafx.h"
#include "CMainStageSix.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"
#include "CCore.h"

CMainStageSix::CMainStageSix()
{
	LoadObj(L"Object\\Main\\Stage6.obj");
}

CMainStageSix::~CMainStageSix()
{
}

void CMainStageSix::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos(0, 480);
	pMove->SetScale(Vec2(10, 700));
	pMove->Init();
	pMove->SetStage(STAGE::M_FIVE);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(1376, CCore::GetInst()->GetResolution().y + 20.f);
	pMove->SetScale(Vec2(64, 64));
	pMove->Init();
	pMove->SetStage(STAGE::M_SEVEN);
	pMove->SetDir(DIR::DOWN);
	m_vNextStage.push_back(pMove);
}

void CMainStageSix::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage6.tile");
}
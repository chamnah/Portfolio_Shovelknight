#include "stdafx.h"
#include "CMainStageEight.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"
#include "CCore.h"

CMainStageEight::CMainStageEight()
{
	LoadObj(L"Object\\Main\\Stage8.obj");
}

CMainStageEight::~CMainStageEight()
{
}

void CMainStageEight::Init()
{
	//CreateMonster();
	CreateFiniteObj();

	CStageMgr::GetInst()->SetPath(L"Object\\Hidden\\HiddenEightOne.obj");
	CStageMgr::GetInst()->SetPathTwo(L"Object\\Hidden\\HiddenEightTwo.obj");

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos(1408, CCore::GetInst()->GetResolution().y + 20.f);
	pMove->SetScale(Vec2(128, 128));
	pMove->Init();
	pMove->SetStage(STAGE::M_NINE);
	pMove->SetDir(DIR::DOWN);
	m_vNextStage.push_back(pMove);
}

void CMainStageEight::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage8.tile");
}
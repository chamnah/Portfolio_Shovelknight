#include "stdafx.h"
#include "CMainStageSeven.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMove.h"
#include "CStageMgr.h"
#include "CCore.h"

CMainStageSeven::CMainStageSeven()
{
	LoadObj(L"Object\\Main\\Stage7.obj");
}

CMainStageSeven::~CMainStageSeven()
{
}

void CMainStageSeven::Init()
{
	CreateMonster();
	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos(352, CCore::GetInst()->GetResolution().y + 20.f);
	pMove->SetScale(Vec2(64, 64));
	pMove->Init();
	pMove->SetStage(STAGE::M_EIGHT);
	pMove->SetDir(DIR::DOWN);
	m_vNextStage.push_back(pMove);
}

void CMainStageSeven::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Stage7.tile");
}
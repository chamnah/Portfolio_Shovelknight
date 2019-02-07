#include "stdafx.h"
#include "CStageOne.h"
#include "CStageMgr.h"
#include "CTile.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"
#include "CCollisionMgr.h"
#include "CBeeto.h"
#include "CStageMove.h"
#include "CSavePoint.h"

CStageOne::CStageOne()
{
	m_vMonster.push_back(tMonster(true, Vec2(500.f, 500.f), false, false, M_TYPE::BEETO));
}

CStageOne::~CStageOne()
{
}

void CStageOne::Init()
{
	CreateMonster();

	CObj* pObj = new CSavePoint;
	pObj->SetPos(800, 600);
	((CSavePoint*)pObj)->SetStage(STAGE::ONE);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);

	CStageMove* pMove = new CStageMove;
	pMove->SetPos(500, 600);
	pMove->SetStage(STAGE::START);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);
}

void CStageOne::Enter()
{
	StageMoveInit(L"Tile\\Test2.tile");
}
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

CStageOne::CStageOne()
{
}

CStageOne::~CStageOne()
{
}

void CStageOne::Init()
{
	CObj* pObj = new CBeeto(500,500);
	m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);
	m_vMonster.push_back(tMonster(true, Vec2(m_vStartPos.x + 500, 500.f), false, false, M_TYPE::BEETO));

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
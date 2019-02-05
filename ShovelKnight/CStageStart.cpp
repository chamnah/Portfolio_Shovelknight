#include "stdafx.h"
#include "CStageStart.h"
#include "CPlayer.h"
#include "CTile.h"
#include "CKeyMgr.h"
#include "CStageMgr.h"
#include "CResMgr.h"
#include "function.h"
#include "CBeeto.h"
#include "CCamMgr.h"
#include "CCore.h"
#include "CCollisionMgr.h"
#include "CTextUI.h"
#include "CHpUI.h"
#include "CStageMove.h"
#include "CCoin.h"

CStageStart::CStageStart()
{
}

CStageStart::~CStageStart()
{
}

void CStageStart::Init()
{
	CObj* pObj = NULL;
	pObj = new CBeeto(100, 300);
	m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);
	m_vMonster.push_back(tMonster(true, Vec2(100, 300), false, false, M_TYPE::BEETO));

	pObj = new CCoin(COIN_TYPE::ONE);
	pObj->SetPos(600,600);
	m_vObj[(UINT)OBJ_TYPE::DROP].push_back(pObj);

	CStageMove* pMove = new CStageMove;
	pMove->SetPos(1500, 600);
	pMove->SetStage(STAGE::ONE);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);
}

void CStageStart::Enter()
{
	StageMoveInit(L"Tile\\Test.tile");
}
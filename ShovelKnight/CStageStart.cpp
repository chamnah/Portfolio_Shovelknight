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
#include "CSavePoint.h"

CStageStart::CStageStart()
{
	m_vMonster.push_back(tMonster(true, Vec2(100, 300), false, false, M_TYPE::BEETO));
}

CStageStart::~CStageStart()
{
}

void CStageStart::Init()
{
	if(m_eDir == DIR::NONE)
		m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->SetPos(Vec2(500, 750));

	CreateMonster();

	CObj* pObj = nullptr;

	pObj = new CSavePoint;
	pObj->SetPos(600, 600);
	((CSavePoint*)pObj)->SetStage(STAGE::START);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);

	pObj = new CCoin(COIN_TYPE::ONE);
	pObj->SetPos(600,600);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::DROP].push_back(pObj);

	CStageMove* pMove = new CStageMove;
	pMove->SetPos(1500, 600);
	pMove->SetStage(STAGE::ONE);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);
}

void CStageStart::Enter()
{
	StageMoveInit(L"Tile\\Test.tile");
}
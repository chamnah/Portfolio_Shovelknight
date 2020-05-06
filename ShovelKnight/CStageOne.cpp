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
#include "CArmorer.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStone.h"

CStageOne::CStageOne()
{
	//LoadObj(L"Object\\Stage2.obj");
	//m_vMonster.push_back(tMonster(true, Vec2(500.f, 500.f), false, false, M_TYPE::BEETO));
}

CStageOne::~CStageOne()
{
}

void CStageOne::Init()
{
	if (m_eDir == DIR::RIGHT)
		CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);
	else if(m_eDir == DIR::LEFT)
		CCamMgr::GetInst()->SetLook(CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE - CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);
	
	CreateMonster();
	CreateFiniteObj();

	CObj* pObj = nullptr;

	

	pObj = new CSavePoint;
	pObj->SetPos(800, 680);
	((CSavePoint*)pObj)->SetStage(STAGE::ONE);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);

	pObj = new CArmorer;
	pObj->SetPos(180, 700);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::KING);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(-64, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::START);
	pMove->SetDir(DIR::LEFT);
	m_vNextStage.push_back(pMove);
}

void CStageOne::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Test2.tile");
}
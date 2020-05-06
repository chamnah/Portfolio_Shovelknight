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
#include "CBubbleDragon.h"
#include "CBubbleAttack.h"
#include "CBackGroundObj.h"
#include "CStone.h"
#include "CPlatter.h"
#include "CDart.h"
#include "CArmorer.h"
#include "SoundMgr.h"
#include "CMoveTile.h"
#include "CFireBlock.h"
#include "CBlock.h"
#include "CSlime.h"

CStageStart::CStageStart()
{
	LoadObj(L"Object\\Stage1.obj");
	//m_vMonster.push_back(tMonster(true, Vec2(100, 300), false, false, M_TYPE::BEETO));
	//m_vMonster.push_back(tMonster(true, Vec2(800, 700), false, false, M_TYPE::BUBBLE_DRAGON));
}

CStageStart::~CStageStart()
{
}

void CStageStart::Init()
{
	if (m_eDir == DIR::RIGHT)
		CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);
	else if (m_eDir == DIR::LEFT)
		CCamMgr::GetInst()->SetLook(CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE - CCore::GetInst()->GetResolution().x / 2.f, CCore::GetInst()->GetResolution().y / 2.f);

	CStageMgr::GetInst()->SetPath(L"Object\\Hidden\\HiddenTest.obj");

	CreateMonster();

	CObj* pObj = nullptr;

	CreateFiniteObj();

	CStageMove* pMove = new CStageMove;
	pMove->SetRealPos((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) + 50.f, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::TWO);
	pMove->SetDir(DIR::RIGHT);
	m_vNextStage.push_back(pMove);

	pMove = new CStageMove;
	pMove->SetRealPos(-32, 480);
	pMove->SetScale(Vec2(64, 700));
	pMove->Init();
	pMove->SetStage(STAGE::MAP);
	pMove->SetDir(DIR::NONE);
	m_vNextStage.push_back(pMove);

	/*CBeeto* pBeeto = new CBeeto(100, 700);
	pBeeto->SetRealPos(100, 700);
	pBeeto->Init();
	m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pBeeto);*/

	CSoundMgr::GetInst()->PlayBGM(L"BGM_02");

}

void CStageStart::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"BackGround", L"Image\\Sky.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);
	StageMoveInit(L"Tile\\Test.tile");
}
#include "stdafx.h"
#include "CStageMove.h"
#include "CStage.h"
#include "CStageMgr.h"

CStageMove::CStageMove()
	:m_pStage(nullptr)
{
	m_vScale = Vec2(TILE_SIZE, TILE_SIZE);
}

CStageMove::~CStageMove()
{
}

int CStageMove::OnCollisionEnter(CCollider * _other)
{
	m_pStage->LoadTile(m_wstrBack);
	CStageMgr::GetInst()->AddBackGround(RIGHT);
	return 0;
}
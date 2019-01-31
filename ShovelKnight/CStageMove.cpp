#include "stdafx.h"
#include "CStageMove.h"

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
	return 0;
}
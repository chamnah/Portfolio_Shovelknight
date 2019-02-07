#include "stdafx.h"
#include "CStageMove.h"
#include "CStageMgr.h"
#include "CCollider.h"

CStageMove::CStageMove()
	: m_bDeath(false)
	, m_eDir(DIR::NONE)
	, m_eStage(STAGE::ONE)
	, m_bReset(false)
{
	m_eType = OBJ_TYPE::STAGE_MOVE;
	m_vScale = Vec2(TILE_SIZE, TILE_SIZE);
	CCamObj::Init();
}

CStageMove::~CStageMove()
{
}

DIR CStageMove::OnCollisionEnter(CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		m_bDeath = true;
	}
	return DIR::NONE;
}

int CStageMove::update()
{
	if (m_bDeath)
	{
		CStageMgr::GetInst()->AddCheck(OBJ_TYPE::UI);
		CStageMgr::GetInst()->AddCheck(OBJ_TYPE::PLAYER);
		CStageMgr::GetInst()->CopyStage(m_eStage,OBJ_TYPE::UI);
		CStageMgr::GetInst()->CopyStage(m_eStage, OBJ_TYPE::PLAYER);
		CStageMgr::GetInst()->SetStageDir(m_eStage, m_eDir);
		CStageMgr::GetInst()->ChangeStage(m_eStage);
		return CHANGE_STAGE;
	}

	CCamObj::update();
	return 0;
}

void CStageMove::render(HDC _dc)
{
	CCamObj::render(_dc);
}

int CStageMove::lateupdate()
{
	return 0;
}
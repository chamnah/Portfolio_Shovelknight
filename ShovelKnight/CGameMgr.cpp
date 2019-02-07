#include "stdafx.h"
#include "CGameMgr.h"
#include "CStageMgr.h"

CGameMgr::CGameMgr()
	:m_iGold(0)
	, m_bDeath(false)
	, m_bExit(false)
	, m_bItemChange(false)
{
}

CGameMgr::~CGameMgr()
{
}

int CGameMgr::Update()
{
	if (m_bDeath)
	{
		m_bDeath = false;

		CStageMgr::GetInst()->ResetStage();
	}

	return 0;
}

bool CGameMgr::IsSaveStage(STAGE _eStage)
{
	for (UINT i = 1; i < m_vecSaveStage.size(); ++i)
	{
		if (m_vecSaveStage[i].eStage == _eStage)
			return true;
	}
	return false;
}
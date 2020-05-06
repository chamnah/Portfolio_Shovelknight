#include "stdafx.h"
#include "CGameMgr.h"
#include "CStageMgr.h"
#include "CItem.h"
#include "CDCMgr.h"
#include "CResMgr.h"
#include "CPlayer.h"
#include "CKeyMgr.h"

CGameMgr::CGameMgr()
	:m_iGold(0)
	, m_bDeath(false)
	, m_bExit(false)
	, m_bItemChange(false)
	, m_eItemType(ITEM_TYPE::NONE)
	, m_eEquipType(EQUIP_TYPE::END)
	, m_iEquipPrice{}
{
	for (UINT i = 0; i < (UINT)EQUIP_TYPE::END; ++i)
	{
		m_iEquipPrice[i] = i * 1000;
	}
}

CGameMgr::~CGameMgr()
{
}

int CGameMgr::Update()
{
	if (KEY(KEY_TYPE::KEY_6, KEY_STATE::AWAY))
		m_iGold += 1000;

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

void CGameMgr::SetEquipSetting()
{
	if (m_eEquipType >= EQUIP_TYPE::END)
		return;
	
	CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0]->SetTexture(CDCMgr::GetInst()->GetPlayerTex((int)m_eEquipType));
	((CPlayer*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->SetOriTex(CDCMgr::GetInst()->GetPlayerTex((int)m_eEquipType));
}
#include "stdafx.h"
#include "CStageMap.h"
#include "CKeyMgr.h"
#include "CSD_Player.h"
#include "CStageMgr.h"
#include "CTile.h"
#include "CMapObj.h"
#include "CGameMgr.h"
#include "CCore.h"
#include "CCamMgr.h"

CStageMap::CStageMap()
{
	m_vPlayerPos = Vec2(224, 480);
}

CStageMap::~CStageMap()
{
}

void CStageMap::Init()
{
	CSD_Player* pSD = new CSD_Player;
	pSD->SetPos(m_vPlayerPos);
	pSD->Init();
	m_vObj[(UINT)OBJ_TYPE::PLAYER].push_back(pSD);
}

int CStageMap::Progress()
{
	if (CStageMgr::GetInst()->GetIsChange())
	{
		if (CStageMgr::GetInst()->GetChange() == STAGE::START)
			CGameMgr::GetInst()->AddSaveStage(tSave(CStageMgr::GetInst()->GetChange(), Vec2(700, 750)));
		else if (CStageMgr::GetInst()->GetChange() == STAGE::KING)
		{
			CCamMgr::GetInst()->SetLook((CCore::GetInst()->GetResolution().x / 2.f), CCore::GetInst()->GetResolution().y + CCore::GetInst()->GetResolution().y / 2.f);
			CGameMgr::GetInst()->AddSaveStage(tSave(CStageMgr::GetInst()->GetChange(), Vec2(300, 750)));
		}
		else if (CStageMgr::GetInst()->GetChange() == STAGE::M_ONE)
		{
			CGameMgr::GetInst()->AddSaveStage(tSave(CStageMgr::GetInst()->GetChange(), Vec2(400, 600)));
		}
		CStageMgr::GetInst()->SetIsChange(false);
		CStageMgr::GetInst()->SetStageDir(CStageMgr::GetInst()->GetChange(),DIR::NONE);
		CStageMgr::GetInst()->ChangeStage(CStageMgr::GetInst()->GetChange());
		return 0;
	}

	CGameStage::Progress();
	Vec2 vPos = m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos();
	int iX = (int)vPos.x / TILE_SIZE;
	int iY = (int)vPos.y / TILE_SIZE;
	int iIdx = (iY * CStageMgr::GetInst()->GetTileSizeX()) + iX;
	
	if (((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetMove() == false)
	{
		if (KEY(KEY_TYPE::KEY_LEFT, KEY_STATE::TAB))
		{
			while (true)
			{
				--iIdx;
				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::COLL)
					break;

				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::NODE)
				{
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetIsMove(true);
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetEndPos(Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f));
					m_vPlayerPos = Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f);
					break;
				}
				if ((iIdx % CStageMgr::GetInst()->GetTileSizeX() == 0))
					break;
			}
		}
		else if (KEY(KEY_TYPE::KEY_RIGHT, KEY_STATE::TAB))
		{
			while (true)
			{
				++iIdx;
				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::COLL)
					break;

				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::NODE)
				{
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetIsMove(true);
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetEndPos(Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f));
					m_vPlayerPos = Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f);
					break;
				}
				if ((iIdx % CStageMgr::GetInst()->GetTileSizeX() - 1 == 0))
					break;
			}
		}
		else if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB))
		{
			while (true)
			{
				iIdx -= CStageMgr::GetInst()->GetTileSizeX();
				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::COLL)
					break;

				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::NODE)
				{
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetIsMove(true);
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetEndPos(Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f));
					m_vPlayerPos = Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f);
					break;
				}
				if ((iIdx % CStageMgr::GetInst()->GetTileSizeX() - 1 == 0))
					break;
			}
		}
		else if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
		{
			while (true)
			{
				iIdx += CStageMgr::GetInst()->GetTileSizeX();
				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::COLL)
					break;

				if (((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->GetTileType() == TILE_TYPE::NODE)
				{
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetIsMove(true);
					((CSD_Player*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetEndPos(Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f));
					m_vPlayerPos = Vec2(m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().x + 32.f, m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->GetPos().y + 32.f);
					break;
				}
				if ((iIdx % CStageMgr::GetInst()->GetTileSizeX() - 1 == 0))
					break;
			}
		}
	}
	return 0;
}

void CStageMap::Enter()
{
	CGameMgr::GetInst()->ClearSaveStage();
	CStageMgr::GetInst()->ClearCheck();
	CStage::Exit();
	LoadObj(L"Object\\Map.obj");
	StageMoveInit(L"Tile\\Map.tile");
}
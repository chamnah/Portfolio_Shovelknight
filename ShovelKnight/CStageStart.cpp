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

CStageStart::CStageStart()
{

}

CStageStart::~CStageStart()
{
	DeleteObject(m_hBit);
	DeleteDC(m_hDC);
}

int CStageStart::Progress()
{
	CStage::Update();

	CCamMgr::GetInst()->SetLook(m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x, CCamMgr::GetInst()->GetLook().y);
	
	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
	{
		CStageMgr::GetInst()->ChangeStage(STAGE::TOOL);
	}

	return 0;
}

void CStageStart::Enter()
{
	LoadTile(L"Tile\\Test.tile");

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->Init();

	m_hBit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), m_iTileSizeX * TILE_SIZE, m_iTileSizeY * TILE_SIZE);
	m_hDC =  CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	HBITMAP  Old = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(Old);
	
	TileDCRender(m_hDC);

	CObj* pObj = NULL;
	pObj = new CPlayer;
	pObj->SetPos(Vec2(500, 650));
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::PLAYER].push_back(pObj);

	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::TILE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::MONSTER, (UINT)OBJ_TYPE::TILE);

	
	pObj = new CBeeto(600, 300);
	m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);
}
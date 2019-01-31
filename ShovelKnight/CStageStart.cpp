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
		CStageMgr::GetInst()->ChangeStage(STAGE::TOOL);

	return 0;
}

void CStageStart::Enter()
{
	LoadTile(L"Tile\\Test.tile");

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->Init();

	m_hBit = CreateCompatibleBitmap(CCore::GetInst()->GetMainDC(), CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE, CStageMgr::GetInst()->GetTileSizeY() * TILE_SIZE);
	m_hDC =  CreateCompatibleDC(CCore::GetInst()->GetMainDC());
	HBITMAP  Old = (HBITMAP)SelectObject(m_hDC, m_hBit);
	DeleteObject(Old);
	
	TileDCRender(m_hDC);

	CObj* pObj = NULL;
	CObj* pPlayer = new CPlayer;
	pPlayer->SetPos(Vec2(500, 750));
	pPlayer->Init();
	m_vObj[(UINT)OBJ_TYPE::PLAYER].push_back(pPlayer);

	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::TILE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::MONSTER);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::MONSTER, (UINT)OBJ_TYPE::TILE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::SKILL, (UINT)OBJ_TYPE::MONSTER);

	pObj = new CBeeto(400, 300);
	m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pObj);

	pObj = new CUI;
	pObj->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"HUD",L"Image\\HUD.bmp"));
	pObj->SetPos(0,0);
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pObj);

	CTextUI* pTexUI = new CTextUI;
	pTexUI->SetPos(35,5);
	pTexUI->SetType(GOLD);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);
	
	pTexUI = new CTextUI;
	pTexUI->SetPos(335, 5);
	pTexUI->SetType(ITEM);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	pTexUI = new CTextUI;
	pTexUI->SetPos(670, 5);
	pTexUI->SetType(LIFE);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	pTexUI = new CTextUI;
	pTexUI->SetPos(1370, 5);
	pTexUI->SetType(BOSS);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	CHpUI* pHpUI = nullptr;

	for (int i = 0; i < (((CDynamicObj*)pPlayer)->GetMaxHP() / 2); ++i)
	{
		pHpUI = new CHpUI;
		pHpUI->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Life", L"Image\\Life.bmp"));
		pHpUI->SetPos(670 + i * 35, 33);
		pHpUI->SetScale(Vec2(32, 32));
		((CUI*)pObj)->AddChildUI(UI_TYPE::HP, pHpUI);
	}
}
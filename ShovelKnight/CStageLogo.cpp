#include "stdafx.h"
#include "CStageLogo.h"
#include "CResMgr.h"
#include "CCamMgr.h"
#include "CBackGroundObj.h"
#include "CImageObj.h"
#include "CKeyMgr.h"
#include "CStageMgr.h"
#include "CPlayer.h"
#include "CTextUI.h"
#include "CHpUI.h"
#include "CCollisionMgr.h"
#include "CScoreUI.h"
#include "CGameMgr.h"
#include "CItemUI.h"

int CStageLogo::Progress()
{
	CStage::Update();

	if (KEY_MGR(KEY_TYPE::KEY_UP, KEY_STATE::AWAY))
	{
		m_eSelect = (SELECT)((int)m_eSelect - 1);
		if ((int)m_eSelect < (int)SELECT::START)
			m_eSelect = SELECT::EXIT;
	}

	else if (KEY_MGR(KEY_TYPE::KEY_DOWN, KEY_STATE::AWAY))
	{
		m_eSelect = (SELECT)((int)m_eSelect + 1);
		if ((int)m_eSelect > (int)SELECT::EXIT)
			m_eSelect = SELECT::START;
	}
	
	if (KEY_MGR(KEY_TYPE::KEY_ENTER, KEY_STATE::AWAY))
	{
		if (m_eSelect == SELECT::START)
		{
			CGameMgr::GetInst()->AddSaveStage(tSave(STAGE::START, Vec2(500, 750)));
			Start();

			CStageMgr::GetInst()->AddCheck(OBJ_TYPE::PLAYER);
			CStageMgr::GetInst()->AddCheck(OBJ_TYPE::UI);
			CStageMgr::GetInst()->CopyStage(CGameMgr::GetInst()->LastSave().eStage,OBJ_TYPE::PLAYER);
			CStageMgr::GetInst()->CopyStage(CGameMgr::GetInst()->LastSave().eStage, OBJ_TYPE::UI);
			CStageMgr::GetInst()->ChangeStage(CGameMgr::GetInst()->LastSave().eStage);
		}
		else if (m_eSelect == SELECT::TOOL)
			CStageMgr::GetInst()->ChangeStage(STAGE::TOOL);
		else if (m_eSelect == SELECT::EXIT)
			return INT_MAX;

		return 0;
	}

	if (m_eSelect == SELECT::START)
		m_vObj[(UINT)OBJ_TYPE::OBJECT][0]->SetPos(500, 550);
	else if (m_eSelect == SELECT::TOOL)
		m_vObj[(UINT)OBJ_TYPE::OBJECT][0]->SetPos(500, 650);
	else if (m_eSelect == SELECT::EXIT)
		m_vObj[(UINT)OBJ_TYPE::OBJECT][0]->SetPos(500, 750);

	return 0;
}

void CStageLogo::Render(HDC _hdc)
{
	RECT rt = {650,550,900,650};
	RECT rt1 = { 650,650,900,750 };
	RECT rt2 = { 650,750,900,850 };
	
	CStage::Render(_hdc);
	SetTextColor(_hdc,RGB(255,255,255));
	SetBkMode(_hdc, TRANSPARENT);
	HFONT OldFont = (HFONT)SelectObject(_hdc, m_Font);

	DrawText(_hdc, L"START", -1, &rt, DT_CENTER);
	DrawText(_hdc, L"MAP TOOL", -1, &rt1, DT_CENTER);
	DrawText(_hdc, L"EXIT", -1, &rt2, DT_CENTER);
	SelectObject(_hdc, OldFont);
}

void CStageLogo::Enter()
{
	AddFontResource(L"..\\bin\\power_pixel-7.ttf");
	AddFontResource(L"..\\bin\\slkscr.ttf");
	AddFontResource(L"..\\bin\\slkscre.ttf");
	CBackGroundObj* pBack = new CBackGroundObj;
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);

	CObj* pObj = new CImageObj;
	pObj->SetPos(200, 200);
	pObj->SetTexture(TEX_LOAD(L"Select",L"Image\\Select.bmp"));
	m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);

	m_eSelect = SELECT::START;
}

void CStageLogo::Start()
{
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::TILE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::STAGE_MOVE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::MONSTER);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::DROP);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::PLAYER, (UINT)OBJ_TYPE::OBJECT);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::DROP, (UINT)OBJ_TYPE::TILE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::MONSTER, (UINT)OBJ_TYPE::TILE);
	CCollisionMgr::GetInst()->OnCollCheck((UINT)OBJ_TYPE::SKILL, (UINT)OBJ_TYPE::MONSTER);

	CObj* pObj = new CPlayer;
	pObj->SetPos(CGameMgr::GetInst()->LastSave().vPos);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::PLAYER].push_back(pObj);

	pObj = new CUI;
	pObj->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"HUD", L"Image\\HUD.bmp"));
	pObj->SetPos(0, 0);
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pObj);

	CTextUI* pTexUI = new CTextUI;
	pTexUI->SetPos(35, 5);
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

	CScoreUI* pScore = new CScoreUI;
	pScore->SetPos(50, 30);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pScore);

	CItemUI* pItem = new CItemUI;
	pItem->SetPos(290,35);
	pItem->SetItemType(ITEM_TYPE::NONE);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pItem);

	CHpUI* pHP = nullptr;

	for (int i = 0; i < (((CDynamicObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetMaxHP() / 2); ++i)
	{
		pHP = new CHpUI;
		pHP->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Life", L"Image\\Life.bmp"));
		pHP->SetPos(670 + i * 35, 33);
		pHP->SetScale(Vec2(32, 32));
		((CUI*)pObj)->AddChildUI(UI_TYPE::HP, pHP);
	}
}

CStageLogo::CStageLogo()
{
	m_Font = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Silkscreen");
}


CStageLogo::~CStageLogo()
{
	DeleteObject(m_Font);
}
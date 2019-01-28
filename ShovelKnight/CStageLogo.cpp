#include "stdafx.h"
#include "CStageLogo.h"
#include "CResMgr.h"
#include "CCamMgr.h"
#include "CBackGroundObj.h"
#include "CImageObj.h"
#include "CKeyMgr.h"
#include "CStageMgr.h"

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
			CStageMgr::GetInst()->ChangeStage(STAGE::START);
		else if (m_eSelect == SELECT::TOOL)
			CStageMgr::GetInst()->ChangeStage(STAGE::TOOL);
		else if (m_eSelect == SELECT::EXIT)
			return INT_MAX;
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

	DrawText(_hdc, L"게임 시작", -1, &rt, DT_CENTER);
	DrawText(_hdc, L"맵 툴", -1, &rt1, DT_CENTER);
	DrawText(_hdc, L"나가기", -1, &rt2, DT_CENTER);
	SelectObject(_hdc, OldFont);
}

void CStageLogo::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);

	CObj* pObj = new CImageObj;
	pObj->SetPos(200, 200);
	pObj->SetTexture(TEX_LOAD(L"Select",L"Image\\Select.bmp"));
	m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);

	m_eSelect = SELECT::START;
}

void CStageLogo::Exit()
{
	
}

CStageLogo::CStageLogo()
{
	m_Font = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"Rix미니버스 R");
}


CStageLogo::~CStageLogo()
{
	DeleteObject(m_Font);
}
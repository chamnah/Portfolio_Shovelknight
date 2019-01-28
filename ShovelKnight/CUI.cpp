#include "stdafx.h"
#include "CUI.h"
#include "CResMgr.h"
#include "CKeyMgr.h"

CUI::CUI()
	:m_pParent(NULL),
	m_bIsMouse(false),
	m_bMouseOnLB(false)
{
	m_vScale = Vec2(0,0);
	m_vecChildUI.resize((UINT)UI_TYPE::END);
}

CUI::~CUI()
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		for(UINT j = 0; j < m_vecChildUI[i].size();++j)
		{
			delete m_vecChildUI[i][j];
		}
		m_vecChildUI[i].clear();
	}
	m_vecChildUI.clear();
}

int CUI::update()
{
	m_vFinalPos = m_vPos;

	if (m_pParent)
		m_vFinalPos = m_vFinalPos + m_pParent->GetPos();

	ChildUpdate();

	return 0;
}

void CUI::render(HDC _dc)
{
	BitBlt(_dc,(int)m_vFinalPos.x, (int)m_vFinalPos.y,m_pTex->GetWidth(),m_pTex->GetHeight(),m_pTex->GetDC(),0,0,SRCCOPY);
	ChildRender(_dc);
}

bool CUI::IsMouseOn()
{
	Vec2 MousePos = CKeyMgr::GetInst()->GetMousePos();

	if (m_vFinalPos.x <= MousePos.x && m_vFinalPos.y <= MousePos.y
		&& m_vFinalPos.x + (float)m_vScale.x >= MousePos.x && m_vFinalPos.y + (float)m_vScale.y >= MousePos.y)
	{
		return true;
	}
	return false;
}

int CUI::ChildUpdate()
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		for (UINT j = 0; j < m_vecChildUI[i].size(); ++j)
		{
			m_vecChildUI[i][j]->update();
		}
	}
	return 0;
}

void CUI::ChildRender(HDC _dc)
{
	for (UINT i = 0; i < m_vecChildUI.size(); ++i)
	{
		for (UINT j = 0; j < m_vecChildUI[i].size(); ++j)
		{
			m_vecChildUI[i][j]->render(_dc);
		}
	}
}

void CUI::AddChildUI(UI_TYPE _eType, CUI*  _UI)
{
	m_vecChildUI[(UINT)_eType].push_back(_UI);
	_UI->m_pParent = (CUI*)this;
}

void CUI::ClearUI(UI_TYPE _eType)
{
	for (UINT i = 0; i < m_vecChildUI[(UINT)_eType].size(); ++i)
	{
		delete m_vecChildUI[(UINT)_eType][i];
	}

	m_vecChildUI[(UINT)_eType].clear();
}
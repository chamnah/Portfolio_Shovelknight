#include "stdafx.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CStageMgr.h"
#include "CCamMgr.h"
CBackGroundObj::CBackGroundObj()
{
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Menu",L"Image\\menu.bmp");
	m_eType = OBJ_TYPE::BACK;
}

CBackGroundObj::~CBackGroundObj()
{
}

int CBackGroundObj::update()
{
	return 0;
}

void CBackGroundObj::render(HDC _dc)
{
	BitBlt(_dc, (int)m_vPos.x, (int)m_vPos.y, (int)m_pTex->GetWidth(), (int)m_pTex->GetHeight(), m_pTex->GetDC(), 0, 0, SRCCOPY);
}
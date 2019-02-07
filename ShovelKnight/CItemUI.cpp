#include "stdafx.h"
#include "CItemUI.h"
#include "CResMgr.h"
#include "CGameMgr.h"

CItemUI::CItemUI()
	:m_eItemType(ITEM_TYPE::NONE)
{
	m_vScale = Vec2(13, 13);
	m_pTex = TEX_LOAD(L"Item", L"Image\\Item.bmp");
}

CItemUI::~CItemUI()
{
}

int CItemUI::update()
{
	m_eItemType = CGameMgr::GetInst()->GetItemType();
	return 0;
}

void CItemUI::render(HDC _dc)
{
	TransparentBlt(_dc, m_vPos.x - m_vScale.x * 2, m_vPos.y - m_vScale.y * 2, m_vScale.x * 4, m_vScale.y * 4, m_pTex->GetDC(), (int)m_eItemType * m_vScale.x, 0, m_vScale.x, m_vScale.y, RGB(0, 255, 0));
}
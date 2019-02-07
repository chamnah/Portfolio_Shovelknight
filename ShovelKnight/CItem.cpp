#include "stdafx.h"
#include "CItem.h"
#include "CResMgr.h"

CItem::CItem()
	:m_eItemType(ITEM_TYPE::NONE)
{
	m_vScale = Vec2(13, 13);
	m_pTex = TEX_LOAD(L"Item",L"Image\\Item.bmp");
}

CItem::~CItem()
{
}

int CItem::update()
{
	return 0;
}

void CItem::render(HDC _dc)
{
	TransparentBlt(_dc,m_vPos.x - m_vScale.x * 2,m_vPos.y - m_vScale.y * 2,m_vScale.x * 4,m_vScale.y * 4,m_pTex->GetDC(),(int)m_eItemType * m_vScale.x,0,m_vScale.x,m_vScale.y,RGB(0,255,0));
}
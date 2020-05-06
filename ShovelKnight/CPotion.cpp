#include "stdafx.h"
#include "CPotion.h"
#include "CCamMgr.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CStageMgr.h"
#include "CUI.h"
#include "CDynamicObj.h"

CPotion::CPotion()
{
	m_pTex = TEX_LOAD(L"Chicken",L"Image\\Chicken.bmp");
	m_tSize = tSize(m_pTex->GetWidth(), m_pTex->GetHeight());
	m_vScale = Vec2(m_tSize.x * 4, m_tSize.y * 4);
	m_eType = OBJ_TYPE::OBJECT;
}

CPotion::~CPotion()
{
}

int CPotion::update()
{
	if (m_bDeath)
		return INT_MAX;

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);
	CCamObj::update();

	
	return 0;
}

void CPotion::render(HDC _dc)
{
	TransparentBlt(_dc, m_vPos.x - m_tSize.x * 2, m_vPos.y - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4, m_pTex->GetDC(), 0, 0, m_tSize.x, m_tSize.y, RGB(0, 255, 0));
	CCamObj::render(_dc);
}

DIR CPotion::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		((CDynamicObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->SetHP(((CDynamicObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->GetHP() + 1);
		((CUI*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI][0])->Cure(1);
		m_bDeath = true;
	}
	return DIR::NONE;
}
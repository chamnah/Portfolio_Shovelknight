#include "stdafx.h"
#include "CMapObj.h"
#include "CCollider.h"
#include "CKeyMgr.h"
#include "CStageMgr.h"

CMapObj::CMapObj()
	:m_eStage(STAGE::END)
{
	m_eType = OBJ_TYPE::OBJECT;
}

CMapObj::~CMapObj()
{
}

int CMapObj::update()
{
	CCamObj::update();

	return 0;
}

void CMapObj::render(HDC _dc)
{
	TransparentBlt(_dc, (int)m_vPos.x - (m_tSize.x * 2), (int)m_vPos.y - (m_tSize.y * 2), m_tSize.x * 4, m_tSize.y * 4, m_pTex->GetDC(), 0, 0, m_tSize.x, m_tSize.y, RGB(0, 255, 0));
	CCamObj::render(_dc);
}

void CMapObj::OnCollision(CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
		{
			CStageMgr::GetInst()->SetIsChange(true);
			CStageMgr::GetInst()->SetChange(m_eStage);
		}
			m_bDeath = true;
	}
}
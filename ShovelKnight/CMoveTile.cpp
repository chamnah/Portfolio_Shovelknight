#include "stdafx.h"
#include "CMoveTile.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CCamMgr.h"

CMoveTile::CMoveTile()
	:m_fSpeed(0.f)
{
	m_pTex = TEX_LOAD(L"MoveTile",L"Image\\MoveTile.bmp");
	m_tSize = tSize(m_pTex->GetWidth(),m_pTex->GetHeight());
	m_fSpeed = 200.f;
	m_eType = OBJ_TYPE::MOVE_BLOCK;
	m_vScale = Vec2(188,60);
}

CMoveTile::~CMoveTile()
{
}

void CMoveTile::Init()
{
	m_vEndPos.x = m_vStartPos.x + 400;
	CCamObj::Init();
}

int CMoveTile::update()
{
	if (m_vRealPos.x <= m_vStartPos.x)
		m_fSpeed = 200.f;
	else if (m_vEndPos.x <= m_vRealPos.x)
		m_fSpeed = -200.f;
	
	m_vRealPos.x += m_fSpeed * DT;

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	CCamObj::update();
	return 0;
}

void CMoveTile::render(HDC _dc)
{
	TransparentBlt(_dc, m_vPos.x - m_tSize.x * 2, m_vPos.y - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4, m_pTex->GetDC(), 0, 0, m_tSize.x, m_tSize.y, RGB(0, 255, 0));
	CCamObj::render(_dc);
}

void CMoveTile::OnCollision(CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		((CCamObj*)_other->GetOwner())->SetRealPos(((CCamObj*)_other->GetOwner())->GetRealPos().x + m_fSpeed * DT, ((CCamObj*)_other->GetOwner())->GetRealPos().y);
	}
}
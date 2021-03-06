#include "stdafx.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CCamObj.h"

DWORD CCollider::g_dwKey = 0;

CCollider::CCollider()
	:m_vPos(0, 0)
	, m_vScale(0, 0)
	, m_vOffset(0, 0)
	, m_dwKey(g_dwKey++)
	, m_vRealPos{}
{
	m_Pen = CreatePen(PS_NONE, 2, RGB(0, 255, 0));
}

CCollider::~CCollider()
{
	DeleteObject(m_Pen);
}

int CCollider::Update()
{
	m_vPrePos = m_vPos;
	m_vPos = m_pOwner->GetPos() + m_vOffset;
	CCollisionMgr::GetInst()->AddCollider(m_pOwner->GetType(),this);
	return 0;
}

void CCollider::Render(HDC _dc)
{
	if (CCollisionMgr::GetInst()->GetCollView())
	{
		HPEN OldPen = (HPEN)SelectObject(_dc, m_Pen);
		MoveToEx(_dc, int(m_vPos.x - m_vScale.x / 2.f), int(m_vPos.y - m_vScale.y / 2.f), NULL);
		LineTo(_dc, int(m_vPos.x + m_vScale.x / 2.f), int(m_vPos.y - m_vScale.y / 2.f));
		LineTo(_dc, int(m_vPos.x + m_vScale.x / 2.f), int(m_vPos.y + m_vScale.y / 2.f));
		LineTo(_dc, int(m_vPos.x - m_vScale.x / 2.f), int(m_vPos.y + m_vScale.y / 2.f));
		LineTo(_dc, int(m_vPos.x - m_vScale.x / 2.f), int(m_vPos.y - m_vScale.y / 2.f));
		SelectObject(_dc, OldPen);
	}
}

Vec2 CCollider::GetRealPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return ((CCamObj*)m_pOwner)->GetRealPos() + m_vOffset;
}

void CCollider::OnCollisionEnter(CCollider* _other)
{
	m_pOwner->OnCollisionEnter(this,_other);
}

void CCollider::OnCollision(CCollider* _other)
{
	m_pOwner->OnCollision(_other);
}

void CCollider::OnCollisionExit(CCollider* _other)
{
	m_pOwner->OnCollisionExit(_other);
}   
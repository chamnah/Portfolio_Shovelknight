#include "stdafx.h"
#include "CDynamicObj.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CTimeMgr.h"

CDynamicObj::CDynamicObj()
	:  m_iHP(0)
	, m_fJump(0.f)
	, m_fGA(1200.f)
	, m_iMaxHP(0.f)
	, m_eDir(DIR::NONE)
	, m_bDamage(false)
	, m_fAccTime(0.f)
	, m_fAccJump(0.f)
	, m_pAnim(nullptr)
{
	m_pAnim = new CAnimator(this);
}

CDynamicObj::~CDynamicObj()
{
	if (m_pAnim != nullptr)
		delete m_pAnim;

	m_pAnim = nullptr;
}

void CDynamicObj::IsJumpUP()
{
}

void CDynamicObj::IsJumpDOWN()
{
}

DIR CDynamicObj::OnCollisionEnter(CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE)
	{
		DIR eDir = DIR::NONE;
		// y축 충돌
		if (abs(m_pColl->GetPrePos().x - _other->GetPrePos().x) + 1 <= m_pColl->GetScale().x / 2.f + _other->GetScale().x / 2.f) // 이미 그 이전에 x축 충돌을 했다면
		{
			float fUP = 0.f;
			if (m_pColl->GetPrePos().y < _other->GetPrePos().y) // 위쪽이라면
			{
				m_vPos.y = _other->GetPos().y - (_other->GetScale().y / 2.f) - (m_pColl->GetScale().y / 2.f) - m_pColl->GetOffset().y;
				m_vRealPos.y = m_vPos.y;
			}
			else if (m_pColl->GetPrePos().y > _other->GetPrePos().y)// 아래쪽이라면
			{
				/*fUP = abs((_other->GetPos().y + _other->GetScale().y / 2.f) - (m_pColl->GetPos().y - m_pColl->GetScale().y / 2.f));

				if ((m_pColl->GetPos().y + fUP) <= m_pColl->GetPrePos().y)
					m_vPos.y += fUP;
				else
					m_vPos.y = (m_pColl->GetPrePos().y - m_pColl->GetOffset().y);*/
			}

			m_fJump = 0.f;
			if (m_bJump)
				m_bJump = false;

			Vec2 vPrePos = Vec2(m_pColl->GetPos().x, (m_vPos.y + m_pColl->GetOffset().y));
			m_pColl->SetPos(vPrePos);
			m_pColl->SetPrePos(vPrePos);
		}

		//x축 충돌
		if (abs(m_pColl->GetPrePos().y - _other->GetPrePos().y) < m_pColl->GetScale().y / 2.f + _other->GetScale().y / 2.f) // 이미 그 이전에 y축 충돌을 했다면
		{
			if (m_pColl->GetPrePos().x < _other->GetPrePos().x) // 왼쪽이라면
			{
				eDir = DIR::LEFT;
				m_vPos.x = (_other->GetPos().x - (_other->GetScale().x / 2.f)) - (m_pColl->GetScale().x / 2.f) - m_pColl->GetOffset().x;
				m_vRealPos.x = ((_other->GetRealPos().x - (_other->GetScale().x / 2.f)) - (m_pColl->GetScale().x / 2.f) - m_pColl->GetOffset().x) - 1;
			}
			else if (m_pColl->GetPrePos().x > _other->GetPrePos().x)//내가 오른쪽이라면
			{
				eDir = DIR::RIGHT;
				m_vPos.x = (_other->GetPos().x + (_other->GetScale().x / 2.f)) + (m_pColl->GetScale().x / 2.f) - m_pColl->GetOffset().x;
				m_vRealPos.x = ((_other->GetRealPos().x + (_other->GetScale().x / 2.f)) + (m_pColl->GetScale().x / 2.f) - m_pColl->GetOffset().x) + 1;
			}
			Vec2 vPrePos = Vec2((m_vPos.x + m_pColl->GetOffset().x), m_pColl->GetPos().y);
			m_pColl->SetPos(vPrePos);
			m_pColl->SetPrePos(vPrePos);
		}
		return eDir;
	}

	return DIR::NONE;
}

void CDynamicObj::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollisionEnter(_other);
}

void CDynamicObj::Gravity(Vec2& _vPos)
{
	m_fJump += m_fGA * DT;
	_vPos.y += m_fJump * DT;
}

void CDynamicObj::SetHP(int _iHP)
{
	m_iHP = _iHP;
}

int CDynamicObj::GetHP()
{
	return m_iHP;
}
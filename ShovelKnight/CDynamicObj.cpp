#include "stdafx.h"
#include "CDynamicObj.h"
#include "CAnimator.h"
#include "CCollider.h"

CDynamicObj::CDynamicObj()
	: m_fAcc(0.f)
	, m_iHP(0)
{
	m_fAcc = 1.5f;
	m_fAccTemp = m_fAcc;
	m_pAnim = new CAnimator(this);
}

CDynamicObj::~CDynamicObj()
{
}

void CDynamicObj::IsJumpUP()
{

}

void CDynamicObj::IsJumpDOWN()
{
}

void CDynamicObj::Gravity() // �� �Լ��� ��� �����̴� ��ü�鿡�� �߷��� ���� ��Ű�� ���� ���� �Լ��̴�.
{
	m_vPos.y += m_fAccTemp;
}

void CDynamicObj::OnCollisionEnter(CCollider * _other)
{
	if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE)
	{
		// y�� �浹
		if (abs(m_pColl->GetPrePos().x - _other->GetPrePos().x) + 1 <= m_pColl->GetScale().x / 2.f + _other->GetScale().x / 2.f) // �̹� �� ������ x�� �浹�� �ߴٸ�
		{
			float fUP = 0.f;
			if (m_pColl->GetPrePos().y < _other->GetPrePos().y) // �����̶��
			{
				m_vPos.y = _other->GetPos().y - (_other->GetScale().y / 2.f) - (m_pColl->GetScale().y / 2.f) - m_pColl->GetOffset().y;
				m_vRealPos.y = m_vPos.y;
			}
			else if (m_pColl->GetPrePos().y > _other->GetPrePos().y)// �Ʒ����̶��
			{
				/*fUP = abs((_other->GetPos().y + _other->GetScale().y / 2.f) - (m_pColl->GetPos().y - m_pColl->GetScale().y / 2.f));

				if ((m_pColl->GetPos().y + fUP) <= m_pColl->GetPrePos().y)
					m_vPos.y += fUP;
				else
					m_vPos.y = (m_pColl->GetPrePos().y - m_pColl->GetOffset().y);*/
			}

			m_fAccTemp = m_fAcc;
			if (m_bJump)
				m_bJump = false;

			Vec2 vPrePos = Vec2(m_pColl->GetPos().x, (m_vPos.y + m_pColl->GetOffset().y));
			m_pColl->SetPos(vPrePos);
			m_pColl->SetPrePos(vPrePos);
		}

		//x�� �浹
		if (abs(m_pColl->GetPrePos().y - _other->GetPrePos().y) < m_pColl->GetScale().y / 2.f + _other->GetScale().y / 2.f) // �̹� �� ������ y�� �浹�� �ߴٸ�
		{
			float fRight = 0.f;
			if (m_pColl->GetPrePos().x < _other->GetPrePos().x) // �����̶��
			{
				m_vPos.x = (_other->GetPos().x - (_other->GetScale().x / 2.f)) - (m_pColl->GetScale().x / 2.f) - m_pColl->GetOffset().x;
				m_vRealPos.x = m_vPos.x;
			}
			else if (m_pColl->GetPrePos().x > _other->GetPrePos().x)//���� �������̶��
			{
				m_vPos.x = (_other->GetPos().x + (_other->GetScale().x / 2.f)) + (m_pColl->GetScale().x / 2.f) - m_pColl->GetOffset().x;
				m_vRealPos.x = m_vPos.x;
			}
			Vec2 vPrePos = Vec2((m_vPos.x + m_pColl->GetOffset().x), m_pColl->GetPos().y);
			m_pColl->SetPos(vPrePos);
			m_pColl->SetPrePos(vPrePos);
		}
	}
}

void CDynamicObj::OnCollision(CCollider * _other)
{
	OnCollisionEnter(_other);
}

void CDynamicObj::SetHP(int _iHP)
{
	m_iHP = _iHP;
}

int CDynamicObj::GetHP()
{
	return m_iHP;
}
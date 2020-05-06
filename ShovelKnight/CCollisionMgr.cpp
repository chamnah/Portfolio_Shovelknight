#include "stdafx.h"
#include "CCollisionMgr.h"
#include "CCollider.h"
#include "CKeyMgr.h"

CCollisionMgr::CCollisionMgr()
	:m_arrCollCheck{}
	, m_bColl(false)
{
}

CCollisionMgr::~CCollisionMgr()
{
}

int CCollisionMgr::Update()
{
	if (KEY(KEY_TYPE::KEY_1, KEY_STATE::AWAY))
		m_bColl = !m_bColl;

	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i) // ��
	{
		for (UINT j = i;j < (UINT)OBJ_TYPE::END; ++j) // ��
		{
			if (m_arrCollCheck[i] & 1 << j) // �浹�� �ؾ��� �ΰ��� �����ٸ�
			{
				Collision(i,j);
			}
		}
	}

	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		m_arrColl[i].clear();
	}

	return 0;
}

void CCollisionMgr::AddCollider(OBJ_TYPE _type, CCollider * _pColl)
{
	m_arrColl[(UINT)_type].push_back(_pColl);
}

void CCollisionMgr::ResetCheck()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		m_arrCollCheck[i] = 0;
		m_arrColl[i].clear();
	}
}

// ���⼭ �浹�� �� �͵� üũ�� �߰���Ų��.
void CCollisionMgr::OnCollCheck(UINT _first, UINT _second)
{
	UINT iRow = _first;
	UINT iCol = _second;

	if (_first > _second)
	{
		iRow = _second;
		iCol = _first;
	}

	m_arrCollCheck[iRow] |= 1 << iCol;
}

// �浹ü�� �޾ƿ´�. �� �浹ü�� �⺻������ Offset�� ���ԵǾ� �ִ�.
bool CCollisionMgr::IsCollision(CCollider* pFirst,CCollider* pSecond)
{
	if (abs(pFirst->GetPos().x - pSecond->GetPos().x) > pFirst->GetScale().x / 2 + pSecond->GetScale().x / 2 ||
		abs(pFirst->GetPos().y - pSecond->GetPos().y) > pFirst->GetScale().y / 2 + pSecond->GetScale().y / 2)
	{
		return false;
	}

	return true;
}

void CCollisionMgr::Collision(UINT _row, UINT _coll)
{
	if (m_arrColl[_row].size() == 0 || m_arrColl[_coll].size() == 0)
		return;
	for (UINT i = 0;i < m_arrColl[_row].size(); ++i)
	{
		for (UINT j = 0;j < m_arrColl[_coll].size(); ++j)
		{
			CKey key;
			key.First = m_arrColl[_row][i]->GetKey();
			key.Second = m_arrColl[_coll][j]->GetKey();
			map<ULONGLONG,bool>::iterator iter = m_mapKey.find(key.Key);

			if (IsCollision(m_arrColl[_row][i], m_arrColl[_coll][j])) // �浹 �ߴ�
			{
				if (iter == m_mapKey.end())
				{
					m_mapKey.insert(make_pair(key.Key,true));
					m_arrColl[_row][i]->OnCollisionEnter(m_arrColl[_coll][j]);
					m_arrColl[_coll][j]->OnCollisionEnter(m_arrColl[_row][i]);
				}
				else if (iter->second == false)
				{
					iter->second = true;
					m_arrColl[_row][i]->OnCollisionEnter(m_arrColl[_coll][j]);
					m_arrColl[_coll][j]->OnCollisionEnter(m_arrColl[_row][i]);
				}
				else
				{
					m_arrColl[_row][i]->OnCollision(m_arrColl[_coll][j]);
					m_arrColl[_coll][j]->OnCollision(m_arrColl[_row][i]);
				}
			}
			else // �浹 ���ߴ�.
			{
				if (iter != m_mapKey.end() && iter->second == true)
				{
					iter->second = false;
					m_arrColl[_row][i]->OnCollisionExit(m_arrColl[_coll][j]);
					m_arrColl[_coll][j]->OnCollisionExit(m_arrColl[_row][i]);
				}
			}
		}
	}
}
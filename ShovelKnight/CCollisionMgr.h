#pragma once

union CKey
{
	struct
	{
		DWORD First;
		DWORD Second;
	};

	ULONGLONG Key;
};

class CCollider;
class CCollisionMgr
{
	SINGLE(CCollisionMgr)

private:
	vector<CCollider*> m_arrColl[(UINT)OBJ_TYPE::END];
	map<ULONGLONG, bool>	   m_mapKey;
	UINT			   m_arrCollCheck[(UINT)OBJ_TYPE::END];
	bool m_bColl;

public:
	int Update();

public:
	void AddCollider(OBJ_TYPE _type,CCollider* _pColl);
	void ResetCheck();
	bool GetCollView() { return m_bColl; }

public:
	void OnCollCheck(UINT _first, UINT _second);
	void Collision(UINT _row, UINT _coll);
	bool IsCollision(CCollider* pFirst, CCollider* pSecond);
};
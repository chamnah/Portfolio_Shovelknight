#pragma once
class CObj;
class CCollider
{	

private:
	Vec2 m_vPrePos;
	Vec2 m_vPos;
	Vec2 m_vScale;
	Vec2 m_vOffset;
	CObj* m_pOwner;
	static DWORD g_dwKey;
	DWORD m_dwKey;

	HPEN m_Pen;

public:
	CCollider();
	~CCollider();

public:
	int Update();
	void Render(HDC _dc);

public:
	Vec2&  GetScale() { return m_vScale; }
	void   SetScale(Vec2& _vScale) { m_vScale = _vScale; }
	Vec2&  GetPos() { return m_vPos; }
	Vec2&  GetPrePos() { return m_vPrePos; }
	void   SetPrePos(Vec2& _vPos) { m_vPrePos = _vPos; }
	DWORD  GetKey() { return m_dwKey; }
	CObj*  GetOwner() { return m_pOwner; }
	void   SetOwner(CObj* _pOwner) { m_pOwner = _pOwner; }
	Vec2&  GetOffset() { return m_vOffset; }
	void   SetOffset(Vec2& _vOffset) { m_vOffset = _vOffset; }
	void   SetPos(Vec2& _vPos) { m_vPos =_vPos; }
	

public:
	void OnCollisionEnter(CCollider* _other);
	void OnCollision(CCollider* _other);
	void OnCollisionExit(CCollider* _other);
};	
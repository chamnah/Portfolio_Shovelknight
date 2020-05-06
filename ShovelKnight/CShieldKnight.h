#pragma once
#include "CDynamicObj.h"
class CShieldKnight :
	public CDynamicObj
{
private:
	bool m_bFall;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	void SetFall(bool _bFall) { m_bFall = _bFall; }

public:
	CShieldKnight();
	virtual ~CShieldKnight();
};
#pragma once
#include "CMonster.h"
class CSlime :
	public CMonster
{
private:
	bool m_bMoving;
	DIR  m_eDirFirst;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	void Moving();
	virtual void TakeDamage(int iDamage = 0, DIR _eDir = DIR::NONE);
	void Damage(bool _bRepeat);

public:
	CSlime();
	virtual ~CSlime();
};
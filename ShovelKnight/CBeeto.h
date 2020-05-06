#pragma once
#include "CMonster.h"
class CAnimator;
class CBeeto:
	public CMonster
{
private:
	Vec2 vTemp;

public:
	void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual void OnCollision(CCollider* _other);
	virtual void TakeDamage(int iDamage = 0, DIR _eDir = DIR::NONE);

public:
	CBeeto(float _fX, float _fY);
	CBeeto(Vec2 _vPos);
	virtual ~CBeeto();
};
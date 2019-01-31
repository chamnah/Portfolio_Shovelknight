#pragma once
#include "CDynamicObj.h"
class CAnimator;
class CBeeto:
	public CDynamicObj
{
private:
	float m_fTime;

public:
	void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual int OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	CBeeto(float _fX, float _fY);
	CBeeto(Vec2 _vPos);
	virtual ~CBeeto();
};
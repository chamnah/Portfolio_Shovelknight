#pragma once
#include "CDynamicObj.h"
class CAnimator;
class CBeeto:
	public CDynamicObj
{
private:
	float m_fTime;
	int   m_iID;

public:
	void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	void SetID(int _iID) { m_iID = _iID; }

public:
	CBeeto(float _fX, float _fY);
	CBeeto(Vec2 _vPos);
	virtual ~CBeeto();
};
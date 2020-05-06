#pragma once
#include "CSkill.h"
class CDart :
	public CSkill
{
private:
	HDC m_hDC;
	HBITMAP m_hBit;
	float m_fAngle;
	float m_fRotation;
	float m_fRotAngle;
	float m_fSpeed;
	float m_fDis;
	Vec2  m_vAngleGo;
	bool  m_bGo;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetGo(bool _bGo) { m_bGo = _bGo; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	void Go();

public:
	CDart();
	virtual ~CDart();
};
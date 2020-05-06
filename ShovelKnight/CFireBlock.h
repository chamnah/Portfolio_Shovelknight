#pragma once
#include "CSkill.h"
class CFireBlock :
	public CSkill
{
public:
	float m_fSpeed;
	bool  m_bMoving;
	float m_fArr;
	bool  m_bArr;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	void Moving();

public:
	void SetMoving(bool _bMove) { m_bMoving = _bMove; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CFireBlock();
	virtual ~CFireBlock();
};
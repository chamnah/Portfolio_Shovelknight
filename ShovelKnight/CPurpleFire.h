#pragma once
#include "CSkill.h"
class CPurpleFire :
	public CSkill
{
private:
	DIR m_eDir;
	float m_fSpeed;

public:
	virtual int update();

public:
	void SetDir(DIR _eDir) { m_eDir = _eDir; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CPurpleFire();
	virtual ~CPurpleFire();
};
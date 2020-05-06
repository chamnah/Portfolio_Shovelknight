#pragma once
#include "CSkill.h"
class CBasic :
	public CSkill
{
public:
	virtual void Init();

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CBasic();
	virtual ~CBasic();
};
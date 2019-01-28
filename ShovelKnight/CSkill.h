#pragma once
#include "CObj.h"
class CCollider;
class CSkill :
	public CObj
{	
protected:
	CCollider* m_pColl;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual void OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	CSkill();
	virtual ~CSkill();
};	
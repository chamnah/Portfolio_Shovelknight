#pragma once
#include "CObj.h"
class CCollider;
class CSkill :
	public CObj
{	
protected:
	CCollider* m_pColl;
	float      m_fTime;
	float      m_fAccTime;
	CObj*      m_pOwner;
	SKILL_TYPE m_eSkillType;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetOwner(CObj* pObj) { m_pOwner = pObj; }
	void SetSkillType(SKILL_TYPE _eType) { m_eSkillType = _eType; }

public:
	virtual DIR OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other) {}

public:
	CSkill();
	virtual ~CSkill();
};	
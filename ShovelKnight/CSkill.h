#pragma once
#include "CCamObj.h"
class CCollider;
class CAnimator;
class CSkill :
	public CCamObj
{	
protected:
	float      m_fTime;
	float      m_fAccTime;
	CObj*      m_pOwner;
	SKILL_TYPE m_eSkillType;

public:
	void InitColl();
	virtual int update();
	virtual void render(HDC _dc);
	void CreateEffect(CTexture* _pTex,const Vec2& _Pos,int _iMaxSize);

public:
	void SetOwner(CObj* pObj) { m_pOwner = pObj; }
	void SetSkillType(SKILL_TYPE _eType) { m_eSkillType = _eType; }
	SKILL_TYPE GetSkillType() { return m_eSkillType; }
	CObj* GetOwner() { return m_pOwner; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual void OnCollision(CCollider* _other) {}

public:
	CSkill();
	virtual ~CSkill();
};	
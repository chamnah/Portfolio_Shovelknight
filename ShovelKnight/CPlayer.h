#pragma once
#include "CDynamicObj.h"
	
class CCollider;
class CPlayer :
	public CDynamicObj
{	
private:
	bool   m_bAttack;
	CObj*  m_pObj;
	bool   m_bJumpAttack;
	bool   m_bAlpha;
	bool   m_bDeath;
	int    m_iJumpFlag;

public:
	virtual int update();
	virtual void render(HDC _dc);
	virtual int lateupdate();

public:
	void MoveAnimation();
	void JumpUpAnimation();
	void JumpDownAnimation();

public:
	virtual DIR OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);
	virtual void OnCollisionExit(CCollider* _other);

public:
	CPlayer();
	virtual ~CPlayer();
};	
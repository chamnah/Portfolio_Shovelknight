#pragma once
#include "CDynamicObj.h"
	
class CCollider;
class CPlayer :
	public CDynamicObj
{	
private:
	ANIM_STATE m_eAnim;
	int    m_iHSize;
	int    m_iWSize;
	float  m_fJump;
	bool   m_bAttack;

public:
	virtual int update();
	virtual void render(HDC _dc);
	virtual int lateupdate();

public:
	void MoveAnimation();
	void JumpUpAnimation();
	void JumpDownAnimation();

public:
	virtual void OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);
	virtual void OnCollisionExit(CCollider* _other);

public:
	CPlayer();
	virtual ~CPlayer();
};	
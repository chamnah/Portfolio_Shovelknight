#pragma once
#include "CMonster.h"
class CCollider;
class CBubbleDragon :
	public CMonster
{
private:
	CCollider* m_pCollHead;
	CCollider* m_pCollBody[4];
	bool       m_bDamage;
	float      m_fDeathTime;
	bool	   m_bState;
	bool       m_bFirst;
	float      m_fStateTime;
	int        m_iNum;
	void(CBubbleDragon::*FP)(void);
	Vec2      m_vCollPos;
	float      m_fCoinTime;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	void AniationInit();
	void Damage(bool _bRepeat = false);
	void LeftWalking(void);
	void RightWalking();
	void BubbleAttack();
	void Idle();

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	CBubbleDragon();
	CBubbleDragon(float fPosX,float fPosY);
	virtual ~CBubbleDragon();
};
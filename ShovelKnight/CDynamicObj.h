#pragma once
#include "CCamObj.h"

class CAnimator;
class CDynamicObj :
	public CCamObj
{
protected:
	float	m_fSpeed;
	bool    m_bJump;
	int     m_iHP;
	int    m_iMaxHP;
	float  m_fJump;
	float  m_fGA;
	DIR    m_eDir;
	bool   m_bDamage;
	float  m_fAccTime;
	float  m_fAccJump;

public:
	float GetSpeed() { return m_fSpeed; };
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetIsJump(bool _bJump) { m_bJump = _bJump; }
	void SetJump(float _fJump) { m_fJump = _fJump; }
	int  GetMaxHP() { return m_iMaxHP; }
	void IsJumpUP();
	void IsJumpDOWN();
	void SetDir(DIR _dir) { m_eDir = _dir; }


public:
	void Gravity(Vec2& _vPos);

public:
	void SetHP(int _iHP);
	int  GetHP();

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual void OnCollision(CCollider* _other);
	virtual void TakeDamage(int iDamage = 0,DIR _eDir = DIR::NONE) {};

public:
	CDynamicObj();
	virtual ~CDynamicObj();
};
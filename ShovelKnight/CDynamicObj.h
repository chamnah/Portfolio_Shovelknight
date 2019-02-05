#pragma once
#include "CCamObj.h"

class CAnimator;
class CDynamicObj :
	public CCamObj
{
protected:
	float	m_fSpeed;
	bool    m_bJump;
	CAnimator* m_pAnim;
	int     m_iHP;
	int    m_iMaxHP;
	int    m_iHSize;
	int    m_iWSize;
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


public:
	void Gravity(Vec2& _vPos);

public:
	void SetHP(int _iHP);
	int  GetHP();

public:
	virtual DIR OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	CDynamicObj();
	virtual ~CDynamicObj();
};
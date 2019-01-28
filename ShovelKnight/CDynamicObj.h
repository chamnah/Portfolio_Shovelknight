#pragma once
#include "CCamObj.h"

class CAnimator;
class CDynamicObj :
	public CCamObj
{
protected:
	float	m_fSpeed;
	float   m_fAcc;
	float   m_fAccTemp;
	bool    m_bJump;
	tAnimSize m_tAnim; // 이건 보다가 쓸모없어지면 지우자
	CAnimator* m_pAnim;
	int     m_iHP;

public:
	float GetSpeed() { return m_fSpeed; };
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetAcc(float _fAcc) { m_fAcc = _fAcc; m_fAccTemp = _fAcc; }
	float GetAcc() { return m_fAccTemp; }
	void AccOn() {	m_fAccTemp = m_fAcc;}
	void AccOff() { m_fAccTemp = 0; }
	void SetJump(bool _bJump) { m_bJump = _bJump; }

	void IsJumpUP();
	void IsJumpDOWN();

    void Gravity();

public:
	void SetHP(int _iHP);
	int  GetHP();

public:
	virtual void OnCollisionEnter(CCollider* _other);
	virtual void OnCollision(CCollider* _other);

public:
	CDynamicObj();
	virtual ~CDynamicObj();
};
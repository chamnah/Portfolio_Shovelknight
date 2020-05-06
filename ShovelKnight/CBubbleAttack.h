#pragma once
#include "CSkill.h"
class CBubbleAttack :
	public CSkill
{
private:
	DIR m_eDir;
	float m_fSpeed;
	float m_fAcc;
	float m_fPosY;
	int   m_iID;

public:
	virtual void Init();
	virtual int update();
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	void SetDir(DIR _eDir) { m_eDir = _eDir; }
	void SetPosY(float _fPosY) { m_fPosY = _fPosY; }
	void SetID(int _iID) { m_iID = _iID; }

public:
	CBubbleAttack();
	virtual ~CBubbleAttack();
};
#pragma once
#include "CMonster.h"
class CBoss :
	public CMonster
{
private:
	BOSS_FSM m_FSM;
	BOSS_FSM m_eFight;
	bool     m_bSkill;
	int      m_iState;
	int      m_iNum;
	bool     m_bFirst;
	float    m_fAcc;
	bool     m_bTransform;
	bool     m_bAlpha;
	bool     m_bFire;
	CTexture* m_pTexShield;
	short    m_sRandom[25];
	bool     m_bMoving;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	void RightAttack();
	void LeftAttack();
	void DownAttack();
	void UpAttack();
	void FireAttack();
	void Figjt();
	float Normalize(float x,float y);
	bool Moving(float _fX,float _fY);
	virtual void TakeDamage(int iDamage = 0, DIR _eDir = DIR::NONE);
	void Damage(bool _bRepeat);
	void Idle();
	void Charge();
	void Shuffle();

public:
	void SetTransform(bool _bTrans) { m_bTransform = _bTrans; }
	void SetAlpha(bool _bAlpha) { m_bAlpha = _bAlpha; }
	bool GetAlpha() { return m_bAlpha; }
public:
	BOSS_FSM* GetFSM() { return &m_FSM; }

public:
	CBoss();
	virtual ~CBoss();
};
#pragma once
#include "CDynamicObj.h"
	
class CCollider;
class CAnimator;
class CPlayer :
	public CDynamicObj
{	
private:
	bool   m_bAttack;
	CObj*  m_pObj;
	bool   m_bJumpAttack;
	bool   m_bAlpha;
	int    m_iJumpFlag;
	int    m_iLadderCheck;
	float  m_fLadderY;
	float  m_fLadderDown;
	float  m_fLadderTime;
	float  m_fLadderEnd;
	float  m_fSkillTime;
	bool   m_bKing;
	bool   m_bEvent;
	CAnimator* m_pSpectrum;
	list<tFrame> m_listFrame;
	bool   m_bSkill;
	bool   m_bSpeedMoving;
	float  m_fSaveSpeed;
	float  m_fShiningTime;
	bool   m_bShining;
	CTexture* m_pTexOri;
	bool   m_bEnd;
	bool   m_bLadderExit;
	bool   m_bStop;
	int    m_iNum;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);
	virtual int lateupdate();

public:
	void MoveAnimation();
	void JumpUpAnimation();
	void JumpDownAnimation();
	void LadderAnimation();
	void Event();
	void Skill();
	void SpeedMoving();
	void SpeedMovingOff(DIR _eDir);
	void Shining();
	void Ending();
	void LadderAnim();

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual void OnCollision(CCollider* _other);
	virtual void OnCollisionExit(CCollider* _other);

public:
	void SetKing(bool _bKing) { m_bKing = _bKing; }
	void SetEvent(bool _bEvent) { m_bEvent = _bEvent; }
	bool GetEvent() { return m_bEvent; }
	void SetOriTex(CTexture* _pTex) { m_pTexOri = _pTex; }
	void Damage();
	bool GetEnd() { return m_bEnd; }

public:
	CPlayer();
	virtual ~CPlayer();
};	
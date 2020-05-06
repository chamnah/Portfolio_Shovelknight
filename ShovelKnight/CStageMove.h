#pragma once
#include "CCamObj.h"
class CStage;
class CStageMove :
	public CCamObj
{
private:
	bool    m_bDeath;
	DIR     m_eDir;
	STAGE   m_eStage;
	bool    m_bReset;

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	virtual int update();
	virtual void render(HDC _dc);
	virtual int lateupdate();

public:
	void SetStage(STAGE _eStage) { m_eStage = _eStage; }
	void SetDir(DIR _eDir) { m_eDir = _eDir; }
	void SetReset(bool _bReset) { m_bReset = _bReset; }
	void SetDeath(bool _bDeath) { m_bDeath = _bDeath; }

public:
	CStageMove();
	virtual ~CStageMove();
};
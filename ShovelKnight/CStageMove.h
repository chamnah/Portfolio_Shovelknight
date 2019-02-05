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

public:
	virtual DIR OnCollisionEnter(CCollider* _other);

public:
	virtual int update();
	virtual void render(HDC _dc);
	virtual int lateupdate();

public:
	void SetStage(STAGE _eStage) { m_eStage = _eStage; }
	void SetDir(DIR _eDir) { m_eDir = _eDir; }

public:
	CStageMove();
	virtual ~CStageMove();
};
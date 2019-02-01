#pragma once
#include "CObj.h"
class CStage;
class CStageMove :
	public CObj
{
private:
	CStage* m_pStage;
	wstring m_wstrBack;

public:
	virtual int OnCollisionEnter(CCollider* _other);

public:
	CStageMove();
	virtual ~CStageMove();
};
#pragma once
#include "CObj.h"
class CStage;
class CStageMove :
	public CObj
{
private:
	CStage* m_pStage;

public:
	virtual int OnCollisionEnter(CCollider* _other);

public:
	CStageMove();
	virtual ~CStageMove();
};
#pragma once
#include "CCamObj.h"
class CMapObj :
	public CCamObj
{
private:
	STAGE m_eStage;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetStage(STAGE _eStage) { m_eStage = _eStage; }

public:
	virtual void OnCollision(CCollider* _other);

public:
	CMapObj();
	virtual ~CMapObj();
};
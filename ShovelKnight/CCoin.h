#pragma once
#include "CCamObj.h"
class CCoin :
	public CCamObj
{
private:
	COIN_TYPE m_eCoinType;
	tSize     m_tSize;
	bool      m_bDeath;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _other);

public:
	CCoin(COIN_TYPE _eType);
	virtual ~CCoin();
};
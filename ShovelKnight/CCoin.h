#pragma once
#include "CDynamicObj.h"
class CCoin :
	public CDynamicObj
{
private:
	COIN_TYPE m_eCoinType;
	tSize     m_tSize; // �̰� ���� ������ �־ ���ľߵȴ�.
	bool      m_bDeath;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CCoin(COIN_TYPE _eType);
	virtual ~CCoin();
};
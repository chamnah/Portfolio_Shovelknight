#pragma once
#include "CCamObj.h"
class CPlatter :
	public CCamObj
{
private:
	int m_iID;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetID(int _iID) { m_iID = _iID; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CPlatter();
	virtual ~CPlatter();
};
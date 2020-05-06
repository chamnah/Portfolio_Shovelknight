#pragma once
#include "CCamObj.h"
class CStone :
	public CCamObj
{
private:
	int m_iIdx;
	int m_iID;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetID(int _iID) { m_iID = _iID; }
	void SetIndex(int _iIdx) { m_iIdx = _iIdx; }


public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CStone();
	virtual ~CStone();
};
#pragma once
#include "CCamObj.h"
class CHiddenTile :
	public CCamObj
{
private:
	bool m_bEffect;
	int m_TileWidthNum;
	int m_iIdx;
	int m_iID;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetIndex(int _iIdx) { m_iIdx = _iIdx; };
	int  GetIndex() { return m_iIdx; }
	void SetID(int _iID) { m_iID = _iID; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CHiddenTile();
	virtual ~CHiddenTile();
};
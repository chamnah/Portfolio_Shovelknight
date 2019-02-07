#pragma once
#include "CObj.h"
class CItem :
	public CObj
{
private:
	ITEM_TYPE m_eItemType;

public:
	void SetItemType(ITEM_TYPE _eType) { m_eItemType = _eType; }
	ITEM_TYPE GetItemType() { return m_eItemType; }

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	CItem();
	virtual ~CItem();
};
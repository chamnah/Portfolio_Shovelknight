#pragma once
#include "CObj.h"
class CItem :
	public CObj
{
private:
	ITEM_TYPE m_eItemType;
	bool      m_bOri;
	int       m_iPrice;
	EQUIP_TYPE m_eEuipType;

public:
	void SetItemType(int _eType) { m_eItemType = (ITEM_TYPE)_eType; }
	int GetItemType() { return (int)m_eItemType; }
	void SetOriRender(bool _bOri) { m_bOri = _bOri; }
	void SetPrice(int _iPrice) { m_iPrice = _iPrice; }
	int GetPrice() { return m_iPrice; }
	
public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	CItem();
	virtual ~CItem();
};
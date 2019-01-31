#pragma once
#include "CUI.h"
class CTextUI :
	public CUI
{
private:
	int m_iType;

public:
	virtual void render(HDC _dc);

public:
	void SetType(int _iType) { m_iType = _iType; }

public:
	CTextUI();
	virtual ~CTextUI();
};
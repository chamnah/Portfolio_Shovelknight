#pragma once
#include "CUI.h"
class CViewUI :
	public CUI
{
private:
	int   m_iType;

public:
	virtual void render(HDC _dc);
	void SetType(int _iType) { m_iType = _iType; }

public:
	CViewUI();
	virtual ~CViewUI();
};
#pragma once
#include "CUI.h"
class CHpUI :
	public CUI
{
private:
	int m_iState;

public:
	virtual void render(HDC _dc);

public:
	void SetHPState(int _iState) { m_iState = _iState; }
	int  GetHPState() { return m_iState; }

public:
	CHpUI();
	virtual ~CHpUI();
};
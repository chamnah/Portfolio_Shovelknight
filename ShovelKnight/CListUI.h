#pragma once
#include "CUI.h"
class CListUI :
	public CUI
{
private:
	wstring m_strName;
	bool    m_bSelect;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual void MouseUp();

public:
	void SetText(wstring _strName) { m_strName = _strName; }
	wstring GetText() { return m_strName; }

public:
	CListUI();
	virtual ~CListUI();
};
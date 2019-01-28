#pragma once
#include "CUI.h"
class CCategoryUI :
	public CUI
{
private:
	HPEN   m_Pen;
	HBRUSH m_Brush;
	wstring m_strName;
	UI_CATEGORY m_eCategory;

public:
	virtual void render(HDC _dc);

public:
	virtual void MouseUp();

public:
	void SetName(wstring _str) { m_strName = _str; }


public:
	CCategoryUI(UI_CATEGORY _eCategory);
	virtual ~CCategoryUI();
};
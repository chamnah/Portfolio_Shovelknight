#pragma once
#include "CUI.h"
class CBtnUI :
	public CUI
{
private:
	int m_iIdx;
	int m_iMaxIdx;
	bool m_bSelect;
	HPEN m_Pen;
	HPEN m_Old;

public:
	void Init();
	void render(HDC _dc);
	
public:
	void SetIndex(int _iIdx);

public:
	virtual void MouseOn();
	virtual void MouseOut();
	virtual void MouseUp();
	virtual void MouseDown();

public:
	CBtnUI();
	virtual ~CBtnUI();
};
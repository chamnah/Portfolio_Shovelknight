#pragma once
#include "CUI.h"
class CLayerUI :
	public CUI
{
private:
	bool	m_bSelect;
	HBITMAP m_hBit;
	HDC		m_hDC;

public:
	virtual void render(HDC _dc);
	virtual void MouseUp();

public:
	void ChangeDCSize(tSize _Size);

public:
	HDC GetDC() { return m_hDC; }

public:
	CLayerUI();
	virtual ~CLayerUI();
};
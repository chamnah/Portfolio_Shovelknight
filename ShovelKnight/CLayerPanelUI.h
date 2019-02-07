#pragma once
#include "CUI.h"
class CLayerPanelUI :
	public CUI
{
private:
	HPEN   m_Pen;
	HBRUSH m_Brush;

public:
	virtual void render(HDC _dc);

public:
	CLayerPanelUI();
	virtual ~CLayerPanelUI();
};
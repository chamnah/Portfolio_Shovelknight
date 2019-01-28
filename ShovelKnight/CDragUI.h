#pragma once
#include "CUI.h"
class CDragUI :
	public CUI
{
private:
	HPEN   m_Pen;
	HBRUSH m_Brush;
	Vec2 m_vCurMousePos;

public:
	void MouseDown();

public:
	void Init();
	int update();
	virtual void render(HDC _dc);

public:
	CDragUI();
	virtual ~CDragUI();
};
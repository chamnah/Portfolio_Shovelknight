#pragma once
#include "CUI.h"
class CCollBtnUI :
	public CUI
{
private:
	HPEN   m_Pen;
	HBRUSH m_Brush;
	HBRUSH m_TempBrush;
	HFONT  m_Font;
	TILE_TYPE m_eState;
	MOUSE_MODE m_eMode;

public:
	virtual void render(HDC _dc);

public:
	virtual void MouseUp();

public:
	CCollBtnUI(TILE_TYPE _State);
	virtual ~CCollBtnUI();
};
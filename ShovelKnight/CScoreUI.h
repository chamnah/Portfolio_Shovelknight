#pragma once
#include "CUI.h"
class CScoreUI :
	public CUI
{
private:
	HFONT m_Font;

public:
	virtual void render(HDC _dc);

public:
	CScoreUI();
	virtual ~CScoreUI();
};
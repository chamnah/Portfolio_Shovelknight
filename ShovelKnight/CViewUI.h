#pragma once
#include "CUI.h"
class CViewUI :
	public CUI
{
public:
	virtual void render(HDC _dc);

public:
	CViewUI();
	virtual ~CViewUI();
};
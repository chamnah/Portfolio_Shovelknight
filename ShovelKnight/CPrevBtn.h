#pragma once
#include "CUI.h"
class CPrevBtn :
	public CUI
{
public:
	virtual void MouseUp();

public:
	virtual void render(HDC _dc);

public:
	CPrevBtn();
	virtual ~CPrevBtn();
};
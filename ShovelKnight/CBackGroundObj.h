#pragma once
#include "CObj.h"
class CBackGroundObj :
	public CObj
{
public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	CBackGroundObj();
	~CBackGroundObj();
};
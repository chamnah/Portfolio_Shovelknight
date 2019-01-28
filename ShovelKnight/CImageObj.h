#pragma once
#include "CObj.h"
class CImageObj :
	public CObj
{
public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	CImageObj();
	virtual ~CImageObj();
};
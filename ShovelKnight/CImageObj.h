#pragma once
#include "CObj.h"
class CImageObj :
	public CObj
{
private:
	Vec2 m_vReal;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	CImageObj();
	virtual ~CImageObj();
};
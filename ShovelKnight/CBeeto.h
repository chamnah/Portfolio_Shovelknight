#pragma once
#include "CDynamicObj.h"
class CAnimator;
class CBeeto:
	public CDynamicObj
{
public:
	void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	CBeeto(float _fX, float _fY);
	CBeeto(Vec2 _vPos);
	virtual ~CBeeto();
};
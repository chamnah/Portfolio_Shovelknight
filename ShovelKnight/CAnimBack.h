#pragma once
#include "CCamObj.h"
class CAnimBack :
	public CCamObj
{
public:
	virtual void Init();

public:
	virtual int update();
	virtual void render(HDC _dc);
	void Blow();

public:
	CAnimBack();
	virtual ~CAnimBack();
};
#pragma once
#include "CCamObj.h"
class CPotion :
	public CCamObj
{
public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);

public:
	CPotion();
	virtual ~CPotion();
};
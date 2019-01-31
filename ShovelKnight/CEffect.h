#pragma once
#include "CObj.h"
class CAnimator;
class CEffect :
	public CObj
{
private:
	Vec2 m_vRealPos;
	POINT m_ptSize;
	int   m_iMaxSize;
	CAnimator* m_pAnim;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	CEffect();
	virtual ~CEffect();
};
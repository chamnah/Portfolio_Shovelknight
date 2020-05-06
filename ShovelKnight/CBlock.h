#pragma once
#include "CCamObj.h"
class CBlock :
	public CCamObj
{
private:
	CTexture* m_pTexEffect;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetEffectTexture(CTexture* _pEffect) { m_pTexEffect = _pEffect; }

public:
	CBlock();
	virtual ~CBlock();
};
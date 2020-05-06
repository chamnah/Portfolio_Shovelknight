#pragma once
#include "CObj.h"
CAnimator;
class CTempObj :
	public CObj
{
private:
	CAnimator* m_pAnim;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void AddAnim(CTexture* _pTex, const tSize& _tSize, int _iMax, int _iSpeed, const Vec2& _vOffset, const Vec2& _vTex);
	void Play(bool _bRepeat);

public:
	CTempObj();
	virtual ~CTempObj();
};
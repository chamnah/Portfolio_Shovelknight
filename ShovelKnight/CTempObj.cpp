#include "stdafx.h"
#include "CTempObj.h"
#include "CAnimator.h"

CTempObj::CTempObj()
{
	m_pAnim = new CAnimator(this);
}

CTempObj::~CTempObj()
{
	delete m_pAnim;
	m_pAnim = nullptr;
}

int CTempObj::update()
{
	m_pAnim->update();
	return 0;
}

void CTempObj::render(HDC _dc)
{
	m_pAnim->render(_dc);
}

void CTempObj::AddAnim(CTexture* _pTex,const tSize& _tSize,int _iMax,int _iSpeed,const Vec2& _vOffset,const Vec2& _vTex)
{
	m_pAnim->AddAnimation(L"Idle", _pTex, RECT{0,0,_tSize.x,_tSize.y}, _iMax, _iSpeed, _vOffset, _vTex);
}

void CTempObj::Play(bool _bRepeat)
{
	m_pAnim->PlayAnim(L"Idle",_bRepeat);
}
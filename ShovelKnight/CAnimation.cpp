#include "stdafx.h"
#include "CAnimation.h"
#include "CTimeMgr.h"

CAnimation::CAnimation(CTexture * _pTex, RECT _rect, int _iMaxCount, CObj* _pOwner,float _fDuration)
	:m_pOwner(nullptr)
	, m_iCurFrame(0)
	, m_bFinish(false)
{
	tAnimFrame af{};


	for (int i = 0; i < _iMaxCount; ++i)
	{
		af.rectSize = RECT{ _rect .left + (_rect.right * i),_rect.top,_rect.right,_rect.bottom};
		m_vecAnimFrame.push_back(af);
	}
	m_pOwner = _pOwner;
	m_pTex = _pTex;
	m_fDuration = _fDuration;
}

CAnimation::~CAnimation()
{
	m_vecAnimFrame.clear();
}

int CAnimation::Update()
{
	if (nullptr == m_pOwner)
		assert(NULL && L"Animation에 값이 적절히 추가 되지 않았다.");

	m_fAccTime += DT;
	
	if (m_fAccTime >= m_fDuration)
	{
		m_fAccTime -= m_fDuration;
		++m_iCurFrame;

		if (m_iCurFrame >= (int)m_vecAnimFrame.size()) // 현재 프레임이 전체 프레임을 넘어 서면
		{
			--m_iCurFrame;
			m_bFinish = true;
		}
	}
	
	return 0;
}

void CAnimation::Render(HDC _dc)
{
	/*
	// 이미지 반전하는 코드이다.
	XFORM xform;
	xform.eM11 = -1.f;
	xform.eM12 = 0.f;
	xform.eM21 = 0.f;
	xform.eM22 = 1.f;
	xform.eDx = 0.f;
	xform.eDy = 0.f;

	SetGraphicsMode(_dc, GM_ADVANCED);
	SetWorldTransform(_dc,&xform);
	TransparentBlt(_dc,-int(m_pOwner->GetPos().x + m_vecAnimFrame[m_iCurFrame].rectSize.right * 2),int(m_pOwner->GetPos().y - m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 2),
		m_vecAnimFrame[m_iCurFrame].rectSize.right * 4, m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 4,m_pTex->GetDC(), 
		m_vecAnimFrame[m_iCurFrame].rectSize.left, m_vecAnimFrame[m_iCurFrame].rectSize.top, m_vecAnimFrame[m_iCurFrame].rectSize.right, m_vecAnimFrame[m_iCurFrame].rectSize.bottom,RGB(0,255,0));

	ModifyWorldTransform(_dc, &xform, MWT_IDENTITY);
	SetGraphicsMode(_dc,GM_COMPATIBLE);*/

	TransparentBlt(_dc, int(m_pOwner->GetPos().x - m_vecAnimFrame[m_iCurFrame].rectSize.right * 2), int(m_pOwner->GetPos().y - m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 2),
		m_vecAnimFrame[m_iCurFrame].rectSize.right * 4, m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 4, m_pTex->GetDC(),
		m_vecAnimFrame[m_iCurFrame].rectSize.left, m_vecAnimFrame[m_iCurFrame].rectSize.top, m_vecAnimFrame[m_iCurFrame].rectSize.right, m_vecAnimFrame[m_iCurFrame].rectSize.bottom, RGB(0, 255, 0));
}

void CAnimation::SetFrameIdx(int _iFrameIdx)
{
	m_iCurFrame = _iFrameIdx;
	m_bFinish = false;
	m_fAccTime = 0.f;
}
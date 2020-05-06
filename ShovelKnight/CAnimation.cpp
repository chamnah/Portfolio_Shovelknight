#include "stdafx.h"
#include "CAnimation.h"
#include "CTimeMgr.h"

CAnimation::CAnimation(CTexture * _pTex, RECT _rect, int _iMaxCount, CObj* _pOwner,float _fDuration, Vec2 _vOffset)
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
	m_vOffset = _vOffset;
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
	TransparentBlt(_dc, int(m_pOwner->GetPos().x + m_vOffset.x - m_vecAnimFrame[m_iCurFrame].rectSize.right * 2), int(m_pOwner->GetPos().y + m_vOffset.y - m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 2),
		m_vecAnimFrame[m_iCurFrame].rectSize.right * 4, m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 4, m_pOwner->GetTexture()->GetDC(),
		m_vTexPos.x + m_vecAnimFrame[m_iCurFrame].rectSize.left, m_vecAnimFrame[m_iCurFrame].rectSize.top, m_vecAnimFrame[m_iCurFrame].rectSize.right - m_vTexPos.x, m_vecAnimFrame[m_iCurFrame].rectSize.bottom, RGB(0, 255, 0));
}

void CAnimation::AlphaRender(HDC _dc)
{
	BLENDFUNCTION BF = {};

	BF.BlendOp = AC_SRC_OVER;
	BF.SourceConstantAlpha = 128.f;
	BF.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(_dc, int(m_pOwner->GetPos().x + m_vOffset.x - m_vecAnimFrame[m_iCurFrame].rectSize.right * 2), int(m_pOwner->GetPos().y + m_vOffset.y - m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 2),
		m_vecAnimFrame[m_iCurFrame].rectSize.right * 4, m_vecAnimFrame[m_iCurFrame].rectSize.bottom * 4, m_pOwner->GetTexture()->GetDC(),
		m_vecAnimFrame[m_iCurFrame].rectSize.left, m_vecAnimFrame[m_iCurFrame].rectSize.top, m_vecAnimFrame[m_iCurFrame].rectSize.right, m_vecAnimFrame[m_iCurFrame].rectSize.bottom, BF);
}

void CAnimation::SetFrameIdx(int _iFrameIdx)
{
	m_iCurFrame = _iFrameIdx;
	m_bFinish = false;
	m_fAccTime = 0.f;
}
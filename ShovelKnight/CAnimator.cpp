#include "stdafx.h"
#include "CAnimator.h"
#include "CAnimation.h"

bool CAnimator::GetCurFinish()
{
	return m_pCurAnim->GetFinish();
}

int CAnimator::GetCurFrame()
{
	return m_pCurAnim->GetFrame();
}

CAnimator::CAnimator(CObj* _pOwner)
	:m_pOwner(_pOwner)
	,m_pCurAnim(nullptr)
	, m_bRepeat(false)
	, m_strCurKey(L"")
{
}

CAnimator::~CAnimator()
{
	SAFE_DELETE_MAP(m_mapAnim);
}

int CAnimator::update()
{
	if (nullptr == m_pCurAnim)
		assert(NULL && L"애니메이션 추가도 안하고 업데이트 했다.");

		m_pCurAnim->Update();
		if (m_pCurAnim->GetFinish() && m_bRepeat)
		{
			m_pCurAnim->SetFrameIdx(0);
		}
	return 0;
}

void CAnimator::render(HDC _dc, bool _bAlpha)
{
	if (nullptr != m_pCurAnim)
	{
		if(_bAlpha)
			m_pCurAnim->AlphaRender(_dc);
		else
			m_pCurAnim->Render(_dc);
	}
}

// 나는 현재 RECT에다가 실제 텍스쳐에 left,top 정보와 거기로 부터 사이즈 정보를 넣고 싶다 right , bottom은 크기 정보 실제 좌표가 아니다.
void CAnimator::AddAnimation(wstring _key, CTexture * _pTex, RECT _rect, int _iMaxCount,float _fDuration)
{
	map<wstring,CAnimation*>::iterator iter = m_mapAnim.find(_key);
	if (iter != m_mapAnim.end())
		assert(NULL && L"이미 Animaition이 추가 되어있습니다.");

	CAnimation* pAnim = new CAnimation(_pTex, _rect, _iMaxCount, m_pOwner,_fDuration);

	m_mapAnim.insert(make_pair(_key, pAnim));
}

void CAnimator::PlayAnim(wstring _key,bool _bRepeat)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_key);
	if (iter == m_mapAnim.end())
		assert(NULL && L"Animaition을 찾을 수 없습니다.");

	m_pCurAnim = iter->second;
	m_bRepeat = _bRepeat;
	m_pCurAnim->SetFinish(false);
	m_strCurKey = _key;
}

void CAnimator::ReStartAnim()
{
	m_pCurAnim->SetFrameIdx(0);
}

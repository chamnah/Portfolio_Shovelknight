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
		assert(NULL && L"�ִϸ��̼� �߰��� ���ϰ� ������Ʈ �ߴ�.");

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

// ���� ���� RECT���ٰ� ���� �ؽ��Ŀ� left,top ������ �ű�� ���� ������ ������ �ְ� �ʹ� right , bottom�� ũ�� ���� ���� ��ǥ�� �ƴϴ�.
void CAnimator::AddAnimation(wstring _key, CTexture * _pTex, RECT _rect, int _iMaxCount,float _fDuration)
{
	map<wstring,CAnimation*>::iterator iter = m_mapAnim.find(_key);
	if (iter != m_mapAnim.end())
		assert(NULL && L"�̹� Animaition�� �߰� �Ǿ��ֽ��ϴ�.");

	CAnimation* pAnim = new CAnimation(_pTex, _rect, _iMaxCount, m_pOwner,_fDuration);

	m_mapAnim.insert(make_pair(_key, pAnim));
}

void CAnimator::PlayAnim(wstring _key,bool _bRepeat)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnim.find(_key);
	if (iter == m_mapAnim.end())
		assert(NULL && L"Animaition�� ã�� �� �����ϴ�.");

	m_pCurAnim = iter->second;
	m_bRepeat = _bRepeat;
	m_pCurAnim->SetFinish(false);
	m_strCurKey = _key;
}

void CAnimator::ReStartAnim()
{
	m_pCurAnim->SetFrameIdx(0);
}

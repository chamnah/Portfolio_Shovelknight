#include "stdafx.h"
#include "CCamObj.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CKeyMgr.h"

CCamObj::CCamObj()
	:m_vRealPos(Vec2(0, 0))
	,m_vPrePos(Vec2(0, 0))
	,m_pColl(nullptr)
	, m_pAnim(nullptr)
	, m_tSize{}
	, m_bDeath(false)
{
}

CCamObj::~CCamObj()
{
	delete m_pColl;
	m_pColl = NULL;

	if (m_pAnim != nullptr)
		delete m_pAnim;

	m_pAnim = nullptr;
}

void CCamObj::InitAnim()
{
}

float CCamObj::Distance(const Vec2 & _vFirst, const Vec2 & _vSecond)
{
	return sqrtf(powf(_vFirst.x - _vSecond.x, 2.f) + powf(_vFirst.y - _vSecond.y, 2.f));
}

void CCamObj::Init()
{
	m_pColl = new CCollider;
	m_pColl->SetOwner(this);
	m_pColl->SetOffset(m_vOffset);
	m_pColl->SetScale(m_vScale);
}

int CCamObj::update()
{
	if(m_pColl != nullptr)
	m_pColl->Update();
	return 0;
}

void CCamObj::render(HDC _dc)
{
	if (m_pColl != nullptr)
		m_pColl->Render(_dc);
}
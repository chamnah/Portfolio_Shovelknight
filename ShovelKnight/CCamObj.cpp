#include "stdafx.h"
#include "CCamObj.h"
#include "CCollider.h"

CCamObj::CCamObj()
	:m_vRealPos(Vec2(0, 0))
	,m_vPrePos(Vec2(0, 0))
	,m_pColl(nullptr)
{
}

CCamObj::~CCamObj()
{
	delete m_pColl;
	m_pColl = NULL;
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
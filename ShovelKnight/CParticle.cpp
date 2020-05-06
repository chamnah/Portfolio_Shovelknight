#include "stdafx.h"
#include "CParticle.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"

CParticle::CParticle()
	:m_fJump(-400.f)
	,m_fGA(1800.f)
	,m_eDir(DIR::RIGHT)
	, m_fSpeed(200.f)
{
}

CParticle::~CParticle()
{
}

int CParticle::update()
{
	m_fJump += m_fGA * DT;
	m_vRealPos.y += m_fJump * DT;

	if (m_eDir == DIR::RIGHT)
		m_vRealPos.x += m_fSpeed * DT;
	else if(m_eDir == DIR::LEFT)
		m_vRealPos.x -= m_fSpeed * DT;

	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);

	if (m_vPos.y > CCore::GetInst()->GetResolution().y + 200)
		return INT_MAX;

	return 0;
}

void CParticle::render(HDC _dc)
{
	TransparentBlt(_dc, m_vPos.x - m_tSize.x * 2, m_vPos.y - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4, m_pTex->GetDC(), 0, 0, m_tSize.x, m_tSize.y, RGB(0, 255, 0));
}
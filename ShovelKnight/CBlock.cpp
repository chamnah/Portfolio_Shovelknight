#include "stdafx.h"
#include "CBlock.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CSkill.h"
#include "CEffect.h"
#include "CStageMgr.h"
#include "CCamMgr.h"
#include "CCore.h"

CBlock::CBlock()
	:m_pTexEffect(nullptr)
{
	m_eType = OBJ_TYPE::BLOCK;
}

CBlock::~CBlock()
{
}

int CBlock::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x,m_vRealPos.y);

	if (m_bDeath)
	{
		Vec2 vPos = Vec2(m_vPos.x + m_vScale.x / 2.f, (m_vPos.y + CCore::GetInst()->GetResolution().y) + m_vScale.y / 2.f);
		CEffect* pEffect = new CEffect;
		pEffect->SetPos(Vec2(m_vRealPos.x, m_vRealPos.y));
		pEffect->SetTexture(m_pTexEffect);
		pEffect->SetMaxSize(3);
		pEffect->Init();
		CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pEffect);

		return INT_MAX;
	}
	CCamObj::update();
	return 0;
}

void CBlock::render(HDC _dc)
{
	BitBlt(_dc,m_vPos.x - m_vScale.x / 2.f,m_vPos.y - m_vScale.y / 2.f,m_vScale.x,m_vScale.y,m_pTex->GetDC(),0,0,SRCCOPY);
	CCamObj::render(_dc);
}
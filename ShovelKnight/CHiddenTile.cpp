#include "stdafx.h"
#include "CHiddenTile.h"
#include "CCamMgr.h"
#include "CResMgr.h"
#include "CEffect.h"
#include "CStageMgr.h"
#include "CGameStage.h"
#include "CCollider.h"
#include "CSkill.h"

CHiddenTile::CHiddenTile()
	:m_iIdx(0)
	, m_bEffect(false)
	,m_iID(0)
{
	m_pTex = TEX_LOAD(L"Level1", L"Image\\Level1.bmp");
	m_vScale = Vec2(TILE_SIZE ,TILE_SIZE);
	m_vOffset = Vec2(32.f,32.f);
	m_eType = OBJ_TYPE::HIDDEN;
}

CHiddenTile::~CHiddenTile()
{
}

void CHiddenTile::Init()
{
	m_TileWidthNum = m_pTex->GetWidth() / TILE_SIZE;
	CCamObj::Init();
}

int CHiddenTile::update()
{
	if (m_bDeath)
	{
		if (!m_bEffect)
		{
			((CGameStage*)CStageMgr::GetInst()->GetCurStage())->GetFiniteObj()[m_iID].bDeath = true;
			CEffect* pEffect = new CEffect;
			pEffect->SetPos(m_vRealPos.x + 32, m_vRealPos.y + 32);
			pEffect->SetTexture(TEX_LOAD(L"TileEffect", L"Image\\TileEffect.bmp"));
			pEffect->SetMaxSize(7);
			pEffect->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pEffect);
			m_bEffect = true;
		}
	}
	else
	{
		m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
		CCamObj::update();
	}

	return 0;
}

void CHiddenTile::render(HDC _dc)
{
	int iCol = m_iIdx % m_TileWidthNum;
	int iLow = m_iIdx / m_TileWidthNum;

	if (!m_bDeath)
	{
		BitBlt(_dc, m_vPos.x + 1, m_vPos.y, m_vScale.x, m_vScale.y, m_pTex->GetDC(), iCol * TILE_SIZE, iLow * TILE_SIZE, SRCCOPY);
		CCamObj::render(_dc);
	}
}

DIR CHiddenTile::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	if (m_bDeath)
		return DIR::NONE;
	if (_other->GetOwner()->GetType() == OBJ_TYPE::SKILL && ((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		if (((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::R_BASIC)
			CStageMgr::GetInst()->SetDir(DIR::RIGHT);
		else if (((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::L_BASIC)
			CStageMgr::GetInst()->SetDir(DIR::LEFT);

		if (((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::R_BASIC || ((CSkill*)_other->GetOwner())->GetSkillType() == SKILL_TYPE::L_BASIC)
		{
			_other->GetOwner()->SetType(OBJ_TYPE::UI);
			CStageMgr::GetInst()->SetHidden(true);
			CStageMgr::GetInst()->SetHiddenType(m_eType);
		}
	}
	return DIR::NONE;
}
#include "stdafx.h"
#include "CMonster.h"
#include "CEffect.h"
#include "CStageMgr.h"

CMonster::CMonster()
	:m_fTime(0.f)
{
	m_eType = OBJ_TYPE::MONSTER;
}


CMonster::~CMonster()
{
}

void CMonster::CreateEffect(const Vec2 & _vPos, CTexture * _pTex, int _iSize)
{
	CEffect* pEffect = new CEffect;
	pEffect->SetPos(_vPos);
	pEffect->SetTexture(_pTex);
	pEffect->SetMaxSize(_iSize);
	pEffect->Init();
	CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::EFFECT].push_back(pEffect);
}
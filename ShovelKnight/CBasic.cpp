#include "stdafx.h"
#include "CBasic.h"
#include "CCollider.h"

CBasic::CBasic()
{
	m_vScale = Vec2(10,10);

	
}

CBasic::~CBasic()
{
}

void CBasic::Init()
{
	m_pColl = new CCollider;
	m_pColl->SetOwner(this);
	m_pColl->SetPos(m_vPos);
	m_pColl->SetScale(m_vScale);
}
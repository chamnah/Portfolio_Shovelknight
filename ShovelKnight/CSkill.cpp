#include "stdafx.h"
#include "CSkill.h"
#include "CCollider.h"

CSkill::CSkill()
	:m_pColl(nullptr)
{
	m_vPos = Vec2(200,200);
	m_vScale = Vec2(30,30);
}

CSkill::~CSkill()
{
	delete m_pColl;
	m_pColl = nullptr;
}

int CSkill::update()
{
	if(m_pColl != nullptr)
	m_pColl->Update();
	return 0;
}

void CSkill::render(HDC _dc)
{
	if (m_pColl != nullptr)
		m_pColl->Render(_dc);
}

void CSkill::OnCollisionEnter(CCollider * _other)
{

}

void CSkill::OnCollision(CCollider * _other)
{
}

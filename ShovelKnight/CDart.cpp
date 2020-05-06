#include "stdafx.h"
#include "CDart.h"
#include "CResMgr.h"
#include "CCamMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CStageMgr.h"
#include "CCore.h"

CDart::CDart()
	:m_fAngle(0.f)
	, m_fSpeed(0.f)
	, m_fDis(0.f)
	, m_fRotation(0.f)
	, m_fRotAngle(0.f)
	, m_bGo(false)
{
	m_pTex = TEX_LOAD(L"Dart", L"Image\\Dart.bmp");
	m_tSize = tSize(m_pTex->GetWidth(), m_pTex->GetHeight());
	m_hDC = CreateCompatibleDC(m_pTex->GetDC());
	m_hBit = CreateCompatibleBitmap(m_pTex->GetDC(), m_pTex->GetWidth(), m_pTex->GetHeight());
	HBITMAP OldBit = (HBITMAP)SelectObject(m_hDC,m_hBit);
	DeleteObject(OldBit);
	m_fDis = 200.f;
	m_fRotation = 500.f;
	m_fSpeed = 100.f;
	m_eType = OBJ_TYPE::SKILL;
	m_eSkillType = SKILL_TYPE::DART_ATTACK;
	m_vScale = Vec2(m_pTex->GetWidth() * 4,m_pTex->GetHeight() * 4);
}

CDart::~CDart()
{
}

int CDart::update()
{
	if (m_bDeath)
		return INT_MAX;

	m_fRotAngle += m_fRotation * DT;

	if (m_bGo)
	{
		m_fSpeed = 300.f;
		Go();
	}
	else
	{
		m_fAngle += m_fSpeed * DT;

		m_vPos.x = m_pOwner->GetPos().x + (cosf((m_fAngle * PI) / 180.f) * m_fDis);
		m_vPos.y = m_pOwner->GetPos().y + (sinf((m_fAngle * PI) / 180.f) * m_fDis);
	}

	if (m_vPos.x < -200 || m_vPos.y < -200 ||
		CCore::GetInst()->GetResolution().x * TILE_SIZE + 200 < m_vPos.x || CCore::GetInst()->GetResolution().y * TILE_SIZE + 200 < m_vPos.y)
		m_bDeath = true;
	CCamObj::update();
	return 0;
}

void CDart::render(HDC _dc)
{
	float sine = sinf((m_fRotAngle * PI) / 180.f);
	float cosine = cosf((m_fRotAngle * PI) / 180.f);

	SetGraphicsMode(_dc, GM_ADVANCED);

	XFORM xform; // �������� ǥ���ϴ� 3��3���� ��� ����
	xform.eM11 = cosine; // 1�� 1�� ���� ���� (ȸ������)
	xform.eM12 = sine; // 1�� 2�� ���� ���� (ȸ������)
	xform.eM21 = -sine; // 2�� 1�� ���� ���� (ȸ������)
	xform.eM22 = cosine; // 2�� 2�� ���� ���� (ȸ������)
	xform.eDx = m_vPos.x; // 3�� 1�� ���� ���� (X�� �̵� ����)
	xform.eDy = m_vPos.y; // 3�� 2�� ���� ���� (Y�� �̵� ����)
	
	SetWorldTransform(_dc, &xform);

	TransparentBlt(_dc,  - m_tSize.x * 2,  - m_tSize.y * 2, m_tSize.x * 4, m_tSize.y * 4, m_pTex->GetDC(), 0, 0, m_tSize.x, m_tSize.y, RGB(0, 255, 0));

	ModifyWorldTransform(_dc,&xform,MWT_IDENTITY);
	SetGraphicsMode(_dc, GM_COMPATIBLE);

	CCamObj::render(_dc);
}

DIR CDart::OnCollisionEnter(CCollider * _mine, CCollider * _other)
{
	CSkill::OnCollisionEnter(_mine,_other);

	return DIR::NONE;
}

void CDart::Go()
{
	m_vPos.x += (cosf((m_fAngle * PI) / 180.f) * m_fSpeed) * DT;
	m_vPos.y += (sinf((m_fAngle * PI) / 180.f) * m_fSpeed) * DT;
}
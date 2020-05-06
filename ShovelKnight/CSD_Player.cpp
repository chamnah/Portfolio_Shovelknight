#include "stdafx.h"
#include "CSD_Player.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

CSD_Player::CSD_Player()
	:m_bMove(false)
{
	m_tSize = tSize(21,19);
	m_pTex = TEX_LOAD(L"SD_Player",L"Image\\SD_Player.bmp");
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"SD_Player", m_pTex, RECT{0,0,m_tSize.x,m_tSize.y},2,0.4f);
	m_pAnim->PlayAnim(L"SD_Player",true);
	m_fSpeed = 200.f;
	m_vScale = Vec2(20,40);
	m_vOffset = Vec2(-10,0);
	m_eType = OBJ_TYPE::PLAYER;
}

CSD_Player::~CSD_Player()
{
}

int CSD_Player::update()
{
	if (m_bMove)
	{
		if (abs(m_vPos.y - m_vEndPos.y) > 1.f || abs(m_vPos.x - m_vEndPos.x) > 1.f)
		{
			float fDir = sqrtf(pow((m_vEndPos.x - m_vPos.x), 2) + powf((m_vEndPos.y - m_vPos.y), 2));
			float fDirX = (m_vEndPos.x - m_vPos.x) / fDir;
			float fDirY = (m_vEndPos.y - m_vPos.y) / fDir;
			m_vPos.x += (m_fSpeed * DT) * fDirX;
			m_vPos.y += (m_fSpeed * DT) * fDirY;
		}
		else
		{
			m_bMove = false;
		}
	}
	m_pAnim->update();
	CCamObj::update();
	return 0;
}

void CSD_Player::render(HDC _dc)
{
	m_pAnim->render(_dc);
	CCamObj::render(_dc);
}
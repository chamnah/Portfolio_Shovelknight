#include "stdafx.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CCamMgr.h"
#include "CCore.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CSkillMgr.h"

CPlayer::CPlayer()
	: m_iWSize(72),m_iHSize(43),m_fJump(2.8f)
{
	m_bJump = false;
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Player", L"Image\\Player.bmp");
	m_fSpeed = 100.f;
	m_eAnim = ANIM_STATE::L_STOP;

	m_vPos = Vec2(500, 500);

	m_vScale = Vec2(50.f,100.f);
	m_vOffset = Vec2(-5, 20);
	m_eType = OBJ_TYPE::PLAYER;

	m_pAnim->AddAnimation(L"R_Idle", m_pTex, RECT{0, m_iHSize * 0 ,m_iWSize,m_iHSize}, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Idle", m_pTex, RECT{ 0, m_iHSize * 1 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Walk", m_pTex, RECT{ 0, m_iHSize * 2 ,m_iWSize,m_iHSize }, 6, 0.1f);
	m_pAnim->AddAnimation(L"L_Walk", m_pTex, RECT{ 0, m_iHSize * 3 ,m_iWSize,m_iHSize }, 6, 0.1f);
	m_pAnim->AddAnimation(L"R_Attack", m_pTex, RECT{ 0, m_iHSize * 4 ,m_iWSize,m_iHSize }, 5, 0.1f);
	m_pAnim->AddAnimation(L"L_Attack", m_pTex, RECT{ 0, m_iHSize * 5 ,m_iWSize,m_iHSize }, 5, 0.1f);
	m_pAnim->AddAnimation(L"R_JumpUp", m_pTex, RECT{ 0, m_iHSize * 6 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpUp", m_pTex, RECT{ 0, m_iHSize * 7 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_JumpDown", m_pTex, RECT{ 0, m_iHSize * 8 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpDown", m_pTex, RECT{ 0, m_iHSize * 9 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_JumpAttack", m_pTex, RECT{ 0, m_iHSize * 10 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpAttack", m_pTex, RECT{ 0, m_iHSize * 11 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Death", m_pTex, RECT{ 0, m_iHSize * 12 ,m_iWSize,m_iHSize }, 3, 0.2f);
	m_pAnim->AddAnimation(L"L_Death", m_pTex, RECT{ 0, m_iHSize * 13 ,m_iWSize,m_iHSize }, 3, 0.2f);
	
	m_pAnim->PlayAnim(L"R_Idle",true);
}	
	
CPlayer::~CPlayer()
{
	delete m_pAnim;
	m_pAnim = NULL;
}	
	
int CPlayer::update()
{	
	m_pAnim->update();
	// 카메라가 왼쪽 끝 지점에 도착한다면
	if (CCamMgr::GetInst()->GetLook().x <= CCore::GetInst()->GetResolution().x / 2)
		m_fSpeed = 200.f;
	else
		m_fSpeed = 100.f;

	
	m_vPrePos = m_vPos;

	if (m_bAttack)
	{
		if (m_pAnim->GetCurFinish())
		{
			if(m_pAnim->GetCurKey() == L"L_Attack")
				m_pAnim->PlayAnim(L"L_Idle", false);
			else if (m_pAnim->GetCurKey() == L"R_Attack")
				m_pAnim->PlayAnim(L"R_Idle", false);
			m_bAttack = false;
		}
	}

	if (!m_bAttack)
	{
		if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD))
			m_vPos.x -= m_fSpeed * DT;
		else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
			m_vPos.x += m_fSpeed * DT;

		if (!m_bJump)
		{
			MoveAnimation();

			if (m_pAnim->GetCurKey() == L"L_JumpUp")
				m_pAnim->PlayAnim(L"L_Idle", false);
			else if (m_pAnim->GetCurKey() == L"R_JumpUp")
				m_pAnim->PlayAnim(L"R_Idle", false);
			else if (m_pAnim->GetCurKey() == L"L_JumpDown")
				m_pAnim->PlayAnim(L"L_Idle", false);
			else if (m_pAnim->GetCurKey() == L"R_JumpDown")
				m_pAnim->PlayAnim(L"R_Idle", false);

			if (m_fAccTemp > m_fAcc) // 걷다가 떨어질때
			{
				if (m_pAnim->GetCurKey() == L"R_Idle" || m_pAnim->GetCurKey() == L"R_Walk")
					m_pAnim->PlayAnim(L"R_JumpDown", false);
				else if (m_pAnim->GetCurKey() == L"L_Idle" || m_pAnim->GetCurKey() == L"L_Walk")
					m_pAnim->PlayAnim(L"L_JumpDown", false);
			}

			if (KEY_MGR(KEY_TYPE::KEY_C, KEY_STATE::TAB)) // 점프
			{
				m_bJump = true;
				if (m_pAnim->GetCurKey() == L"R_Idle" || m_pAnim->GetCurKey() == L"R_Walk")
					m_pAnim->PlayAnim(L"R_JumpUp", false);
				else if (m_pAnim->GetCurKey() == L"L_Idle" || m_pAnim->GetCurKey() == L"L_Walk")
					m_pAnim->PlayAnim(L"L_JumpUp", false);
			}
			else
			{
				m_vPos.y += m_fAccTemp; // 중력
			}
		}

		if (m_bJump)
		{
			JumpUpAnimation();

			if ((m_fJump - m_fAccTemp) <= 0)
			{
				if (m_pAnim->GetCurKey() == L"R_JumpUp")
					m_pAnim->PlayAnim(L"R_JumpDown", false);
				else if (m_pAnim->GetCurKey() == L"L_JumpUp")
					m_pAnim->PlayAnim(L"L_JumpDown", false);
			}

			m_vPos.y = m_vPos.y - (m_fJump - m_fAccTemp);
		}
	}

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_X, KEY_STATE::TAB))
	{
		m_bAttack = true;
		
		if (m_pAnim->GetCurKey() == L"R_Idle" || m_pAnim->GetCurKey() == L"R_Walk" ||
			m_pAnim->GetCurKey() == L"R_JumpUp" || m_pAnim->GetCurKey() == L"R_JumpDown")
			m_pAnim->PlayAnim(L"R_Attack", false);
		else if (m_pAnim->GetCurKey() == L"L_Idle" || m_pAnim->GetCurKey() == L"L_Walk" ||
			m_pAnim->GetCurKey() == L"L_JumpUp" || m_pAnim->GetCurKey() == L"L_JumpDown")
			m_pAnim->PlayAnim(L"L_Attack", false);

		m_pAnim->ReStartAnim();
	}

	m_fAccTemp += m_fAccTemp * DT;
	
	CCamObj::update(); // Collidsion 업데이트이다.

	return 0;
}

void CPlayer::render(HDC _dc)
{
	m_pAnim->render(_dc);
	//TransparentBlt(_dc, (int)m_vPos.x - m_iWSize * 2, (int)m_vPos.y - m_iHSize * 2, m_iWSize * 4, m_iHSize * 4, m_pTex->GetDC(), (int)m_fAnim * m_iWSize, m_iHSize * (UINT)m_eAnim, m_iWSize, m_iHSize, RGB(0, 255, 0));
	CCamObj::render(_dc);
}

int CPlayer::lateupdate()
{
	return 0;
}

void CPlayer::MoveAnimation()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD))
		m_pAnim->PlayAnim(L"L_Walk", true);
	
	else if(CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
		m_pAnim->PlayAnim(L"R_Walk", true);
	
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::AWAY))
		m_pAnim->PlayAnim(L"L_Idle", false);
	
	else if(CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::AWAY))
		m_pAnim->PlayAnim(L"R_Idle", false);


}

void CPlayer::JumpUpAnimation()
{
	if (KEY_MGR(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD) || KEY_MGR(KEY_TYPE::KEY_LEFT, KEY_STATE::AWAY))
		m_pAnim->PlayAnim(L"L_JumpUp", false);

	else if (KEY_MGR(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD) || KEY_MGR(KEY_TYPE::KEY_RIGHT, KEY_STATE::AWAY))
		m_pAnim->PlayAnim(L"R_JumpUp", false);
}

void CPlayer::JumpDownAnimation()
{
}

void CPlayer::OnCollisionEnter(CCollider * _other)
{
	CDynamicObj::OnCollisionEnter(_other);
}

void CPlayer::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollision(_other);
}

void CPlayer::OnCollisionExit(CCollider * _other)
{
}
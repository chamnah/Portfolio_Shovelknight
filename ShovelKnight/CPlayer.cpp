#include "stdafx.h"
#include "CPlayer.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CCamMgr.h"
#include "CCore.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CStageMgr.h"
#include "CBasic.h"
#include "CUI.h"


CPlayer::CPlayer()
	: m_pObj(nullptr)
	, m_bJumpAttack(false)
	, m_bAlpha(false)
	, m_bDeath(false)
	, m_iJumpFlag(0)
{
	m_iMaxHP = 6;
	m_iHP = m_iMaxHP;
	m_iWSize = 72;
	m_iHSize = 43;
	m_bJump = false;
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Player", L"Image\\Player_1.bmp");
	m_fSpeed = 100.f;

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
	m_pAnim->AddAnimation(L"R_Damage", m_pTex, RECT{ 0, m_iHSize * 14 ,m_iWSize,m_iHSize }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Damage", m_pTex, RECT{ 0, m_iHSize * 15 ,m_iWSize,m_iHSize }, 1, 0.2f);
	
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

	/*if (m_iHP <= 0)
	{
		if(m_iDir == RIGHT)
			m_pAnim->PlayAnim(L"R_Death", true);
		else if(m_iDir == LEFT)
			m_pAnim->PlayAnim(L"L_Death", true);

		return 0;
	}*/
	// 카메라가 왼쪽 끝 지점에 도착한다면
	if ((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) <= CCore::GetInst()->GetResolution().x)
		m_fSpeed = 200.f;
	else
	{
		if (m_vPos.x <= CCore::GetInst()->GetResolution().x / 2)
			m_fSpeed = 200.f;
		else
			m_fSpeed = 100.f;
	}
	m_vPrePos = m_vPos;

	if (!m_bDamage)
	{
		if (m_bAttack)
		{
			if (m_pObj == nullptr && m_pAnim->GetCurFrame() == 3)
			{
				m_pObj = new CBasic;
				((CBasic*)m_pObj)->SetOwner(this);
				m_pObj->Init();
				if (m_pAnim->GetCurKey() == L"R_Attack")
					m_pObj->SetOffset(Vec2(110, 30));
				else
					m_pObj->SetOffset(Vec2(-110, 30));
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(m_pObj);
			}
			else if (m_pObj != nullptr && m_pAnim->GetCurFrame() == 4)
			{
				CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
				m_pObj = nullptr;
			}
			if (m_pAnim->GetCurFinish())
			{
				if (m_iDir == LEFT)
					m_pAnim->PlayAnim(L"L_Idle", false);
				else if (m_iDir == RIGHT)
					m_pAnim->PlayAnim(L"R_Idle", false);
				m_bAttack = false;
			}
		}

		if (!m_bAttack)
		{
			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD))
			{
				m_iDir = LEFT;
				m_vPos.x -= m_fSpeed * DT;
			}
			else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
			{
				m_iDir = RIGHT;
				m_vPos.x += m_fSpeed * DT;
			}
			if (!m_bJump)
			{
				if (m_bJumpAttack)
				{
					if (m_pObj != nullptr)
					{
						CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
						m_pObj = nullptr;

					}
					m_bJumpAttack = false;
				}
				MoveAnimation();

				if (m_pAnim->GetCurKey() == L"L_JumpUp")
					m_pAnim->PlayAnim(L"L_Idle", false);
				else if (m_pAnim->GetCurKey() == L"R_JumpUp")
					m_pAnim->PlayAnim(L"R_Idle", false);
				else if (m_pAnim->GetCurKey() == L"L_JumpDown")
					m_pAnim->PlayAnim(L"L_Idle", false);
				else if (m_pAnim->GetCurKey() == L"R_JumpDown")
					m_pAnim->PlayAnim(L"R_Idle", false);
				else if (m_pAnim->GetCurKey() == L"L_JumpAttack")
					m_pAnim->PlayAnim(L"L_Idle", false);
				else if (m_pAnim->GetCurKey() == L"R_JumpAttack")
					m_pAnim->PlayAnim(L"R_Idle", false);

				if (m_fJump > 3) // 걷다가 떨어질때
				{
					m_bJump = true;
					if (m_iDir == RIGHT)
						m_pAnim->PlayAnim(L"R_JumpDown", false);
					else if (m_iDir == LEFT)
						m_pAnim->PlayAnim(L"L_JumpDown", false);
				}

				if (KEY_MGR(KEY_TYPE::KEY_C, KEY_STATE::TAB)) // 점프
				{
					m_fAccJump += DT; // 프레임 단축을 위해서
					m_bJump = true;
					if (m_iDir == RIGHT)
						m_pAnim->PlayAnim(L"R_JumpUp", false);
					else if (m_iDir == LEFT)
						m_pAnim->PlayAnim(L"L_JumpUp", false);
					m_fJump = -500.f;
					m_iJumpFlag = 0x00;
					m_fAccJump = 0;
				}
			}

			if (m_bJump)
			{
				if (!(m_iJumpFlag & JUMP_END) && (m_pAnim->GetCurKey() == L"R_JumpUp" || m_pAnim->GetCurKey() == L"L_JumpUp") && KEY_MGR(KEY_TYPE::KEY_C, KEY_STATE::HOLD))
				{
					m_fAccJump += DT;
					if (!(m_iJumpFlag & JUMP_ONE) && m_fAccJump > 0.2f)
					{
						m_iJumpFlag |= JUMP_ONE;
						m_fJump = m_fJump -100.f;
					}
					else if (m_fAccJump > 0.4f)
					{
						m_iJumpFlag |= JUMP_END;
						m_fJump = m_fJump - 100.f;
					}
				}

				m_pColl->SetOffset(m_vOffset.x, 10.f);
				if (m_pObj == nullptr && KEY_MGR(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB) || KEY_MGR(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD))
				{
					m_bJumpAttack = true;
					if (m_pObj == nullptr)
					{
						m_pObj = new CBasic;
						((CBasic*)m_pObj)->SetOwner(this);
						m_pObj->Init();
						m_pObj->SetOffset(Vec2(-5, 60));
						((CBasic*)m_pObj)->SetSkillType(SKILL_TYPE::JUMP_ATTACK);
						CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(m_pObj);
					}
				}
				if (m_fJump >= 0)
				{
					if (m_iDir == RIGHT)
						m_pAnim->PlayAnim(L"R_JumpDown", false);
					else if (m_iDir == LEFT)
						m_pAnim->PlayAnim(L"L_JumpDown", false);
				}
				else
				{
					JumpUpAnimation();
				}

				if (m_bJumpAttack)
				{
					if (m_iDir == RIGHT)
						m_pAnim->PlayAnim(L"R_JumpAttack", false);
					else if (m_iDir == LEFT)
						m_pAnim->PlayAnim(L"L_JumpAttack", false);
				}
			}
			else
			{
				m_pColl->SetOffset(m_vOffset);
			}
		}

		if (!m_bAttack && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_X, KEY_STATE::TAB))
		{
			m_bJumpAttack = false;
			m_bAttack = true;

			if (m_pObj != nullptr)
			{
				CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
				m_pObj = nullptr;
			}

			if (m_iDir == RIGHT)
				m_pAnim->PlayAnim(L"R_Attack", false);
			else if (m_iDir == LEFT)
				m_pAnim->PlayAnim(L"L_Attack", false);

			m_pAnim->ReStartAnim();
		}
	}
	else if (m_bDamage)
	{
		if(!m_bJump)
		{ 
			m_bJump = true;
			m_fJump = -80.f;
		}
		if (m_iDir == LEFT)
			m_vPos.x += m_fSpeed * DT;
		else if (m_iDir == RIGHT)
			m_vPos.x -= m_fSpeed * DT;

		m_fAccTime += DT;
		if (m_fAccTime > 0.5f)
		{
			if (m_iDir == LEFT)
				m_pAnim->PlayAnim(L"L_Idle", false);
			else if (m_iDir == RIGHT)
				m_pAnim->PlayAnim(L"R_Idle", false);
			m_fAccTime = 0;
			m_bDamage = false;
			m_bAlpha = true;
		}
	}

	if (m_bAlpha)
	{
		m_fAccTime += DT;
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0;
			m_bAlpha = false;
		}
	}

	Gravity(m_vPos);
	
	CCamObj::update(); // Collidsion 업데이트이다.

	return 0;
}

void CPlayer::render(HDC _dc)
{
	m_pAnim->render(_dc, m_bAlpha);
	CCamObj::render(_dc);
}

int CPlayer::lateupdate()
{
	return 0;
}

void CPlayer::MoveAnimation()
{
	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD))
	{
		m_iDir = LEFT;
		m_pAnim->PlayAnim(L"L_Walk", true);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
	{
		m_iDir = RIGHT;
		m_pAnim->PlayAnim(L"R_Walk", true);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::AWAY))
	{
		m_iDir = LEFT;
		m_pAnim->PlayAnim(L"L_Idle", false);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::AWAY))
	{
		m_iDir = RIGHT;
		m_pAnim->PlayAnim(L"R_Idle", false);
	}

}

void CPlayer::JumpUpAnimation()
{
	if (KEY_MGR(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD) || KEY_MGR(KEY_TYPE::KEY_LEFT, KEY_STATE::AWAY))
	{
		m_iDir = LEFT;
		m_pAnim->PlayAnim(L"L_JumpUp", false);
	}
	else if (KEY_MGR(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD) || KEY_MGR(KEY_TYPE::KEY_RIGHT, KEY_STATE::AWAY))
	{
		m_iDir = RIGHT;
		m_pAnim->PlayAnim(L"R_JumpUp", false);
	}
}

void CPlayer::JumpDownAnimation()
{
}

int CPlayer::OnCollisionEnter(CCollider * _other)
{
	CDynamicObj::OnCollisionEnter(_other);
	if (!m_bAlpha && !m_bDamage && _other->GetOwner()->GetType() == OBJ_TYPE::MONSTER)
	{
		if (m_pObj != nullptr)
		{
			CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
			m_pObj = nullptr;
		}
		m_bDamage = true;
		m_bJump = true; 
		m_iHP -= 1;
		
		((CUI*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI][0])->SetDamage(1);


		if (m_iDir == LEFT)
			m_pAnim->PlayAnim(L"L_Damage", false);
		else if (m_iDir == RIGHT)
			m_pAnim->PlayAnim(L"R_Damage", false);
		
		m_fJump = -200.f;
	}

	return 0;
}

void CPlayer::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollision(_other);

	if (!m_bAlpha && !m_bDamage && _other->GetOwner()->GetType() == OBJ_TYPE::MONSTER)
	{
		m_bDamage = true;
		if (m_pObj != nullptr)
		{
			CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
			m_pObj = nullptr;
		}

		m_iHP -= 1;
		if (m_iDir == LEFT)
			m_pAnim->PlayAnim(L"L_Damage", false);
		else if (m_iDir == RIGHT)
			m_pAnim->PlayAnim(L"R_Damage", false);

		m_bJump = true;
		m_fJump = -200.f;
	}
}

void CPlayer::OnCollisionExit(CCollider * _other)
{
}
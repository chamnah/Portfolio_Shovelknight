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
#include "CGameMgr.h"
#include "CTile.h"
#include "CDCMgr.h"
#include "CDart.h"
#include "SoundMgr.h"
#include "CLightning.h"

CPlayer::CPlayer()
	: m_pObj(nullptr)
	, m_bJumpAttack(false)
	, m_bAlpha(false)
	, m_iJumpFlag(0)
	, m_iLadderCheck(0)
	, m_fLadderTime(0.f)
	, m_fLadderDown(0.f)
	, m_bKing(false)
	, m_bEvent(false)
	, m_bSkill(false)
	, m_fSkillTime(0.f)
	, m_bSpeedMoving(false)
	, m_fShiningTime(0.f)
	, m_bShining(false)
	, m_pTexOri(nullptr)
	, m_bEnd(false)
	, m_bLadderExit(false)
	, m_bStop(false)
{
	m_fGA = 2400.f;
	m_eDir = DIR::RIGHT;
	m_iMaxHP = 6;
	m_iHP = m_iMaxHP;
	m_tSize = tSize(72, 43);
	m_bJump = false;
	if (CGameMgr::GetInst()->GetEquipment() != EQUIP_TYPE::END)
		m_pTexOri = CDCMgr::GetInst()->GetPlayerTex((int)CGameMgr::GetInst()->GetEquipment());
	else
		m_pTexOri = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Player", L"Image\\Player_1.bmp");
	

	m_pTex = m_pTexOri;

	m_fSpeed = 300.f;
	m_fSaveSpeed = 300.f;

	m_vPos = Vec2(500, 500);

	m_vScale = Vec2(50.f,100.f);
	m_vOffset = Vec2(-5, 20);
	m_eType = OBJ_TYPE::PLAYER;

	m_pAnim->AddAnimation(L"R_Idle", m_pTex, RECT{0, m_tSize.y * 0 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Idle", m_pTex, RECT{ 0, m_tSize.y * 1 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Walk", m_pTex, RECT{ 0, m_tSize.y * 2 ,m_tSize.x,m_tSize.y }, 6, 0.1f);
	m_pAnim->AddAnimation(L"L_Walk", m_pTex, RECT{ 0, m_tSize.y * 3 ,m_tSize.x,m_tSize.y }, 6, 0.1f);
	m_pAnim->AddAnimation(L"R_Attack", m_pTex, RECT{ 0, m_tSize.y * 4 ,m_tSize.x,m_tSize.y }, 5, 0.08f);
	m_pAnim->AddAnimation(L"L_Attack", m_pTex, RECT{ 0, m_tSize.y * 5 ,m_tSize.x,m_tSize.y }, 5, 0.08f);
	m_pAnim->AddAnimation(L"R_JumpUp", m_pTex, RECT{ 0, m_tSize.y * 6 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpUp", m_pTex, RECT{ 0, m_tSize.y * 7 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_JumpDown", m_pTex, RECT{ 0, m_tSize.y * 8 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpDown", m_pTex, RECT{ 0, m_tSize.y * 9 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_JumpAttack", m_pTex, RECT{ 0, m_tSize.y * 10 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpAttack", m_pTex, RECT{ 0, m_tSize.y * 11 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Death", m_pTex, RECT{ 0, m_tSize.y * 12 ,m_tSize.x,m_tSize.y }, 3, 0.2f);
	m_pAnim->AddAnimation(L"L_Death", m_pTex, RECT{ 0, m_tSize.y * 13 ,m_tSize.x,m_tSize.y }, 3, 0.2f);
	m_pAnim->AddAnimation(L"R_Damage", m_pTex, RECT{ 0, m_tSize.y * 14 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Damage", m_pTex, RECT{ 0, m_tSize.y * 15 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Wand", m_pTex, RECT{ 0, m_tSize.y * 16 ,m_tSize.x,m_tSize.y }, 2, 0.2f);
	m_pAnim->AddAnimation(L"L_Wand", m_pTex, RECT{ 0, m_tSize.y * 17 ,m_tSize.x,m_tSize.y }, 2, 0.2f);
	m_pAnim->AddAnimation(L"R_Ladder", m_pTex, RECT{ 0, m_tSize.y * 18 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Ladder", m_pTex, RECT{ 0, m_tSize.y * 19 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"LadderEnd", m_pTex, RECT{ 0, m_tSize.y * 20 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"RU_Walk", m_pTex, RECT{ 0, m_tSize.y * 21 ,m_tSize.x,m_tSize.y }, 6, 0.2f);
	m_pAnim->AddAnimation(L"LU_Walk", m_pTex, RECT{ 0, m_tSize.y * 22 ,m_tSize.x,m_tSize.y }, 6, 0.2f);
	m_pAnim->AddAnimation(L"RU_JumpUp", m_pTex, RECT{ 0, m_tSize.y * 23 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"LU_JumpUp", m_pTex, RECT{ 0, m_tSize.y * 24 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"RU_JumpDown", m_pTex, RECT{ 0, m_tSize.y * 25 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"LU_JumpDown", m_pTex, RECT{ 0, m_tSize.y * 26 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_JumpShield", m_pTex, RECT{ 0, m_tSize.y * 27 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpShield", m_pTex, RECT{ 0, m_tSize.y * 28 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_JumpDownShield", m_pTex, RECT{ 0, m_tSize.y * 29 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_JumpDownShield", m_pTex, RECT{ 0, m_tSize.y * 30 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Shield", m_pTex, RECT{ 0, m_tSize.y * 31 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Shield", m_pTex, RECT{ 0, m_tSize.y * 32 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Smash", m_pTex, RECT{ 0, m_tSize.y * 33 ,86,m_tSize.y }, 2, 0.1f,Vec2(-4,0));
	m_pAnim->AddAnimation(L"L_Smash", m_pTex, RECT{ 0, m_tSize.y * 34 ,86,m_tSize.y }, 2, 0.1f, Vec2(-11, 0));
	m_pAnim->AddAnimation(L"UP", m_pTex, RECT{ 0, m_tSize.y * 35 ,m_tSize.x,m_tSize.y }, 1, 0.2f);
	
	m_pAnim->PlayAnim(L"R_Idle",true);
}
	
CPlayer::~CPlayer()
{
	m_bSpeedMoving = false;
	m_fSkillTime = 0.f;
	m_bSkill = false;
}	
	
void CPlayer::Init()
{
	m_vRealPos = m_vPos;
	CCamObj::Init();
}

int CPlayer::update()
{		
	if (m_bSkill)
	{
		m_fSaveSpeed = 500.f;
		CCamMgr::GetInst()->SetSpeed(m_fSaveSpeed);
		m_listFrame.push_back(tFrame(CCamMgr::GetInst()->GetCamMousePos(m_vPos), m_pAnim->GetCurFrame(), m_pAnim->GetCurKey()));
		m_fSkillTime += DT;

		if (m_fSkillTime > 5.f)
		{
			m_fSkillTime = 0.f;
			m_bSpeedMoving = false;
			m_bSkill = false;
			m_listFrame.clear();
			m_fSaveSpeed = 300.f;
			CCamMgr::GetInst()->SetSpeed(m_fSaveSpeed);
		}
		else if (m_fSkillTime > 0.1f)
		{
			m_bSpeedMoving = true;
		}
	}

	if (CGameMgr::GetInst()->GetPlayerStop())
	{
		if (m_bEnd)
		{
			Gravity(m_vRealPos);
			m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
			CCamObj::update();
			Ending();
			return 0;
		}

		if (m_bKing)
		{
			CCamObj::update();
			m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
		}
		return 0;
	}
	
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_bKing)
	{
		if (m_bEvent)
		{
			m_pAnim->update();
			CCamObj::update();
			Event();
			return 0;
		}
	}

	if (CTimeMgr::GetInst()->GetStop())
		return 0;

	m_pAnim->update();

	if (m_iHP <= 0)
	{
		if (m_pAnim->GetCurFinish() == true)
			CGameMgr::GetInst()->SetDeath(true);
		
		if(m_eDir == DIR::RIGHT)
			m_pAnim->PlayAnim(L"R_Death", false);
		else if(m_eDir == DIR::LEFT)
			m_pAnim->PlayAnim(L"L_Death", false);

		return 0;
	}

	if ((CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) <= CCore::GetInst()->GetResolution().x)
		m_fSpeed = m_fSaveSpeed;
	else
	{
		if (CCamMgr::GetInst()->IsMove())
			m_fSpeed = m_fSaveSpeed;
		else
			m_fSpeed = m_fSaveSpeed;
	}
	m_vPrePos = m_vPos;

	if (!m_bDamage)
	{
		if (m_bAttack)
		{
			if (m_pObj == nullptr && m_bShining)
			{
				
				m_pObj = new CBasic;
				((CBasic*)m_pObj)->SetOwner(this);
				
				if (m_pAnim->GetCurKey() == L"R_Smash")
				{
					m_pObj->SetScale(Vec2(300,120));
					m_pObj->SetOffset(Vec2(10, 10));
					((CBasic*)m_pObj)->SetSkillType(SKILL_TYPE::SMASH);
				}
				else
				{
					m_pObj->SetScale(Vec2(300, 120));
					m_pObj->SetOffset(Vec2(-30, 10));
					((CBasic*)m_pObj)->SetSkillType(SKILL_TYPE::SMASH);
				}
				m_pObj->Init();
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(m_pObj);
			}
			else if (m_pObj == nullptr && m_pAnim->GetCurFrame() == 3)
			{
				m_pObj = new CBasic;
				((CBasic*)m_pObj)->SetOwner(this);
				m_pObj->Init();
				if (m_pAnim->GetCurKey() == L"R_Attack")
				{
					m_pObj->SetOffset(Vec2(90, 30));
					((CBasic*)m_pObj)->SetSkillType(SKILL_TYPE::R_BASIC);
				}
				else
				{
					m_pObj->SetOffset(Vec2(-110, 30));
					((CBasic*)m_pObj)->SetSkillType(SKILL_TYPE::L_BASIC);
				}
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(m_pObj);
			}
			if (m_pAnim->GetCurFinish())
			{
				m_bShining = false;
				if (m_pObj != nullptr)
				CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
				m_pObj = nullptr;
				if (m_eDir == DIR::LEFT)
					m_pAnim->PlayAnim(L"L_Idle", false);
				else if (m_eDir == DIR::RIGHT)
					m_pAnim->PlayAnim(L"R_Idle", false);
				m_bAttack = false;
			}
		}

		if (!m_bAttack)
		{
			Skill();

			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD) && !(m_iLadderCheck & LadderRiding))
			{
				m_eDir = DIR::LEFT;
				m_vRealPos.x -= m_fSpeed * DT;
			}
			else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD) && !(m_iLadderCheck & LadderRiding))
			{
				m_eDir = DIR::RIGHT;
				m_vRealPos.x += m_fSpeed * DT;
			}
			if (!m_bJump)
			{
				if (m_bJumpAttack)
				{
					if (m_pObj != nullptr)
					{
						m_pColl->SetOffset(m_vOffset);
						m_pColl->SetScale(m_vScale);
						CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
						m_pObj = nullptr;

					}
					m_bJumpAttack = false;
				}
				if ((m_iLadderCheck & LadderRiding))
					LadderAnimation();
				else
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

				if (m_fJump > 20) // 걷다가 떨어질때
				{
					m_bJump = true;
					if (m_eDir == DIR::RIGHT)
						m_pAnim->PlayAnim(L"R_JumpDown", false);
					else if (m_eDir == DIR::LEFT)
						m_pAnim->PlayAnim(L"L_JumpDown", false);
				}

				if (KEY_MGR(KEY_TYPE::KEY_C, KEY_STATE::TAB)) // 점프
				{
					m_bJump = true;
					if (m_eDir == DIR::RIGHT)
						m_pAnim->PlayAnim(L"R_JumpUp", false);
					else if (m_eDir == DIR::LEFT)
						m_pAnim->PlayAnim(L"L_JumpUp", false);
					if (!(m_iLadderCheck & LadderRiding))
					{
						CSoundMgr::GetInst()->Play(L"Jump",false);
						m_fJump = -880.f;
					}
					m_iJumpFlag = 0x00;
					m_fAccJump = 0;
					m_iLadderCheck = 0;
				}
			}

			if (m_bJump)
			{
				if (!(m_iJumpFlag & JUMP_END) && (m_pAnim->GetCurKey() == L"R_JumpUp" || m_pAnim->GetCurKey() == L"L_JumpUp") && KEY_MGR(KEY_TYPE::KEY_C, KEY_STATE::HOLD))
				{
					m_fAccJump += DT;
					if (!(m_iJumpFlag & JUMP_ONE) && m_fAccJump > 0.1f)
					{
						m_iJumpFlag |= JUMP_ONE;
						m_fJump = m_fJump -240.f;
					}
					else if (!(m_iJumpFlag & JUMP_TWO) && m_fAccJump > 0.2f)
					{
						m_iJumpFlag |= JUMP_TWO;
						m_fJump = m_fJump - 180.f;
					}
					else if (!(m_iJumpFlag & JUMP_THREE) && m_fAccJump > 0.3f)
					{
						m_iJumpFlag |= JUMP_THREE;
						m_fJump = m_fJump - 60.f;
					}
					else if (!(m_iJumpFlag & JUMP_FOUR) && m_fAccJump > 0.4f)
					{
						m_iJumpFlag |= JUMP_FOUR;
						m_fJump = m_fJump - 40.f;
					}
					else if(m_fAccJump > 0.5f)
					{
						m_iJumpFlag |= JUMP_END;
						m_fJump = m_fJump - 30.f;
					}
				}

				if (KEY_MGR(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
					m_bLadderExit = false;

				if (!m_bLadderExit)
				{
					if (m_pObj == nullptr && KEY_MGR(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB) || KEY_MGR(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD))
					{
						m_bJumpAttack = true;
						if (m_pObj == nullptr)
						{
							m_pObj = new CBasic;
							((CBasic*)m_pObj)->SetOwner(this);
							m_pObj->SetOffset(Vec2(-5, 55));
							m_pObj->SetScale(Vec2(45,30));
							m_pObj->Init();
							((CBasic*)m_pObj)->SetSkillType(SKILL_TYPE::JUMP_ATTACK);
							CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(m_pObj);

							m_pColl->SetScale(Vec2(m_vScale.x - 10, 90.f));
							m_pColl->SetOffset(Vec2(m_pColl->GetOffset().x, -10.f));
						}
					}
				}
				else if(!m_bJumpAttack)
				{
					m_pColl->SetOffset(m_vOffset.x, 10.f);
					m_pColl->SetScale(m_vScale);
				}
				if (m_fJump >= 0)
				{
					if (m_eDir == DIR::RIGHT)
						m_pAnim->PlayAnim(L"R_JumpDown", false);
					else if (m_eDir == DIR::LEFT)
						m_pAnim->PlayAnim(L"L_JumpDown", false);
				}
				else
				{
					JumpUpAnimation();
				}

				if (m_bJumpAttack)
				{
					if (m_eDir == DIR::RIGHT)
						m_pAnim->PlayAnim(L"R_JumpAttack", false);
					else if (m_eDir == DIR::LEFT)
						m_pAnim->PlayAnim(L"L_JumpAttack", false);
				}
			}
			else
			{
				m_pColl->SetOffset(m_vOffset);
				m_pColl->SetScale(m_vScale);
			}

			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_X, KEY_STATE::HOLD))
			{
				m_fShiningTime += DT;

				if (m_fShiningTime > 1)
					m_bShining = true;

				if (m_bShining)
					Shining();
					
			}
			else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_X, KEY_STATE::AWAY))
			{
				if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD))
				{
					for (UINT i = 0; i < CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].size(); ++i)
					{
						if (((CSkill*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL][i])->GetSkillType() == SKILL_TYPE::DART_ATTACK)
						{
							((CDart*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL][i])->SetGo(true);
						}
					}
					CKeyMgr::GetInst()->ComboClear();
				}
				else
				{
					m_fShiningTime = 0.f;
					if (!(m_iLadderCheck & LadderRiding))
					{
						if (m_bShining)
						{
							m_pTex = m_pTexOri;

							m_bJumpAttack = false;
							m_bAttack = true;

							if (m_pObj != nullptr)
							{
								m_pColl->SetOffset(m_vOffset);
								m_pColl->SetScale(m_vScale);
								CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
								m_pObj = nullptr;
							}

							CSoundMgr::GetInst()->Play(L"Slash", false);

							if (m_eDir == DIR::RIGHT)
								m_pAnim->PlayAnim(L"R_Smash", false);
							else if (m_eDir == DIR::LEFT)
								m_pAnim->PlayAnim(L"L_Smash", false);

							m_pAnim->ReStartAnim();

						}
						else
						{
							m_bJumpAttack = false;
							m_bAttack = true;

							if (m_pObj != nullptr)
							{
								m_pColl->SetOffset(m_vOffset);
								m_pColl->SetScale(m_vScale);
								CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
								m_pObj = nullptr;
							}

							CSoundMgr::GetInst()->Play(L"Attack", false);

							if (m_eDir == DIR::RIGHT)
								m_pAnim->PlayAnim(L"R_Attack", false);
							else if (m_eDir == DIR::LEFT)
								m_pAnim->PlayAnim(L"L_Attack", false);

							m_pAnim->ReStartAnim();
						}
					}
					else
					{
						m_pTex = m_pTexOri;
						m_bShining = false;
					}
				}
			}
		}

		if (!m_bAttack && CGameMgr::GetInst()->GetItemType() == ITEM_TYPE::WAND &&  KEY(KEY_TYPE::KEY_Z, KEY_STATE::TAB))
		{
			m_bJumpAttack = false;
			m_bAttack = true;

			CLightning* pLight = new CLightning;

			if (m_eDir == DIR::RIGHT)
			{
				m_pAnim->PlayAnim(L"R_Wand", false);
				Vec2 vPos = m_vRealPos;

				vPos.x += 110.f;
				pLight->SetRealPos(vPos);
			}
			else if (m_eDir == DIR::LEFT)
			{
				m_pAnim->PlayAnim(L"L_Wand", false);

				Vec2 vPos = m_vRealPos;

				vPos.x -= 110.f;
				pLight->SetRealPos(vPos);
			}

			pLight->SetOwner(this);
			pLight->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(pLight);

			m_pAnim->ReStartAnim();
		}
	}
	else if (m_bDamage)
	{
		//CCamMgr::GetInst()->SetControl(false);
		if(!m_bJump)
		{ 
			m_bJump = true;
			m_fJump = -80.f;
			m_iJumpFlag = 0x00;
			m_fAccJump = 0;
		}

		if (m_eDir == DIR::LEFT && !(m_iLadderCheck & LadderRiding))
		{
			m_vRealPos.x += 200.f * DT;
		}
		else if (m_eDir == DIR::RIGHT && !(m_iLadderCheck & LadderRiding))
		{
			m_vRealPos.x -= 200.f * DT;
		}
		m_fAccTime += DT;
		if (m_fAccTime > 0.5f)
		{
			//CCamMgr::GetInst()->SetControl(true);
			if (m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_Idle", false);
			else if (m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_Idle", false);
			m_fAccTime = 0;
			m_bDamage = false;
			m_bAlpha = true;
		}
	}

	if (m_bAlpha)
	{
		m_fAccTime += DT;
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0;
			m_bAlpha = false;
		}
	}

	if(!(m_iLadderCheck & LadderRiding))
		Gravity(m_vRealPos);
	else if((m_iLadderCheck & LadderRiding))
	{
		if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::HOLD) && !(m_iLadderCheck & LadderExit))
		{
			m_vRealPos.y -= 100.f * DT;
			LadderAnim();
			
		}
		if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD) && !(m_iLadderCheck & LadderExit))
		{
			m_vRealPos.y += 100.f * DT;
			LadderAnim();
		}
		else if (m_iLadderCheck & LadderExit)
		{
			if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
			{
				m_iLadderCheck ^= LadderExit;
				m_pAnim->PlayAnim(L"R_Ladder", false);
				m_fLadderTime = 0.f;
			}
			else if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD))
			{
				m_fLadderTime += DT;
				if (m_fLadderTime > 0.2f)
				{
					m_iLadderCheck ^= LadderExit;
					m_pAnim->PlayAnim(L"R_Ladder", false);
					m_fLadderTime = 0.f;
				}
			}
			else if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB))
			{
				m_vRealPos.y = m_fLadderY;
				m_iLadderCheck ^= LadderExit;
				m_iLadderCheck ^= LadderRiding;
				m_pAnim->PlayAnim(L"R_Idle",false);
				m_fLadderTime = 0.f;
			}
			else if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::HOLD))
			{
				m_fLadderTime += DT;
				if (m_fLadderTime > 0.2f)
				{
					m_vRealPos.y = m_fLadderY;
					m_iLadderCheck ^= LadderExit;
					m_iLadderCheck ^= LadderRiding;
					m_pAnim->PlayAnim(L"R_Idle", false);
					m_fLadderTime = 0.f;
				}
			}
		}
	}
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	if(m_iLadderCheck & LadderColl)
	m_iLadderCheck ^= LadderColl;
	else if((m_iLadderCheck & LadderColl) == 0)
	{
		if (!m_bStop)
		{
			if (m_iLadderCheck & LadderRiding)
			{
				m_fLadderTime = 0.f;
				m_iLadderCheck = 0;
			}
		}
		if (m_bStop)
			m_bStop = false;
	}
	CCamObj::update(); // Collidsion 업데이트이다.

	if ((m_pColl->GetPos().x - m_pColl->GetScale().x / 2.f) < -10)
	{
		m_pColl->SetPos(Vec2((m_pColl->GetScale().x / 2.f) - 10.f, m_pColl->GetPos().y));
		m_vPos.x = ((m_pColl->GetScale().x / 2.f) - 10.f) - m_pColl->GetOffset().x;
		m_vRealPos.x = m_vPos.x;
	}

	if (m_vPos.y > CCore::GetInst()->GetResolution().y + 200.f)
	{
		if (CStageMgr::GetInst()->GetCurState() == STAGE::KING)
		{
			if (CStageMgr::GetInst()->GetTileSizeY() * TILE_SIZE < m_vRealPos.y)
			{
				m_iHP = 0;
				CGameMgr::GetInst()->SetDeath(true);
			}
		}
		else
		{
			m_iHP = 0;
			CGameMgr::GetInst()->SetDeath(true);
		}
	}

	return 0;
}

void CPlayer::render(HDC _dc)
{
	if (m_bSpeedMoving && m_listFrame.size() > 0)
	{
		Vec2 vPos = CCamMgr::GetInst()->GetRealPos(m_listFrame.front().vPos.x, m_listFrame.front().vPos.y);
		m_pAnim->SceneRender(_dc, vPos, m_listFrame.front().wcsAnimKey, m_listFrame.front().iAnimFrame);
		m_listFrame.pop_front();
	}
	m_pAnim->render(_dc, m_bAlpha);
	Rectangle(_dc, m_pColl->GetPos().x -1, m_pColl->GetPos().y - 1, m_pColl->GetPos().x + 1, m_pColl->GetPos().y + 1);
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
		m_eDir = DIR::LEFT;
		m_pAnim->PlayAnim(L"L_Walk", true);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD))
	{
		m_eDir = DIR::RIGHT;
		m_pAnim->PlayAnim(L"R_Walk", true);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LEFT, KEY_STATE::AWAY))
	{
		m_eDir = DIR::LEFT;
		m_pAnim->PlayAnim(L"L_Idle", false);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RIGHT, KEY_STATE::AWAY))
	{
		m_eDir = DIR::RIGHT;
		m_pAnim->PlayAnim(L"R_Idle", false);
	}
}

void CPlayer::JumpUpAnimation()
{
	if (KEY_MGR(KEY_TYPE::KEY_LEFT, KEY_STATE::HOLD) || KEY_MGR(KEY_TYPE::KEY_LEFT, KEY_STATE::AWAY))
	{
		m_eDir = DIR::LEFT;
		m_pAnim->PlayAnim(L"L_JumpUp", false);
	}
	else if (KEY_MGR(KEY_TYPE::KEY_RIGHT, KEY_STATE::HOLD) || KEY_MGR(KEY_TYPE::KEY_RIGHT, KEY_STATE::AWAY))
	{
		m_eDir = DIR::RIGHT;
		m_pAnim->PlayAnim(L"R_JumpUp", false);
	}
}

void CPlayer::JumpDownAnimation()
{
}

void CPlayer::LadderAnimation()
{
	if (KEY(KEY_TYPE::KEY_LEFT, KEY_STATE::TAB))
	{
		m_eDir = DIR::LEFT;
		m_pAnim->PlayAnim(L"L_Ladder",false);
	}
	else if (KEY(KEY_TYPE::KEY_RIGHT, KEY_STATE::TAB))
	{
		m_eDir = DIR::RIGHT;
		m_pAnim->PlayAnim(L"R_Ladder", false);
	}
}

void CPlayer::Event()
{
	if (m_bJump)
		Gravity(m_vRealPos);
	else
	{
		CGameMgr::GetInst()->SetPlayerStop(true);
		m_bEvent = false;
		m_pAnim->PlayAnim(L"UP", false);
	}
}

void CPlayer::Skill()
{
	
		if (CKeyMgr::GetInst()->GetCombo().size() >= 3)
		{
			if (CKeyMgr::GetInst()->GetCombo()[0] == KEY_TYPE::KEY_DOWN && CKeyMgr::GetInst()->GetCombo()[1] == KEY_TYPE::KEY_DOWN && 
				CKeyMgr::GetInst()->GetCombo()[2] == KEY_TYPE::KEY_Z)
			{
				m_bJump = false;
				if (m_eDir == DIR::RIGHT)
				{
					m_vPos.x += 100;
					m_vRealPos.x += 100;
				}
				else if (m_eDir == DIR::LEFT)
				{
					m_vPos.x -= 100;
					m_vRealPos.x -= 100;
				}
				CCamMgr::GetInst()->SetLook(m_vPos.x, CCamMgr::GetInst()->GetLook().y);
				CKeyMgr::GetInst()->ComboClear();
			}
		}
	
		if (CKeyMgr::GetInst()->GetCombo().size() >= 2)
		{
			if (CKeyMgr::GetInst()->GetCombo()[0] == KEY_TYPE::KEY_RIGHT && CKeyMgr::GetInst()->GetCombo()[1] == KEY_TYPE::KEY_RIGHT)
			{
				m_bSkill = true;
				m_fSkillTime = 0.f;
				CKeyMgr::GetInst()->ComboClear();
			}
			else if (CKeyMgr::GetInst()->GetCombo()[0] == KEY_TYPE::KEY_LEFT && CKeyMgr::GetInst()->GetCombo()[1] == KEY_TYPE::KEY_LEFT)
			{
				m_bSkill = true;
				m_fSkillTime = 0.f;
				CKeyMgr::GetInst()->ComboClear();
			}
			else if (CKeyMgr::GetInst()->GetCombo()[0] == KEY_TYPE::KEY_DOWN && CKeyMgr::GetInst()->GetCombo()[1] == KEY_TYPE::KEY_DOWN)
			{
				CDart* pDart = new CDart;
				pDart->SetOwner(this);
				pDart->Init();
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(pDart);
				CKeyMgr::GetInst()->ComboClear();
			}
			else if (CKeyMgr::GetInst()->GetCombo()[0] == KEY_TYPE::KEY_DOWN && CKeyMgr::GetInst()->GetCombo()[1] == KEY_TYPE::KEY_Z)
			{
				for (UINT i = 0; i < CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].size(); ++i)
				{
					if (((CSkill*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL][i])->GetSkillType() == SKILL_TYPE::DART_ATTACK)
					{
						((CDart*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL][i])->SetGo(true);
					}
				}
				CKeyMgr::GetInst()->ComboClear();
			}
		}
		else if (CKeyMgr::GetInst()->GetCombo().size() >= 2)
		{
		}

}

void CPlayer::SpeedMoving()
{
	m_listFrame.push_back(tFrame(CCamMgr::GetInst()->GetCamMousePos(m_vPos),m_pAnim->GetCurFrame(),m_pAnim->GetCurKey()));
}

void CPlayer::SpeedMovingOff(DIR _eDir)
{
	m_bStop = true;
	m_fSkillTime = 0.f;
	m_bSpeedMoving = false;
	m_bSkill = false;
	m_listFrame.clear();
	m_fSaveSpeed = 300.f;
	CCamMgr::GetInst()->SetSpeed(m_fSaveSpeed);
	if (!m_bJump && (_eDir != DIR::UP && _eDir != DIR::DOWN))
	{
		if (m_eDir == DIR::RIGHT)
			m_pAnim->PlayAnim(L"R_Idle", false);
		else if (m_eDir == DIR::LEFT)
			m_pAnim->PlayAnim(L"L_Idle", false);
	}
}

void CPlayer::Shining()
{
	if (m_fShiningTime < 0.25f)
		m_pTex = CDCMgr::GetInst()->GetPlayerTex((UINT)EQUIP_TYPE::SHINING);
	else if (m_fShiningTime < 0.5f)
		m_pTex = m_pTexOri;
	else
		m_fShiningTime = 0.f;
}

void CPlayer::Ending()
{
	if (m_bJump)
	{
		if (m_fJump >= 0)
		{
			if (m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_JumpDownShield", false);
			else if (m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_JumpDownShield", false);
		}
		else
		{
			if(m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_JumpShield", false);
			else if(m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_JumpShield", false);
		}
	}
	else
	{
		if (m_eDir == DIR::RIGHT)
			m_pAnim->PlayAnim(L"R_Shield", false);
		else if (m_eDir == DIR::LEFT)
			m_pAnim->PlayAnim(L"L_Shield", false);
	}
}

void CPlayer::LadderAnim()
{
	m_fLadderTime += DT;
	if (m_fLadderTime - 0.2f < 0.f)
	{
		m_eDir = DIR::LEFT;
	}
	else if (m_fLadderTime - 0.4f < 0.f)
	{
		m_eDir = DIR::RIGHT;
	}
	else
		m_fLadderTime = 0.f;

	if (m_eDir == DIR::RIGHT)
		m_pAnim->PlayAnim(L"R_Ladder", false);
	else if (m_eDir == DIR::LEFT)
		m_pAnim->PlayAnim(L"L_Ladder", false);
}

void CPlayer::Damage()
{
	if (!m_bAlpha && !m_bDamage)
	{
		if (m_pObj != nullptr)
		{
			CStageMgr::GetInst()->EraseStageObj(OBJ_TYPE::SKILL, m_pObj);
			m_pColl->SetOffset(m_vOffset);
			m_pColl->SetScale(m_vScale);
			m_pObj = nullptr;
		}
		m_bDamage = true;
		m_bJump = true;
		m_iHP -= 1;
		
		if(m_iHP <= 0)
			CSoundMgr::GetInst()->Play(L"Die", false);
		else
		CSoundMgr::GetInst()->Play(L"Hurt",false);

		((CUI*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI][0])->SetDamage(1);


		if (m_eDir == DIR::LEFT)
			m_pAnim->PlayAnim(L"L_Damage", false);
		else if (m_eDir == DIR::RIGHT)
			m_pAnim->PlayAnim(L"R_Damage", false);

		m_fJump = -200.f;
		m_iJumpFlag = 0x00;
		m_fAccJump = 0;
		m_iLadderCheck = 0;
	}
}

DIR CPlayer::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	CDynamicObj::OnCollisionEnter(_mine,_other);

	if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE && ((CTile*)_other->GetOwner())->GetTileType() == TILE_TYPE::TRAP)
	{
		CSoundMgr::GetInst()->Play(L"Die",false);
		((CUI*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI][0])->SetDamage(m_iHP);
		m_iHP = 0;
	}
	if (_other->GetOwner()->GetType() == OBJ_TYPE::MONSTER && ((CDynamicObj*)_other->GetOwner())->GetHP() > 0)
		Damage();
	else if (_other->GetOwner()->GetType() == OBJ_TYPE::SKILL)
	{
		if (((CSkill*)_other->GetOwner())->GetOwner() != nullptr)
		{
			if (((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::MONSTER
				|| ((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::KING)
			{
				Damage();
			}
		}
		else
		{
			Damage();
		}
	}

	if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE && ((CTile*)_other->GetOwner())->GetTileType() == TILE_TYPE::LADDER)
	{
		//m_iLadderCheck |= LadderColl;

		//if (abs(m_pColl->GetPrePos().x - _other->GetPrePos().x) + 1 <= m_pColl->GetScale().x / 2.f + _other->GetScale().x / 2.f) // 이미 그 이전에 x축 충돌을 했다면
		//{
		//	float fUP = 0.f;
		//	if (m_pColl->GetPrePos().y + m_pColl->GetScale().y / 2.f < _other->GetPrePos().y) // 위쪽이라면
		//	{
		//		m_vPos.y = _other->GetPos().y - (_other->GetScale().y / 2.f) - (m_pColl->GetScale().y / 2.f) - m_pColl->GetOffset().y;
		//		m_vRealPos.y = m_vPos.y;

		//		m_fJump = 0.f;
		//		if (m_bJump)
		//			m_bJump = false;

		//		Vec2 vPrePos = Vec2(m_pColl->GetPos().x, (m_vPos.y + m_pColl->GetOffset().y));
		//		m_pColl->SetPos(vPrePos);
		//		m_pColl->SetPrePos(vPrePos);
		//		return DIR::NONE;
		//	}
		//}
	}

	if (_other->GetOwner()->GetType() == OBJ_TYPE::SHIELD)
	{
		((CDynamicObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SHIELD][0])->SetDeath(true);
		m_bEnd = true;
		CGameMgr::GetInst()->SetPlayerStop(true);
	}
	CCamMgr::GetInst()->SetPlayerPos(m_pColl->GetPos().x, CCamMgr::GetInst()->GetLook().y);
	CCamMgr::GetInst()->SetLook(m_vRealPos.x, CCamMgr::GetInst()->GetLook().y);

	return DIR::NONE;
}

void CPlayer::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollision(_other);

	if (_other->GetOwner()->GetType() == OBJ_TYPE::MONSTER && ((CDynamicObj*)_other->GetOwner())->GetHP() > 0)
		Damage();
	else if (_other->GetOwner()->GetType() == OBJ_TYPE::SKILL)
	{
		if (((CSkill*)_other->GetOwner())->GetOwner() != nullptr)
		{
			if (((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::MONSTER
				|| ((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::KING)
			{
				Damage();
			}
		}
		else
		{
			Damage();
		}
	}
	
	// 사다리인 경우
	if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE && ((CTile*)_other->GetOwner())->GetTileType() == TILE_TYPE::LADDER)
	{
		// 사다리 충돌 체크
		m_iLadderCheck |= LadderColl;

		int iX = 0;
		int iY = 0;

		Vec2 vPos = CCamMgr::GetInst()->GetCamMousePos(((CCamObj*)_other->GetOwner())->GetPos().x, ((CCamObj*)_other->GetOwner())->GetPos().y);
		iX = (int)vPos.x / TILE_SIZE;
		iY = ((int)vPos.y / TILE_SIZE)-1;

		if (iY < 0)
			iY = 0;
		bool LadderCheck = ((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iX + (iY * CStageMgr::GetInst()->GetTileSizeX())])->GetTileType() == TILE_TYPE::LADDER; // 내가 충돌한 위쪽 타일이 사다리라렴ㄴ
		//((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iX + (iY * CStageMgr::GetInst()->GetTileSizeX())])->SetDebug(true);
		iY = ((int)vPos.y / TILE_SIZE) + 1;
		bool LadderDownCheck = ((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iX + (iY * CStageMgr::GetInst()->GetTileSizeX())])->GetTileType() == TILE_TYPE::NONE; // 내가 충돌한 타일 아래쪽이 비었다면
		// 사다리가 맨 꼭대기에 서있는 상태
		if (!LadderCheck && !(m_iLadderCheck & LadderRiding) && abs(m_pColl->GetPrePos().x - _other->GetPrePos().x) + 1 <= m_pColl->GetScale().x / 2.f + _other->GetScale().x / 2.f) // 이미 그 이전에 x축 충돌을 했다면
		{
			float fUP = 0.f;
			if (m_pColl->GetPrePos().y + m_pColl->GetScale().y / 2.f < _other->GetPrePos().y) // 위쪽이라면
			{
				m_vPos.y = _other->GetPos().y - (_other->GetScale().y / 2.f) - (m_pColl->GetScale().y / 2.f) - m_pColl->GetOffset().y;
				m_vRealPos.y = m_vPos.y;

				m_fJump = 0.f;
				if (m_bJump)
					m_bJump = false;

				Vec2 vPrePos = Vec2(m_pColl->GetPos().x, (m_vPos.y + m_pColl->GetOffset().y));
				m_pColl->SetPos(vPrePos);
				m_pColl->SetPrePos(vPrePos);

				if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
				{
					m_fLadderTime = 0.f;
					m_iLadderCheck |= LadderExit;
					m_vRealPos.y = _other->GetRealPos().y - (_other->GetScale().y / 2.f) - m_pColl->GetOffset().y;
					m_fLadderTime = 0.f;
					m_vRealPos.x = _other->GetRealPos().x;
					m_iLadderCheck |= LadderRiding;
					m_bJump = false;
					m_iJumpFlag = 0x00;
					m_fAccJump = 0;
					m_fJump = 0.f;
					m_pAnim->PlayAnim(L"LadderEnd", false);
				}
				return;
			}
		}

		if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB) || KEY(KEY_TYPE::KEY_UP, KEY_STATE::HOLD))
		{
			if (!LadderCheck && !(m_iLadderCheck & LadderExit) && (m_iLadderCheck & LadderRiding) && (m_pColl->GetPos().y) < _other->GetPos().y - (_other->GetScale().y / 2.f))
			{
				m_fLadderTime = 0.f;
				m_pAnim->PlayAnim(L"LadderEnd", false);
				m_iLadderCheck |= LadderExit;
				m_fLadderY = _other->GetPos().y - (_other->GetScale().y / 2.f) - (m_pColl->GetScale().y / 2.f) - m_pColl->GetOffset().y;
				return;
			}
		}
		if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB) && (m_pColl->GetPos().y) > _other->GetPos().y - (_other->GetScale().y / 2.f))
		{
			m_fLadderTime = 0.f;
			m_vRealPos.x = _other->GetRealPos().x;
			m_iLadderCheck |= LadderRiding;
			m_bJump = false;
			m_iJumpFlag = 0x00;
			m_fAccJump = 0;
			m_fJump = 0.f;
		}

		if (LadderDownCheck && !(m_iLadderCheck & LadderExit) && (m_iLadderCheck & LadderRiding) && (m_pColl->GetPos().y) > _other->GetPos().y + (_other->GetScale().y / 2.f))
		{
		/*if ((_other->GetRealPos().y + _other->GetScale().y / 2.f) < m_pColl->GetRealPos().y)
		{*/
			if ((m_iLadderCheck & LadderRiding) && (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::HOLD) || KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB)))
			{
				m_bLadderExit = true;
				m_bJump = true;
				m_pAnim->PlayAnim(L"R_JumpDown", false);
				m_fLadderTime = 0.f;
				m_iLadderCheck = 0;
			}
		}
	}

	if ((_other->GetOwner()->GetType() == OBJ_TYPE::TILE && ((CTile*)_other->GetOwner())->GetTileType() == TILE_TYPE::COLL) || _other->GetOwner()->GetType() == OBJ_TYPE::BLOCK)
	{
		if ((m_iLadderCheck & LadderRiding) && (KEY(KEY_TYPE::KEY_DOWN,KEY_STATE::HOLD) || KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB)))
		{
			m_pAnim->PlayAnim(L"R_Idle", false);
			m_fLadderTime = 0.f;
			m_iLadderCheck = 0;
		}
	}
}

void CPlayer::OnCollisionExit(CCollider * _other)
{
	/*if (_other->GetOwner()->GetType() == OBJ_TYPE::TILE && ((CTile*)_other->GetOwner())->GetTileType() == TILE_TYPE::LADDER)
	{
		m_iLadderCheck |= LadderExit;
		m_fLadderY = _other->GetPos().y - (_other->GetScale().y / 2.f) - (m_pColl->GetScale().y / 2.f) - m_pColl->GetOffset().y;
	}*/
}
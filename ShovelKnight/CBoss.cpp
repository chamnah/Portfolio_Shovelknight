#include "stdafx.h"
#include "CBoss.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"
#include "CPurpleFire.h"
#include "CStageMgr.h"
#include "CUI.h"
#include "CDCMgr.h"
#include "CFireBlock.h"
#include "SoundMgr.h"

void CBoss::Init()
{
	CCamObj::Init();
	m_fSpeed = 300.f;
	m_iHP = 6;
	m_iMaxHP = m_iHP;
	srand((UINT)time(NULL));

	for (int i = 0; i < 25; ++i)
	{
		m_sRandom[i] = i;
	}
}

int CBoss::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	
	if (m_iHP > 0)
	{
		if (CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x - m_vRealPos.x < 0)
			m_eDir = DIR::LEFT;
		else
			m_eDir = DIR::RIGHT;

		Damage(false);

		if (m_FSM == BOSS_FSM::WINDOW_VIEW)
			m_pAnim->PlayAnim(L"WindowView", true);
		else if (m_FSM == BOSS_FSM::WINDOW_IDLE)
			m_pAnim->PlayAnim(L"WindowIdle", true);
		else if (m_FSM == BOSS_FSM::BLOW)
		{
			m_pAnim->PlayAnim(L"Blow", false);
			if (m_pAnim->GetCurFrame() == 15)
				m_FSM = BOSS_FSM::IDLE;
		}
		else if (m_FSM == BOSS_FSM::IDLE)
			m_pAnim->PlayAnim(L"L_Walk", true);
		else if (m_FSM == BOSS_FSM::FIGHT)
			Figjt();

		CCamObj::update();
	}
	else if(!m_bTransform)
	{
		m_pTex = TEX_LOAD(L"Boss", L"Image\\Boss.bmp");
		m_bDeath = true;
		m_fAcc += m_fSpeed * DT;
		m_vRealPos.x += m_fSpeed * DT;

		if (m_fAcc >= 10.f)
		{
			m_fSpeed = -300.f;
		}
		else if(m_fAcc <= 0)
			m_fSpeed = 300.f;
		m_pAnim->PlayAnim(L"Death",true);

		m_bFirst = false;
	}
	else if (m_bTransform)
	{
		if (!m_bFirst)
		{
			m_bFirst = true;
			CSoundMgr::GetInst()->Play(L"TransForm", false);
		}
		m_pAnim->PlayAnim(L"Transform", true);
	}
	m_pAnim->update();
	
	return 0;
}

void CBoss::render(HDC _dc)
{
	if (!m_bAlpha)
		m_pAnim->render(_dc);
	else
		TransparentBlt(_dc, m_vPos.x + 15.f - m_pTexShield->GetWidth() * 2, m_vPos.y + 30.f - m_pTexShield->GetHeight() * 2, m_pTexShield->GetWidth() * 4, m_pTexShield->GetHeight() * 4,
			m_pTexShield->GetDC(),0, 0, m_pTexShield->GetWidth(), m_pTexShield->GetHeight(), RGB(0, 255, 0));
	CCamObj::render(_dc);
}

void CBoss::RightAttack()
{
	if (Moving(64.f, 1600.f))
	{

		if (!m_bSkill)
		{
			m_pAnim->PlayAnim(L"R_Attack", true);
			if (m_pAnim->GetCurFrame() == 3)
			{
				CSoundMgr::GetInst()->Play(L"FireBall", false);
				m_iState = CHARGE_END;
				m_bSkill = true;
				CSkill* pSkill = new CPurpleFire;
				Vec2 vPos = m_vRealPos;
				vPos.x += 30;
				vPos.y += 30;
				pSkill->SetRealPos(vPos);
				pSkill->SetPos(vPos);
				pSkill->InitColl();
				((CPurpleFire*)pSkill)->SetDir(DIR::RIGHT);
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(pSkill);
				++m_iNum;
			}
		}
		else if (m_bSkill && m_pAnim->GetCurFrame() != 3)
			m_bSkill = false;
	}
	if (m_iNum >= 3 && m_pAnim->GetCurFrame() == 4)
	{
		m_iNum = 0;
		m_eFight = BOSS_FSM::IDLE;
		m_bSkill = false;
	}
}

void CBoss::LeftAttack()
{
	if (Moving(1600.f - 64.f, 1600.f))
	{
		if (!m_bSkill)
		{
			m_pAnim->PlayAnim(L"L_Attack", true);
			if (m_pAnim->GetCurFrame() == 3)
			{
				CSoundMgr::GetInst()->Play(L"FireBall", false);
				m_iState = CHARGE_END;
				m_bSkill = true;
				CSkill* pSkill = new CPurpleFire;
				Vec2 vPos = m_vRealPos;
				vPos.x -= 30;
				vPos.y += 30;
				pSkill->SetRealPos(vPos);
				pSkill->SetPos(vPos);
				pSkill->InitColl();
				((CPurpleFire*)pSkill)->SetDir(DIR::LEFT);
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(pSkill);
				++m_iNum;
			}
		}
		else if (m_bSkill && m_pAnim->GetCurFrame() != 3)
			m_bSkill = false;
	}
	if (m_iNum >= 3 && m_pAnim->GetCurFrame() == 4)
	{
		m_iNum = 0;
		m_eFight = BOSS_FSM::IDLE;
		m_bSkill = false;
	}
}

void CBoss::DownAttack()
{
	if (Moving(CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x, 1400.f))
		m_bMoving = true;
	if (m_bMoving)
	{
		if (!m_bSkill)
		{
			m_pAnim->PlayAnim(L"D_Attack", true);
			m_iState = CHARGE_END;
			if (m_pAnim->GetCurFrame() == 3)
			{
				CSoundMgr::GetInst()->Play(L"FireBall", false);
				m_bSkill = true;
				CSkill* pSkill = new CPurpleFire;
				pSkill->SetRealPos(m_vRealPos);
				pSkill->SetPos(m_vRealPos);
				pSkill->InitColl();
				((CPurpleFire*)pSkill)->SetDir(DIR::DOWN);
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(pSkill);
				++m_iNum;
			}
		}
		else if (m_bSkill && m_pAnim->GetCurFrame() != 3)
		{
			m_fTime = 0.f;
			m_bSkill = false;
		}
	}
	if (m_iNum >= 3 && m_pAnim->GetCurFrame() == 4)
	{
		m_iNum = 0;
		m_eFight = BOSS_FSM::IDLE;
		m_bSkill = false;
		m_bMoving = false;
	}
}

void CBoss::UpAttack()
{
	if (Moving(CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x, 2000.f))
		m_bMoving = true;
	if (m_bMoving)
	{
		if (!m_bSkill)
		{
			m_pAnim->PlayAnim(L"U_Attack", true);
			m_iState = CHARGE_END;
			if (m_pAnim->GetCurFrame() == 3)
			{
				CSoundMgr::GetInst()->Play(L"FireBall", false);
				m_bSkill = true;
				CSkill* pSkill = new CPurpleFire;
				pSkill->SetRealPos(m_vRealPos);
				pSkill->SetPos(m_vRealPos);
				pSkill->InitColl();
				((CPurpleFire*)pSkill)->SetDir(DIR::UP);
				CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::SKILL].push_back(pSkill);
				++m_iNum;
			}
		}
		else if (m_bSkill && m_pAnim->GetCurFrame() != 3)
		{
			m_fTime = 0.f;
			m_bSkill = false;
		}
	}
	if (m_iNum >= 3 && m_pAnim->GetCurFrame() == 4)
	{
		m_iNum = 0;
		m_eFight = BOSS_FSM::IDLE;
		m_bSkill = false;
		m_bMoving = false;
	}
}

void CBoss::FireAttack()
{
	m_fTime += DT;
	if (m_fTime >= 2)
	{
		m_iState = CHARGE_END;
		if (!m_bSkill)
		{
			m_bSkill = true;
			if (m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_FireAttack", true);
			else if (m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_FireAttack", true);
		}
		if (m_pAnim->GetCurFrame() == 4)
		{
			for (UINT i = 0; i < CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::FIRE_BLOCK].size(); ++i)
			{
				((CFireBlock*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::FIRE_BLOCK][i])->SetMoving(true);
			}
			m_eFight = BOSS_FSM::IDLE;
			m_fTime = 0.f;
			m_bFire = false;
			m_bSkill = false;
		}
	}
}

void CBoss::Figjt()
{
	Charge();

	if (m_eFight == BOSS_FSM::RIGHT_ATTACK)
		RightAttack();
	else if (m_eFight == BOSS_FSM::LEFT_ATTACK)
		LeftAttack();
	else if (m_eFight == BOSS_FSM::DOWN_ATTACK)
		DownAttack();
	else if (m_eFight == BOSS_FSM::UP_ATTACK)
		UpAttack();
	else if (m_eFight == BOSS_FSM::FIRE_ATTACK)
		FireAttack();
	else if (m_eFight == BOSS_FSM::IDLE)
		Idle();
	
}

float CBoss::Normalize(float x, float y)
{
	return sqrtf(pow((x - m_vRealPos.x), 2) + powf((y - m_vRealPos.y), 2));
}

bool CBoss::Moving(float _fX, float _fY)
{
	if (abs(m_vRealPos.y - _fY) > 1.f || abs(m_vRealPos.x - _fX) > 1.f)
	{
		float fDir = Normalize(_fX, _fY);
		float fDirX = (_fX - m_vRealPos.x) / fDir;
		float fDirY = (_fY - m_vRealPos.y) / fDir;
		m_vRealPos.x     += (m_fSpeed * DT) * fDirX;
		m_vRealPos.y += (m_fSpeed * DT) * fDirY;
		return false;
	}
	return true;
}

void CBoss::TakeDamage(int iDamage, DIR _eDir)
{
	m_iHP -= 1;
	((CUI*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI][0])->SetBossDamage(1);
	m_bDamage = true;
}

void CBoss::Damage(bool _bRepeat)
{
	if (m_bDamage)
	{
		if (m_fAccTime <= 0.05f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 0);
		else if (m_fAccTime <= 0.10f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 1);
		else if (m_fAccTime <= 0.15f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 2);
		else if (m_fAccTime <= 0.20f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 3);
		else if (m_fAccTime <= 0.25f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 4);
		else if (m_fAccTime <= 0.30f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 5);
		else if (m_fAccTime <= 0.35f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::KING, 6);
		else if (!_bRepeat)
		{
			m_pTex = TEX_LOAD(L"Boss", L"Image\\Boss.bmp");
			m_fAccTime = 0.f;
			m_bDamage = false;
		}
		else // Áö¿ö¾ß´ï
		{
			Vec2 vPos = Vec2(rand() % 201, (rand() % 201) - 50);
			CreateEffect(m_vRealPos + vPos, TEX_LOAD(L"Explosion", L"Image\\Explosion.bmp"), 5);
			if (m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_Attack", false);
			else if (m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_Attack", false);
			m_fAccTime = 0.f;
		}
		m_fAccTime += DT;
	}
}

void CBoss::Idle()
{
	m_fTime += DT;

	if (m_fTime >= 3.f)
	{
		if (CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::BLOCK].size() <= 40)
			m_eFight = BOSS_FSM::FIRE_ATTACK;
		else
			m_eFight = (BOSS_FSM)((rand() % 5) + 5);
		if (m_eFight == BOSS_FSM::FIRE_ATTACK)
			m_bFire = true;
		m_fTime = 0.f;
		m_iState = 0;
		return ;
	}

	if(m_eDir == DIR::RIGHT)
		m_pAnim->PlayAnim(L"R_Walk", true);
	else if(m_eDir == DIR::LEFT)
		m_pAnim->PlayAnim(L"L_Walk", true);
}

void CBoss::Charge()
{
	if ((m_iState & CHARGE_END) == 0)
	{
		if (m_bFire)
		{
			if (!m_bFirst)
			{
				m_bFirst = true;

				Shuffle();

				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						CFireBlock* pFire = new CFireBlock;
						pFire->SetPos(Vec2((m_sRandom[j] * TILE_SIZE) + 32, 1960 + (i * 64)));
						pFire->SetOwner(this);
						pFire->Init();
						CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::FIRE_BLOCK].push_back(pFire);
					}
				}
			}
			if ((m_iState & CHARGE_IDLE) ||
				(m_pAnim->GetCurKey() == L"R_FireCharge" || m_pAnim->GetCurKey() == L"L_FireCharge") && m_pAnim->GetCurFinish())
			{
				if (m_pAnim->GetCurKey() == L"R_FireChargeIdle" || m_pAnim->GetCurKey() == L"L_FireChargeIdle")
					m_pAnim->SetCurFrame(m_pAnim->GetCurFrame());

				m_iState = CHARGE_IDLE;
				if (m_eDir == DIR::RIGHT)
					m_pAnim->PlayAnim(L"R_FireChargeIdle", true);
				else if (m_eDir == DIR::LEFT)
					m_pAnim->PlayAnim(L"L_FireChargeIdle", true);
			}
			else
			{
				if(m_pAnim->GetCurKey() != L"R_FireCharge" && m_pAnim->GetCurKey() != L"L_FireCharge")
					CSoundMgr::GetInst()->Play(L"Wand",false);
				int iFrame = 0;
				if (m_pAnim->GetCurKey() == L"R_FireCharge" || m_pAnim->GetCurKey() == L"L_FireCharge")
					iFrame = m_pAnim->GetCurFrame();

				if (m_eDir == DIR::RIGHT)
					m_pAnim->PlayAnim(L"R_FireCharge", false);
				else if (m_eDir == DIR::LEFT)
					m_pAnim->PlayAnim(L"L_FireCharge", false);

				m_pAnim->SetCurFrame(iFrame);
			}
		}
		else
		{
			if ((m_iState & CHARGE_IDLE) ||
				(m_pAnim->GetCurKey() == L"R_Charge" || m_pAnim->GetCurKey() == L"L_Charge") && m_pAnim->GetCurFinish())
			{
				if (m_pAnim->GetCurKey() == L"R_ChargeIdle" || m_pAnim->GetCurKey() == L"L_ChargeIdle")
				{
					m_pAnim->SetCurFrame(m_pAnim->GetCurFrame());
				}

				m_iState = CHARGE_IDLE;
				if (m_eDir == DIR::RIGHT)
					m_pAnim->PlayAnim(L"R_ChargeIdle", true);
				else if (m_eDir == DIR::LEFT)
					m_pAnim->PlayAnim(L"L_ChargeIdle", true);
			}
			else
			{
				int iFrame = 0;
				if (m_pAnim->GetCurKey() == L"R_Charge" || m_pAnim->GetCurKey() == L"L_Charge")
					iFrame = m_pAnim->GetCurFrame();

				if (m_pAnim->GetCurKey() != L"R_Charge" && m_pAnim->GetCurKey() != L"L_Charge")
					CSoundMgr::GetInst()->Play(L"Wand",false);
				if (m_eDir == DIR::RIGHT)
					m_pAnim->PlayAnim(L"R_Charge", false);
				else if (m_eDir == DIR::LEFT)
					m_pAnim->PlayAnim(L"L_Charge", false);

				m_pAnim->SetCurFrame(iFrame);
			}
		}
	}
	else
	{
		m_bFirst = false;
	}
}

void CBoss::Shuffle()
{
	int iTemp = 0;
	for (int i = 0; i < 100; ++i)
	{
		int iRandom = rand() % 25;
		int iRandom2 = rand() % 25;
		iTemp = m_sRandom[iRandom2];
		m_sRandom[iRandom2] = m_sRandom[iRandom];
		m_sRandom[iRandom] = iTemp;
	}
}

CBoss::CBoss()
	:m_FSM(BOSS_FSM::WINDOW_VIEW)
	, m_bSkill(false)
	, m_iState(0)
	, m_eFight(BOSS_FSM::FIRE_ATTACK)
	, m_bFirst(false)
	, m_fAcc(0.f)
	, m_bAlpha(false)
	, m_bTransform(false)
	, m_bFire(true)
	, m_pTexShield(nullptr)
	, m_bMoving(false)
{
	m_pTex = TEX_LOAD(L"Boss",L"Image\\Boss.bmp");
	m_pTexShield = TEX_LOAD(L"ShieldKnightAppear",L"Image\\ShieldKnightAppear.bmp");
	m_tSize = tSize(93,84);
	m_pAnim->AddAnimation(L"R_Walk", m_pTex, RECT{ 0,m_tSize.y * 0,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_Walk", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_BackWalk", m_pTex, RECT{ 0,m_tSize.y * 2,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"R_BackWalk", m_pTex, RECT{ 0,m_tSize.y * 3,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"R_Charge", m_pTex, RECT{ 0,m_tSize.y * 4,m_tSize.x,m_tSize.y }, 9, 0.1f);
	m_pAnim->AddAnimation(L"L_Charge", m_pTex, RECT{ 0,m_tSize.y * 5,m_tSize.x,m_tSize.y }, 9, 0.1f);
	m_pAnim->AddAnimation(L"R_ChargeIdle", m_pTex, RECT{ 0,m_tSize.y * 6,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_ChargeIdle", m_pTex, RECT{ 0,m_tSize.y * 7,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"R_Attack", m_pTex, RECT{ 0,m_tSize.y * 8,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"L_Attack", m_pTex, RECT{ 0,m_tSize.y * 9,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"D_Attack", m_pTex, RECT{ 0,m_tSize.y * 10,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"RD_Attack", m_pTex, RECT{ 0,m_tSize.y * 11,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"LD_Attack", m_pTex, RECT{ 0,m_tSize.y * 12,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"RU_Attack", m_pTex, RECT{ 0,m_tSize.y * 13,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"LU_Attack", m_pTex, RECT{ 0,m_tSize.y * 14,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"U_Attack", m_pTex, RECT{ 0,m_tSize.y * 15,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"R_FireCharge", m_pTex, RECT{ 0,m_tSize.y * 16,m_tSize.x,m_tSize.y }, 9, 0.1f);
	m_pAnim->AddAnimation(L"L_FireCharge", m_pTex, RECT{ 0,m_tSize.y * 17,m_tSize.x,m_tSize.y }, 9, 0.1f);
	m_pAnim->AddAnimation(L"R_FireChargeIdle", m_pTex, RECT{ 0,m_tSize.y * 18,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"L_FireChargeIdle", m_pTex, RECT{ 0,m_tSize.y * 19,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"R_FireAttack", m_pTex, RECT{ 0,m_tSize.y * 20,m_tSize.x,m_tSize.y }, 5, 0.1f);
	m_pAnim->AddAnimation(L"L_FireAttack", m_pTex, RECT{ 0,m_tSize.y * 21,m_tSize.x,m_tSize.y }, 5, 0.1f);
	m_pAnim->AddAnimation(L"WindowView", m_pTex, RECT{0,m_tSize.y * 22,m_tSize.x,m_tSize.y},4,0.2f);
	m_pAnim->AddAnimation(L"WindowIdle", m_pTex, RECT{ 0,m_tSize.y * 23,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->AddAnimation(L"Blow", m_pTex, RECT{ 0,m_tSize.y * 24,m_tSize.x,m_tSize.y }, 16, 0.1f);
	m_pAnim->AddAnimation(L"Death", m_pTex, RECT{ 0,m_tSize.y * 25,m_tSize.x,m_tSize.y }, 4, 0.1f);

	m_pAnim->AddAnimation(L"Transform", m_pTex, RECT{ 0,m_tSize.y * 27,m_tSize.x,m_tSize.y }, 4, 0.2f);
	m_pAnim->PlayAnim(L"WindowView",true);
	m_eType = OBJ_TYPE::KING;
}

CBoss::~CBoss()
{
}
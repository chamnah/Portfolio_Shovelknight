#include "stdafx.h"
#include "CBubbleDragon.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CCollider.h"
#include "CDCMgr.h"
#include "CTimeMgr.h"
#include "CEffect.h"
#include "CStageMgr.h"
#include "CBubbleAttack.h"
#include "CCore.h"
#include "CCamMgr.h"
#include "CTile.h"
#include "CCoin.h"
#include "SoundMgr.h"

CBubbleDragon::CBubbleDragon()
	:m_bDamage(false)
	, m_fDeathTime(0.f)
	, m_bState(false)
	, FP(&CBubbleDragon::Idle)
	, m_fStateTime(0.f)
	, m_iNum(0)
	, m_fCoinTime(0.f)
{
}

CBubbleDragon::CBubbleDragon(float fPosX, float fPosY)
	:m_bDamage(false)
	, m_fDeathTime(0.f)
	, m_bState(false)
	, FP(&CBubbleDragon::Idle)
	, m_fStateTime(0.f)
	, m_iNum(0)
{
	m_vPos = Vec2(fPosX,fPosY);
	m_vRealPos = m_vPos;
}

CBubbleDragon::~CBubbleDragon()
{
	delete m_pCollHead;
	m_pCollHead = nullptr;

	for (int i = 0; i < 4; ++i)
	{
		delete m_pCollBody[i];
		m_pCollBody[i] = nullptr;
	}
}

void CBubbleDragon::Init()
{
	m_fSpeed = 200.f;
	m_eMType = M_TYPE::BUBBLE_DRAGON;
	srand(time(NULL));
	m_eType = OBJ_TYPE::MONSTER;
	Vec2 vScale = Vec2(0, 0);
	Vec2 vOffset = Vec2(0, 0);
	
	for (int i = 0; i < 4; ++i)
	{
		m_pCollBody[i] = new CCollider;
		m_pCollBody[i]->SetOwner(this);
	}

	if (m_eDir == DIR::RIGHT)
	{
		m_vScale = Vec2(100, 100);
		vScale = Vec2(170, 100);
		vOffset = Vec2(140, 95);
	}
	else if (m_eDir == DIR::LEFT)
	{
		m_vScale = Vec2(390, 140);
		m_vOffset = Vec2(140, 90);

		m_pCollBody[0]->SetScale(Vec2(50,220));
		m_pCollBody[0]->SetOffset(Vec2(-35,50));

		m_pCollBody[1]->SetScale(Vec2(50, 200));
		m_pCollBody[1]->SetOffset(Vec2(230, 0));

		m_pCollBody[2]->SetScale(Vec2(230, 200));
		m_pCollBody[2]->SetOffset(Vec2(110, -34));

		m_pCollBody[3]->SetScale(Vec2(230, 200));
		m_pCollBody[3]->SetOffset(Vec2(230, 60));

		vScale = Vec2(140, 90);
		vOffset = Vec2(-150, 115);
	}
	m_pCollHead = new CCollider;
	m_pCollHead->SetOwner(this);
	m_pCollHead->SetOffset(vOffset);
	m_pCollHead->SetScale(vScale);

	m_iMaxHP = 5;
	m_iHP = m_iMaxHP;

	m_vRealPos = m_vPos;
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pTex = TEX_LOAD(L"BubbleDragon", L"Image\\BubbleDragon.bmp");
	m_tSize = tSize(181, 89);

	AniationInit();

	if(m_eDir == DIR::RIGHT)
		m_pAnim->PlayAnim(L"R_Idle", true);
	else if(m_eDir == DIR::LEFT)
		m_pAnim->PlayAnim(L"L_Idle", true);
	
	CCamObj::Init();
}

int CBubbleDragon::update()
{
	
	/*if ((m_vRealPos.x - (m_tSize.x * 2.f)) <= 0.f)
		m_vRealPos.x = (m_pTex->GetWidth() / 2.f);
	
	else if ((m_vRealPos.x + (m_tSize.x * 2.f)) >= CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE)
		m_vRealPos.x = (CStageMgr::GetInst()->GetTileSizeX() * TILE_SIZE) - (m_pTex->GetWidth() / 2.f);*/

	if (m_iHP > 0)
	{
		if (!m_bState)
		{
			float fWidth = m_pCollHead->GetRealPos().x - ((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().x;
			//float fHeight = powf(((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().y - m_vRealPos.y, 2.f);

			//float fLength = sqrt(fWidth + fHeight);

			if (fWidth < 50.f)
			{
				FP = &CBubbleDragon::RightWalking;
				
			}
			else if (fWidth <= 500.f)
			{
				FP = &CBubbleDragon::BubbleAttack;
			}
			else if (fWidth < 700.f)
			{
				FP = &CBubbleDragon::LeftWalking;
			}
			else 
				FP = &CBubbleDragon::Idle;
		}
		(this->*FP)();

		m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

		m_pAnim->update();
		Damage(false);
		CCamObj::update();

		for (int i = 0; i < 4; ++i)
			m_pCollBody[i]->Update();

		m_pCollHead->Update();
	}
	else
	{
		m_fCoinTime += DT;

		if (m_fCoinTime > 0.2f)
		{
			int iSpeed = rand() % 301;
			int iMinSpeed = rand() % 601;
			m_fCoinTime = 0.f;
			CCoin* pObj = new CCoin((COIN_TYPE)(rand() % 6));
			pObj->SetPos(m_vRealPos);
			pObj->SetSpeed(iSpeed - iMinSpeed);
			pObj->SetJump(-(rand() % 201 + 600));
			pObj->Init();
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::DROP].push_back(pObj);
		}



		m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
		Damage(true);

		if (m_fDeathTime == 0.f)
			CSoundMgr::GetInst()->Play(L"Explode",false);
		m_fDeathTime += DT;
		if (m_fDeathTime > 3.f)
			return INT_MAX;
	}

	

	return 0;
}

void CBubbleDragon::render(HDC _dc)
{
	m_pAnim->render(_dc);
	if (m_iHP > 0)
	{
		m_pCollHead->Render(_dc);
		for (int i = 0; i < 4; ++i)
			m_pCollBody[i]->Render(_dc);
		CCamObj::render(_dc);
	}
}

DIR CBubbleDragon::OnCollisionEnter(CCollider* _mine, CCollider * _other)
{
	CDynamicObj::OnCollisionEnter(_mine, _other);
	if (_mine == m_pCollHead && _other->GetOwner()->GetType() == OBJ_TYPE::SKILL && ((CSkill*)_other->GetOwner())->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		m_bDamage = true;
		m_iHP -= 1;
	}

	return DIR::NONE;
}

void CBubbleDragon::OnCollision(CCollider * _other)
{
	CDynamicObj::OnCollision(_other);
}

void CBubbleDragon::AniationInit()
{
	m_pAnim->AddAnimation(L"R_Idle", m_pTex, RECT{ 0,m_tSize.y * 0,m_tSize.x,m_tSize.y }, 12, 0.2f);
	m_pAnim->AddAnimation(L"L_Idle", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 12, 0.2f);
	m_pAnim->AddAnimation(L"R_Walk", m_pTex, RECT{ 0,m_tSize.y * 2,m_tSize.x,m_tSize.y }, 6, 0.2f);
	m_pAnim->AddAnimation(L"L_Walk", m_pTex, RECT{ 0,m_tSize.y * 3,m_tSize.x,m_tSize.y }, 6, 0.2f);
	m_pAnim->AddAnimation(L"R_BackWalk", m_pTex, RECT{ 0,m_tSize.y * 4,m_tSize.x,m_tSize.y }, 6, 0.2f);
	m_pAnim->AddAnimation(L"L_BackWalk", m_pTex, RECT{ 0,m_tSize.y * 5,m_tSize.x,m_tSize.y }, 6, 0.2f);
	m_pAnim->AddAnimation(L"R_AttackReady", m_pTex, RECT{ 0,m_tSize.y * 6,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_AttackReady", m_pTex, RECT{ 0,m_tSize.y * 7,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"R_Attack", m_pTex, RECT{ 0,m_tSize.y * 8,m_tSize.x,m_tSize.y }, 1, 0.2f);
	m_pAnim->AddAnimation(L"L_Attack", m_pTex, RECT{ 0,m_tSize.y * 9,m_tSize.x,m_tSize.y }, 1, 0.2f);
}

void CBubbleDragon::Damage(bool _bRepeat)
{
	if (m_bDamage)
	{
		if (m_fTime <= 0.05f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 0);
		else if (m_fTime <= 0.10f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 1);
		else if (m_fTime <= 0.15f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 2);
		else if (m_fTime <= 0.20f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 3);
		else if (m_fTime <= 0.25f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 4);
		else if (m_fTime <= 0.30f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 5);
		else if (m_fTime <= 0.35f)
			m_pTex = CDCMgr::GetInst()->GetTexture(M_TYPE::BUBBLE_DRAGON, 6);
		else if(!_bRepeat)
		{
			m_pTex = TEX_LOAD(L"BubbleDragon", L"Image\\BubbleDragon.bmp");
			m_fTime = 0.f;
			m_bDamage = false;
		}
		else
		{
			Vec2 vPos = Vec2(rand() % 201, (rand() % 201) - 50);
			CreateEffect(m_vRealPos + vPos,TEX_LOAD(L"Explosion",L"Image\\Explosion.bmp"),5);
			if(m_eDir == DIR::RIGHT)
				m_pAnim->PlayAnim(L"R_Attack",false);
			else if(m_eDir == DIR::LEFT)
				m_pAnim->PlayAnim(L"L_Attack", false);
			m_fTime = 0.f;
		}
		m_fTime += DT;
	}
}

void CBubbleDragon::LeftWalking()
{
	if (m_pAnim->GetCurKey() == L"L_Walk" &&m_pAnim->GetCurFinish() == true)
	{
		m_fStateTime += DT;
		if (m_fStateTime > 3.f)
		{
			m_pAnim->SetCurFinish(false);
			m_fStateTime = 0.f;
			m_bState = false;
		}
		return;
	}

	m_vRealPos.x -= m_fSpeed * DT;
	
	if (!m_bState)
	{
		m_pAnim->PlayAnim(L"L_Walk", false);
		m_pAnim->ReStartAnim();
	}
	m_bState = true;
}

void CBubbleDragon::RightWalking()
{
	if (m_pAnim->GetCurKey() == L"L_BackWalk" && m_pAnim->GetCurFinish() == true)
	{
		m_fStateTime += DT;
		if (m_fStateTime > 3.f)
		{
			m_pAnim->SetCurFinish(false);
			m_fStateTime = 0.f;
			m_bState = false;
		}
		return;
	}

	m_vRealPos.x += m_fSpeed * DT;

	int iX = (int)(m_pColl->GetRealPos().x - m_vOffset.x + m_vScale.x / 2.f) / TILE_SIZE;
	int iY = (int)(m_pColl->GetRealPos().y - m_vOffset.y) / TILE_SIZE;

	iX += 3;

	if(((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iX + iY * CStageMgr::GetInst()->GetTileSizeX()])->GetTileType() == TILE_TYPE::COLL)
		m_vRealPos.x -= m_fSpeed * DT;

	m_vCollPos = ((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][iX + iY * CStageMgr::GetInst()->GetTileSizeX()])->GetPos();

	if (!m_bState)
	{
		
		m_pAnim->PlayAnim(L"L_BackWalk", false);
		m_pAnim->ReStartAnim();
	}
	m_bState = true;
}

void CBubbleDragon::BubbleAttack()
{
	m_fStateTime += DT;

	if (m_iNum >= 3)
	{
		if (m_fStateTime > 1.f)
		{
			m_fStateTime = 0.f;
			m_iNum = 0;
			m_bState = false;
		}
	}
	else
	{
		m_pAnim->PlayAnim(L"L_Attack", false);
		m_bState = true;
		if (m_fStateTime >= 0.5f)
		{
			m_fStateTime = 0.f;
			++m_iNum;
			CBubbleAttack* pBubble = new CBubbleAttack;
			pBubble->SetRealPos(m_pCollHead->GetRealPos());
			pBubble->SetOwner(this);
			pBubble->SetDir(DIR::LEFT);
			pBubble->Init();
			OBJ[(UINT)OBJ_TYPE::SKILL].push_back(pBubble);
		}
	}
}

void CBubbleDragon::Idle()
{
	m_pAnim->PlayAnim(L"L_Idle",true);
}
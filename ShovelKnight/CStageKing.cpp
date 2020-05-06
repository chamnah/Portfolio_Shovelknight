#include "stdafx.h"
#include "CStageKing.h"
#include "CBlock.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CBoss.h"
#include "CPlayer.h"
#include "CCore.h"
#include "CGameMgr.h"
#include "CTimeMgr.h"
#include "CTextUI.h"
#include "CPurpleFire.h"
#include "CHpUI.h"
#include "CImageObj.h"
#include "CDCMgr.h"
#include "CShieldKnight.h"
#include "CStageMgr.h"
#include "SoundMgr.h"
#include "CCamMgr.h"

CStageKing::CStageKing()
	:m_bEvent(false)
	, m_bExit(false)
	, m_fTime(0.f)
	, m_vBoss{}
	, m_eCourse(STAGE_COURSE::ONE)
	, m_iNum(0)
	, m_fAccTime(0.f)
	, m_bDeath(false)
	, m_bFirst(false)
{
}

CStageKing::~CStageKing()
{
}

void CStageKing::Init()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			CBlock* pBlock = new CBlock;
			pBlock->SetPos(j * TILE_SIZE + 32, (i * TILE_SIZE + 800.f) + CCore::GetInst()->GetResolution().y + 32);
			pBlock->SetRealPos(j * TILE_SIZE + 32, (i * TILE_SIZE + 800.f) + CCore::GetInst()->GetResolution().y + 32);
			pBlock->SetScale(Vec2(TILE_SIZE, TILE_SIZE));
			pBlock->SetOffset(Vec2(0, 0));
			pBlock->SetTexture(TEX_LOAD(L"BlackBlock", L"Image\\BlackBlock.bmp"));
			pBlock->SetEffectTexture(TEX_LOAD(L"BlackBlockDeath", L"Image\\BlackBlockDeath.bmp"));
			pBlock->Init();
			m_vObj[(UINT)OBJ_TYPE::BLOCK].push_back(pBlock);
		}
	}

	Vec2 vPos = CCamMgr::GetInst()->GetRealPos(0.f,1700.f);

	((CPlayer*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetKing(true);
	((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos(Vec2(m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().x, m_vObj[(UINT)OBJ_TYPE::PLAYER][0]->GetPos().y + CCore::GetInst()->GetResolution().y));

	CBoss* pBoss = new CBoss;
	pBoss->SetRealPos(700,400);
	pBoss->SetPos(200, 200);
	pBoss->SetScale(Vec2(100,100));
	pBoss->Init();
	m_vObj[(UINT)OBJ_TYPE::MONSTER].push_back(pBoss);

	CHpUI* pHP = nullptr;

	for (int i = 0; i < (((CDynamicObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetMaxHP() / 2); ++i)
	{
		pHP = new CHpUI;
		pHP->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"M_Life", L"Image\\M_Life.bmp"));
		pHP->SetPos(1360 + i * 35, 33);
		pHP->SetScale(Vec2(32, 32));
		((CUI*)m_vObj[(UINT)OBJ_TYPE::UI][0])->AddChildUI(UI_TYPE::BOSS_HP, pHP);
	}

	m_bEvent = false;
	m_bExit = false;
	m_fTime = 0.f; 
	m_eCourse = STAGE_COURSE::ONE;
	m_iNum = 0;
	m_fAccTime = 0.f;

	CSoundMgr::GetInst()->PlayBGM(L"BGM_03");
}
int CStageKing::Progress()
{
	if (CGameStage::Progress())
	{
		if (m_eCourse == STAGE_COURSE::ONE && ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().x > 600.f)
		{
			((CPlayer*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetEvent(true);
			m_eCourse = STAGE_COURSE::TWO;
			m_fY = CCamMgr::GetInst()->GetLook().y;
		}

		if (m_eCourse == STAGE_COURSE::TWO && ((CPlayer*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetEvent() == false)
		{
			m_fTime += DT;

			if (m_fTime >= 5.f)
			{
				m_bExit = true;
				m_fTime = 5.f;

				CUI* pObj = new CUI;
				pObj->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"TaskingBox", L"Image\\TaskingBox.bmp"));
				pObj->SetPos(0, 0);
				m_vObj[(UINT)OBJ_TYPE::UI].push_back(pObj);

				CUI* pFace = new CUI;
				pFace->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"BossFace", L"Image\\BossFace.bmp"));
				pFace->SetPos(30, 40);
				pObj->AddChildUI(UI_TYPE::NONE, pFace);

				*((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetFSM() = BOSS_FSM::WINDOW_IDLE;

				CTextUI* pText = new CTextUI;
				pText->SetPos(Vec2(240, 70));
				pText->SetFace(pFace);
				pText->AddText(L"이제 그만 집에 돌아가자", TEX_LOAD(L"Face1", L"Image\\Face1.bmp"));
				pText->AddText(L"싫어", TEX_LOAD(L"BossFace", L"Image\\BossFace.bmp"));
				pText->AddText(L"안 갈꺼야!!", TEX_LOAD(L"BossFace", L"Image\\BossFace.bmp"));
				pText->SetBuy(false);
				pText->SetBoss(((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetFSM());
				pObj->AddChildUI(UI_TYPE::NONE, pText);

			}
			float fY = m_fY + (m_fTime  * (((CCore::GetInst()->GetResolution().y / 2.f) - m_fY) / 5.f));
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, fY);
			if (m_fTime >= 5.f)
			{
				CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, (CCore::GetInst()->GetResolution().y / 2.f));
				m_eCourse = STAGE_COURSE::THREE;
				m_fTime = 0.f;
				m_fY = CCamMgr::GetInst()->GetLook().y;
				m_vBoss.y = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetRealPos().y;
			}
		}
		else if (m_eCourse == STAGE_COURSE::THREE && *((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetFSM() == BOSS_FSM::IDLE)
		{
			m_fTime += DT;

			if (m_fTime >= 5.f)
			{
				m_eCourse = STAGE_COURSE::FOUR;
				m_fTime = 5.f;
			}
			float fY = (m_fTime  * ((((CCore::GetInst()->GetResolution().y)* 1.9f) - m_fY) / 5.f));
			((CCamObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->SetRealPos(((CCamObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetRealPos().x, m_vBoss.y + fY);
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, m_fY + fY);
		}
		else if (m_eCourse == STAGE_COURSE::FOUR && ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetDeath() == 0)
		{
			*((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetFSM() = BOSS_FSM::FIGHT;
			m_fY = CCamMgr::GetInst()->GetLook().y;
			m_fTime = 0.f;
			m_vBoss = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetRealPos();
		}
		else if (m_eCourse == STAGE_COURSE::FOUR && ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetDeath())
		{
			m_bDeath = true;
			if (m_fTime == 0.f)
			{
				for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::BLOCK].size(); ++i)
					delete m_vObj[(UINT)OBJ_TYPE::BLOCK][i];

				m_vObj[(UINT)OBJ_TYPE::BLOCK].clear();

				for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::SKILL].size(); ++i)
					delete m_vObj[(UINT)OBJ_TYPE::SKILL][i];

					m_vObj[(UINT)OBJ_TYPE::SKILL].clear();

				for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::FIRE_BLOCK].size(); ++i)
					delete m_vObj[(UINT)OBJ_TYPE::FIRE_BLOCK][i];

				m_vObj[(UINT)OBJ_TYPE::FIRE_BLOCK].clear();
			}
			m_fTime += DT;

			if (m_fTime >= 5.f)
			{
				m_eCourse = STAGE_COURSE::FIVE;
				m_fTime = 5.f;
			}

			float fY = (m_fTime  * ((((CCore::GetInst()->GetResolution().y) / 2.f) - m_fY) / 5.f));
			float fBossX = m_vBoss.x + (m_fTime * (((CCore::GetInst()->GetResolution().x / 2.f) - m_vBoss.x) / 5.f));
			float fBossY = m_vBoss.y + (m_fTime * ((270.f - m_vBoss.y) / 5.f));

			((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->SetRealPos(fBossX, fBossY);
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, m_fY + fY);
		}
		else if (m_eCourse == STAGE_COURSE::FIVE)
		{
			if (!m_bEvent)
			{
				m_fTime = 0.f;
				LoadObj(L"Object\\Boss.obj");
				m_bEvent = true;
			}

			m_fTime += DT;

			if (m_fTime >= 2.f)
			{
				CCamMgr::GetInst()->SetShake(false);
				if (m_iNum >= 3)
				{
					for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::OBJECT].size(); ++i)
						delete m_vObj[(UINT)OBJ_TYPE::OBJECT][i];

					m_vObj[(UINT)OBJ_TYPE::OBJECT].clear();
					
					CSoundMgr::GetInst()->Play(L"GlassLarge", false);
					m_eCourse = STAGE_COURSE::SIX;
					((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->SetTransform(true);
					ChangeTile(L"Tile\\BossEnd.tile", CDCMgr::GetInst()->GetMapTex());
					m_fTime = 0.f;
					((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->SetRealPos(((CCamObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetRealPos().x,2176.f);
					return 0;
				}
				++m_iNum;
				m_fTime = 0.f;
				for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::OBJECT].size(); ++i)
					((CImageObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT][i])->SetIndex(m_iNum);
				if (m_iNum <= 2)
					CSoundMgr::GetInst()->Play(L"GlassSmall",false);
				else if(m_iNum == 3)
					CSoundMgr::GetInst()->Play(L"GlassMed", false);
			}
			else if (m_fTime >= 1.f)
				CCamMgr::GetInst()->SetShake(true);
		}
		else if (m_eCourse == STAGE_COURSE::SIX)
		{
			m_fTime += DT;

			if (m_fTime >= 2.f)
			{
				m_fTime = 0.f;
				m_eCourse = STAGE_COURSE::SEVEN;
				((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->SetAlpha(true);
			}
		}
		// 현재 땅으로 떨어져서 사라지는 현상이 발생 되는데 그 원인은 카메라 이동 후에 그 타일을 업데이트 안시켜주기 때문이다.
		else if (m_eCourse == STAGE_COURSE::SEVEN)
		{
			m_fTime += DT;
			m_fAccTime += DT;
			if (m_fAccTime > 6.f)
			{
				m_eCourse = STAGE_COURSE::Eight;

				CShieldKnight* pShield = new CShieldKnight;
				Vec2 vPos = ((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetRealPos();
				pShield->SetRealPos(vPos.x + 15, vPos.y + 30);
				pShield->Init();
				m_vObj[(UINT)OBJ_TYPE::SHIELD].push_back(pShield);

				CSoundMgr::GetInst()->Play(L"End",false);

				delete m_vObj[(UINT)OBJ_TYPE::MONSTER][0];
				m_vObj[(UINT)OBJ_TYPE::MONSTER].clear();
				
				m_fTime = 0.f;
				m_fAccTime = 0.f;
				ChangeTile(L"Tile\\BossEnd.tile", TEX_LOAD(L"Level2",L"Image\\Level2.bmp"));
				return 0;
			}
			if (m_fAccTime > 3.f)
			{
				if (m_fTime >= 0.05f)
				{
					m_fTime = 0.f;
					((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->SetAlpha(!((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetAlpha());
				}
			}
			else
			{
				if (m_fTime >= 0.3f)
				{
					m_fTime = 0.f;
					((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->SetAlpha(!((CBoss*)m_vObj[(UINT)OBJ_TYPE::MONSTER][0])->GetAlpha());
				}
			}
		}
		else if (m_eCourse == STAGE_COURSE::Eight)
		{
			m_fTime += DT;
			if (m_fTime >= 4.f)
			{
				m_eCourse = STAGE_COURSE::Nine;
				m_fTime = 0.f;
				CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, (CStageMgr::GetInst()->GetTileSizeY() * TILE_SIZE) - CCore::GetInst()->GetResolution().y / 2.f);
			}
		}
		else if (m_eCourse == STAGE_COURSE::Nine)
		{
			if (((CPlayer*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetEnd())
			{
				if (!m_bFirst)
				{
					m_eCourse = STAGE_COURSE::TEN;
					m_bFirst = true;
					CUI* pObj = new CUI;
					pObj->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"TaskingBox", L"Image\\TaskingBox.bmp"));
					pObj->SetPos(0, 0);
					m_vObj[(UINT)OBJ_TYPE::UI].push_back(pObj);

					CUI* pFace = new CUI;
					pFace->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Face1", L"Image\\Face1.bmp"));
					pFace->SetPos(30, 40);
					pObj->AddChildUI(UI_TYPE::NONE, pFace);

					CTextUI* pText = new CTextUI;
					pText->SetPos(Vec2(240, 70));
					pText->SetFace(pFace);
					pText->AddText(L"발표",TEX_LOAD(L"Face1", L"Image\\Face1.bmp"));
					pText->AddText(L"끝!!", TEX_LOAD(L"Face2", L"Image\\Face2.bmp"));
					pText->SetBuy(false);
					pObj->AddChildUI(UI_TYPE::NONE, pText);
				}
			}
			/*m_fTime += DT;

			if (m_fTime >= 5.f)
			{
				m_fTime = 5.f;
			}
			float fY = (m_fTime  * ((((CCore::GetInst()->GetResolution().y)* 2.0f) - (CCore::GetInst()->GetResolution().y / 2.f)) / 5.f));
			CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetLook().x, (CCore::GetInst()->GetResolution().y / 2.f) + fY);*/
		}
		else if (m_eCourse == STAGE_COURSE::TEN)
		{
			if (m_vObj[(UINT)OBJ_TYPE::UI].size() == 1)
			{
				CStageMgr::GetInst()->SetFade(true);
			}
		}
		if (((int)m_eCourse >= (int)STAGE_COURSE::FOUR) && (int)m_eCourse <= (int)STAGE_COURSE::Eight)
		{
			if (m_bDeath)
			{
				for (UINT i = m_vObj[(UINT)OBJ_TYPE::TILE].size() - 51; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
				{
					m_vObj[(UINT)OBJ_TYPE::TILE][i]->update();
				}
			}
		}
	}
	return 0;
}

void CStageKing::Enter()
{
	CBackGroundObj* pBack = new CBackGroundObj;
	pBack->SetTexture(TEX_LOAD(L"Back", L"Image\\BackGround.bmp"));
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pBack);

	StageMoveInit(L"Tile\\Boss.tile");
}
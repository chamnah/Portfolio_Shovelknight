#include "stdafx.h"
#include "CArmorer.h"
#include "CAnimator.h"
#include "CCamMgr.h"
#include "CCollider.h"
#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CUI.h"
#include "CStageMgr.h"
#include "CTextUI.h"
CArmorer::CArmorer()
	:m_bView(false)
{
	m_pAnim = new CAnimator(this);
}

CArmorer::~CArmorer()
{
	if (m_pAnim != nullptr)
		delete m_pAnim;

	m_pAnim = nullptr;
}

void CArmorer::Init()
{
	m_eType = OBJ_TYPE::OBJECT;
	m_tSize = tSize(69,67);
	m_vOffset = Vec2(-50,30);
	m_vScale = Vec2(130,150);
	m_vRealPos = m_vPos;
	CCamObj::Init();
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Armorer",L"Image\\Armorer.bmp");
	m_pAnim->AddAnimation(L"R_Making", m_pTex, RECT{ 0,m_tSize.y * 0,m_tSize.x,m_tSize.y }, 5, 0.2f);
	m_pAnim->AddAnimation(L"R_Idle", m_pTex, RECT{0,m_tSize.y * 1,m_tSize.x,m_tSize.y},2,0.5f);
	m_pAnim->AddAnimation(L"R_Complete", m_pTex, RECT{ 0,m_tSize.y * 2,m_tSize.x,m_tSize.y }, 7, 0.2f);
	m_pAnim->PlayAnim(L"R_Idle",true);
}

int CArmorer::update()
{
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);
	m_pAnim->update();
	m_pColl->Update();
	return 0;
}

void CArmorer::render(HDC _dc)
{
	m_pAnim->render(_dc);
	m_pColl->Render(_dc);
}

void CArmorer::OnCollision(CCollider * _other)
{
	if (m_bView)
		return;
	if (_other->GetOwner()->GetType() == OBJ_TYPE::PLAYER)
	{
		if (KEY(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
		{
			m_bView = true;
			CUI* pObj = new CUI;
			pObj->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"TaskingBox", L"Image\\TaskingBox.bmp"));
			pObj->SetPos(0, 0);
			CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::UI].push_back(pObj);

			CUI* pFace = new CUI;
			pFace->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Face", L"Image\\Face.bmp"));
			pFace->SetPos(30, 40);
			pObj->AddChildUI(UI_TYPE::NONE,pFace);

			CTextUI* pText = new CTextUI;
			pText->SetPos(Vec2(240, 70));
			pText->AddText(L"ÀÌºÁ ÀþÀºÀÌ µ· Á» °¡Áö°í ÀÖ³ª?");
			pText->AddText(L"½Î°Ô ÁÙ°Ô °ñ¶óºÁ");
			pText->SetBuy(true);
			pText->SetNPC(this);
			pObj->AddChildUI(UI_TYPE::NONE, pText);
		}
	}
}
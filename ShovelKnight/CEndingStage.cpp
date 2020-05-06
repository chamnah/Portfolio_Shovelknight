#include "stdafx.h"
#include "CEndingStage.h"
#include "CBackGroundObj.h"
#include "CResMgr.h"
#include "CTempObj.h"
#include "SoundMgr.h"
#include "CTimeMgr.h"
CEndingStage::CEndingStage()
	:m_fTime(0.f)
	, m_bFirst(false)
{
}

CEndingStage::~CEndingStage()
{
}

int CEndingStage::Progress()
{

	m_fTime += DT;

	if (m_fTime > 3.f)
	{
		m_fTime = 0.f;
		if (!m_bFirst)
		{
			m_bFirst = true;
			CTempObj* pTemp = new CTempObj;
			pTemp->SetPos(800, 300);
			pTemp->SetTexture(TEX_LOAD(L"EndText", L"Image\\EndText.bmp"));
			pTemp->AddAnim(TEX_LOAD(L"EndText", L"Image\\EndText.bmp"), tSize(149, 114), 100, 0.1f, Vec2(0, 0), Vec2(2, 0));
			pTemp->Play(false);
			m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pTemp);
		}
	}

	CGameStage::Progress();
	return 0;
}

void CEndingStage::Enter()
{
	CTempObj* pTemp = new CTempObj;
	pTemp->SetPos(800,450);
	pTemp->SetTexture(TEX_LOAD(L"Ending", L"Image\\Ending11.bmp"));
	pTemp->AddAnim(TEX_LOAD(L"Ending",L"Image\\Ending11.bmp"),tSize(400,225),2,0.1f, Vec2(0, 0), Vec2(0, 0));
	pTemp->Play(true);
	m_vObj[(UINT)OBJ_TYPE::BACK].push_back(pTemp);

	

	CSoundMgr::GetInst()->PlayBGM(L"BGM_04");
}
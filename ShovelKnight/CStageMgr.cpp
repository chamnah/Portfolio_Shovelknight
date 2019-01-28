#include "stdafx.h"
#include "CStageMgr.h"
#include "CStageLogo.h"
#include "CStageStart.h"
#include "CStageTool.h"
#include "CCore.h"

CStageMgr::CStageMgr()
{
}

CStageMgr::~CStageMgr()
{
	for (UINT i = 0; i < (UINT)STAGE::END; ++i)
	{
		delete m_pArrStage[i];
	}
}

void CStageMgr::Init()
{
	m_pArrStage[(UINT)STAGE::LOGO] = new CStageLogo;
	m_pArrStage[(UINT)STAGE::START] = new CStageStart;
	m_pArrStage[(UINT)STAGE::TOOL] = new CStageTool;
	m_pCurStage = m_pArrStage[(UINT)STAGE::LOGO];
	m_eCurState = STAGE::LOGO;
	m_pCurStage->Enter();
}

int CStageMgr::Update()
{
	return m_pCurStage->Progress();
}

void CStageMgr::Render(HDC _dc)
{
	m_pCurStage->Render(_dc);
}

void CStageMgr::ChangeStage(STAGE _eStage)
{
	if (m_eCurState == _eStage)
		return;

	m_eCurState = _eStage;
	m_pCurStage->Exit();
	m_pCurStage = m_pArrStage[(UINT)_eStage];
	m_pCurStage->Enter();
}
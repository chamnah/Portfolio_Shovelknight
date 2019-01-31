#include "stdafx.h"
#include "CSkillMgr.h"
#include "CBasic.h"

CSkillMgr::CSkillMgr()
{
}


CSkillMgr::~CSkillMgr()
{
}

int CSkillMgr::Update()
{
	list<CSkill*>::iterator iter = m_listSkill.begin();
	for (; iter != m_listSkill.end();)
	{
		if ((*iter)->update() == INT_MAX)
		{
			delete *iter;
			iter = m_listSkill.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	return 0;
}

void CSkillMgr::Render(HDC _dc)
{
	list<CSkill*>::iterator iter = m_listSkill.begin();
	for (; iter != m_listSkill.end(); ++iter)
	{
		(*iter)->render(_dc);
	}
}

void CSkillMgr::AddSkill(SKILL_TYPE _eType)
{
	if (_eType == SKILL_TYPE::BASIC)
	{
		CSkill* pSkill = new CBasic;
		pSkill->Init();
		m_listSkill.push_back(pSkill);
	}
}
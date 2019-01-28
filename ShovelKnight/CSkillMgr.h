
class CSkill;
class CSkillMgr
{
	SINGLE(CSkillMgr);

private:
	list<CSkill*> m_listSkill;

public:
	int  Update();
	void Render(HDC _dc);

public:
	void AddSkill(SKILL_TYPE _eType);

};
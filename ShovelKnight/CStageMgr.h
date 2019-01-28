#pragma once
class CStage;
class CStageMgr
{
	SINGLE(CStageMgr)

private:
	CStage* m_pArrStage[(UINT)STAGE::END];
	CStage* m_pCurStage;
	STAGE   m_eCurState;

public:
	void Init();
	int Update();
	void Render(HDC _dc);
	void ChangeStage(STAGE _eStage);
	CStage* const GetCurStage(){ return m_pCurStage; }
	STAGE GetCurState() { return m_eCurState; }
};
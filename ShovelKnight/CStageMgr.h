#pragma once
class CStage;
class CObj;
class CStageMgr
{
	SINGLE(CStageMgr)

private:
	CStage* m_pArrStage[(UINT)STAGE::END];
	CStage* m_pCurStage;
	STAGE   m_eCurState;
	UINT    m_iTileSizeX;
	UINT    m_iTileSizeY;

public:
	void Init();
	int Update();
	void Render(HDC _dc);
	void ChangeStage(STAGE _eStage);
	CStage* const GetCurStage(){ return m_pCurStage; }
	vector<vector<CObj*>>& GetObjVector();
	void EraseStageObj(OBJ_TYPE _Type,CObj* _pObj);
	STAGE GetCurState() { return m_eCurState; }

public:
	void SetTileSizeX(int _X) { m_iTileSizeX = _X; }
	void SetTileSizeY(int _Y) { m_iTileSizeY = _Y; }
	UINT GetTileSizeX() { return m_iTileSizeX; }
	UINT GetTileSizeY() { return m_iTileSizeY; }
};
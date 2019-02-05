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
	tSize    m_iPreTileSize;
	UINT    m_iTileSizeX;
	UINT    m_iTileSizeY;
	HDC		m_hDC;
	HBITMAP m_hBit;
	bool    m_arrDelCheck[(UINT)OBJ_TYPE::END];

public:
	void Init();
	int Update();
	void Render(HDC _dc);
	void ChangeStage(STAGE _eStage,DIR _eDir = DIR::NONE);
	CStage* const GetCurStage(){ return m_pCurStage; }
	vector<vector<CObj*>>& GetObjVector();
	void EraseStageObj(OBJ_TYPE _Type,CObj* _pObj);
	STAGE GetCurState() { return m_eCurState; }
	void CreateBackGround();
	void AddBackGround(DIR _iDir);
	void ArriveBackGround(DIR _eDir);
	void TileRender(HDC _dc);
	void CopyRender(HDC _dc, Vec2& _vPos,Vec2& _vTexPos);
	void CopyStage(STAGE _eStage,OBJ_TYPE _eType);
public:
	void SetTileSizeX(int _X) { m_iTileSizeX = _X; }
	void SetTileSizeY(int _Y) { m_iTileSizeY = _Y; }
	UINT GetTileSizeX() { return m_iTileSizeX; }
	UINT GetTileSizeY() { return m_iTileSizeY; }
	tSize GetPreTileSize() { return m_iPreTileSize; }
	HDC  GetDC() { return m_hDC; }
	void SetStageDir(STAGE _eStage,DIR _eDir);

	void ClearCheck();
	void AddCheck(OBJ_TYPE _eType) { m_arrDelCheck[(UINT)_eType] = true; }
	bool GetCheck(int _iType) { return m_arrDelCheck[_iType]; }
	bool GetCheck(OBJ_TYPE _eType) { return m_arrDelCheck[(UINT)_eType]; }
};
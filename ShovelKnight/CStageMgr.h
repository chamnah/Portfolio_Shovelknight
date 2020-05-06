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
	bool    m_bChange;
	STAGE   m_eChange;
	bool    m_bHidden;
	bool    m_bHiddenFirst;
	DIR     m_eDir;
	float   m_fTime;
	int     m_iNext;
	OBJ_TYPE m_eHiddenType;
	wstring m_wcsPath;
	wstring m_wcsPathTwo;
	bool    m_bFade;
	bool    m_bFadeOut;
	float   m_fFadeTime;
	bool    m_bFirst;
	float   m_fOutTime;

public:
	void Init();
	int Update();
	void Render(HDC _dc);
	void ChangeStage(STAGE _eStage);
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
	void ResetStage();

public:
	void SetTileSizeX(int _X) { m_iTileSizeX = _X; }
	void SetTileSizeY(int _Y) { m_iTileSizeY = _Y; }
	UINT GetTileSizeX() { return m_iTileSizeX; }
	UINT GetTileSizeY() { return m_iTileSizeY; }
	tSize GetPreTileSize() { return m_iPreTileSize; }
	HDC  GetDC() { return m_hDC; }
	void SetStageDir(STAGE _eStage,DIR _eDir);
	void SetPreTileSize() { m_iPreTileSize = tSize(m_iTileSizeX, m_iTileSizeY); }
	void SetIsChange(bool _bChange) { m_bChange = _bChange; }
	bool GetIsChange() { return m_bChange; }
	void SetChange(STAGE _eChange) { m_eChange = _eChange; }
	STAGE GetChange() { return m_eChange; }
	void SetHidden(bool _bHidden) { m_bHidden = _bHidden; }
	bool GetHidden() { return m_bHidden; }
	void SetDir(DIR _eDir) { m_eDir = _eDir; }
	void SetHiddenType(OBJ_TYPE _eType) { m_eHiddenType = _eType; }
	OBJ_TYPE GetHiddenType() { return m_eHiddenType; }
	void SetPath(const wstring& _wcsPath) { m_wcsPath = _wcsPath; }
	void SetPathTwo(const wstring& _wcsPathTwo) { m_wcsPathTwo = _wcsPathTwo; }
	void SetFade(bool _bFade) { m_bFade = _bFade; }

	void R_HiddenRemove();
	void L_HiddenRemove();

	void ReStart();
	void ClearCheck();
	void AddCheck(OBJ_TYPE _eType) { m_arrDelCheck[(UINT)_eType] = true; }
	bool GetCheck(int _iType) { return m_arrDelCheck[_iType]; }
	bool GetCheck(OBJ_TYPE _eType) { return m_arrDelCheck[(UINT)_eType]; }
};
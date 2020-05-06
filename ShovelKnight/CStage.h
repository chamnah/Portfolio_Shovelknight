#pragma once
class CUI;
class CHiddenTile;
class CStage
{
protected:
	vector<vector<CObj*>> m_vObj;
	Vec2 m_vPos;
	Vec2 m_vStartPos;
	vector<vector<CHiddenTile*>> m_vecHidden;

public:
	virtual int Progress() = 0;
	virtual void Enter() = 0;
	virtual void Exit();
	void ReSetExit();
	virtual void Render(HDC _hdc);
	virtual int  Update();
	virtual void ClearObj(int _iObj);
	virtual void CreateTile(int iSizeX, int iSizeY, int iTileSize);
	virtual void ChangeTile(int iSizeX, int iSizeY);
	virtual int LoadTile(wstring _strPath = L"");
	virtual int LoadObj(wstring _strPath = L"") { return 0; }

protected:
	virtual void TileDCRender(HDC _dc);

public:
	vector<vector<CObj*>>& GetObjVector() { return m_vObj; };
	vector<CObj*>& GetObj(OBJ_TYPE _eType) { return m_vObj[(UINT)_eType]; }

public:
	void UICheck();
	bool UICheck_PreOrder(CUI* _pObj);
	void MouseOnOff(CUI* _pUI);
	void MouseLBTN(CUI* _pUI);
	void ArriveTile();
	void ReStart();

public:
	CStage();
	virtual ~CStage();
};
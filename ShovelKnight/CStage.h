#pragma once
class CUI;
class CStage
{
protected:
	vector<vector<CObj*>> m_vObj;

protected:
	UINT				  m_iTileSizeX;
	UINT				  m_iTileSizeY;

public:
	virtual int Progress() = 0;
	virtual void Enter() = 0;
	virtual void Exit();
	virtual void Render(HDC _hdc);
	virtual int  Update();
	virtual void ClearObj(int _iObj);
	virtual void CreateTile(int iSizeX, int iSizeY, int iTileSize);
	virtual void ChangeTile(int iSizeX, int iSizeY);
	virtual void LoadTile(wstring _strPath = L"");

protected:
	virtual void TileDCRender(HDC _dc);

	UINT		 GetTileSizeX() { return m_iTileSizeX; }
	UINT		 GetTileSizeY() { return m_iTileSizeY; }

public:
	void UICheck();
	bool UICheck_PreOrder(CUI* _pObj);
	void MouseOnOff(CUI* _pUI);
	void MouseLBTN(CUI* _pUI);

public:
	CStage();
	virtual ~CStage();
};
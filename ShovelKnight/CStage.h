#pragma once
class CUI;
class CStage
{
protected:
	vector<vector<CObj*>> m_vObj;
	Vec2 m_vPos;
	Vec2 m_vStartPos;

public:
	virtual int Progress() = 0;
	virtual void Enter() = 0;
	virtual void Exit();
	virtual void Render(HDC _hdc);
	virtual int  Update();
	virtual void ClearObj(int _iObj);
	virtual void CreateTile(int iSizeX, int iSizeY, int iTileSize);
	virtual void ChangeTile(int iSizeX, int iSizeY);
	virtual void LoadTile(wstring _strPath = L"", Vec2 vPos = {});

protected:
	virtual void TileDCRender(HDC _dc);

public:
	vector<vector<CObj*>>& GetObjVector() { return m_vObj; };

public:
	void UICheck();
	bool UICheck_PreOrder(CUI* _pObj);
	void MouseOnOff(CUI* _pUI);
	void MouseLBTN(CUI* _pUI);
	void ArriveTile();

public:
	CStage();
	virtual ~CStage();
};
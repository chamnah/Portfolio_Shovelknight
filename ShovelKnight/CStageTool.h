#pragma once
#include "CStage.h"

class CStageTool :
	public CStage
{
private:
	HMENU m_hMenu;
	int   m_iTileIdx;
	Vec2  m_vSelectBtn;
	POINT m_ptIdxPos;
	bool  m_bCollView;
	HFONT m_Font;
	int   m_iHiddeniIdx;

public:
	virtual int Progress();
	virtual void Enter();
	virtual void Exit();
	virtual void Render(HDC _hdc);

	void TilePicking();
	void TileCopy();
	void TileCopyPinking();
	void ObjPicking();
	void HiddenTilePicking();

	bool OnUI();

public:
	void SaveTile();
	void SaveObject();
	virtual int LoadObj(wstring _strPath = L"");


public:
	CStageTool();
	virtual ~CStageTool();
};
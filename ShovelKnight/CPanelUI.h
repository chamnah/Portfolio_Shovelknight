#pragma once
#include "CUI.h"
class CPanelUI :
	public CUI
{
private:
	HPEN   m_Pen;
	HBRUSH m_Brush;
	POINT  m_ptCurBtnSize;
	int    m_iCurPage;
	tRect  m_tPadding;
	CTexture* m_pBtnTex;
	vector<wstring> m_vecFile;

public:
	virtual void render(HDC _dc);

public:
	void NextPage();
	void PrevPage();

public:
	void Init();
	void SetBtn(CTexture* _pTex);
	void SetList();
	void VecFilePush(wstring _wstrFile) { m_vecFile.push_back(_wstrFile); }

public:
	CPanelUI();
	virtual ~CPanelUI();
};
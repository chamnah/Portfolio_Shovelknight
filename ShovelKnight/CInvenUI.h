#pragma once
#include "CUI.h"
class CItem;
class CInvenUI :
	public CUI
{
private:
	int m_iTab;
	vector<CItem> m_vecLeft;
	vector<CItem> m_vecRight;
	CTexture* m_pCursorTex;
	int       m_iCursorPos;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	CInvenUI();
	virtual ~CInvenUI();
};
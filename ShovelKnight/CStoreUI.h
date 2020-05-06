#pragma once
#include "CUI.h"
class CItem;
class CStoreUI :
	public CUI
{
private:
	vector<CItem*> m_vecStore;
	int m_iCursorPos;
	HFONT m_hFont;
	CTexture* m_pCursorTex;
	CObj*  m_pNPC;

public:
	virtual int update();
	virtual void render(HDC _dc);
	void SetArmorStore();
	void SetNPC(CObj* _pNPC) { m_pNPC = _pNPC; }


public:
	CStoreUI();
	virtual ~CStoreUI();
};
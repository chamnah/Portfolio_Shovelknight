#pragma once
#include "CUI.h"

class CTexture;
struct tText
{
	wstring wcsText;
	CTexture* pTex;
};

class CArmorer;
class CTextUI :
	public CUI
{
private:
	vector<tText> m_vecText;
	int m_iNext;
	int m_iText;
	HFONT m_hFont;
	float m_fTime;
	wstring m_wcsText;
	CArmorer* m_pNPC;
	bool m_bBuy;
	bool m_bSelect;
	bool m_bView;
	tSize m_tYesPos;
	tSize m_tNoPos;
	BOSS_FSM* m_pFSM;
	CUI*  m_pFace;


public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void AddText(const wstring& _wcsText, CTexture* _pTex = nullptr);
	void SetNPC(CArmorer* pObj) { m_pNPC = pObj; }
	void SetFace(CUI* _pUI) { m_pFace = _pUI; }
	void SetBuy(bool _bBuy) { m_bBuy = _bBuy; }
	void SetYesPos(const tSize& _tPos) { m_tYesPos = _tPos; }
	void SetNoPos(const tSize& _tPos) { m_tNoPos = _tPos; }
	void SetBoss(BOSS_FSM* _pFSM) { m_pFSM = _pFSM; }

public:
	CTextUI();
	virtual ~CTextUI();
};
#pragma once



class CGameMgr
{
	SINGLE(CGameMgr)

private:
	int m_iGold;
	vector<tSave> m_vecSaveStage;
	bool m_bDeath;
	bool m_bExit;
	ITEM_TYPE m_eItemType;
	bool m_bItemChange;

public:
	int Update();

public:
	void SetGold(int _iGold) { m_iGold = _iGold; }
	int  GetGold() { return m_iGold; }
	void SetDeath(bool _bDeath) { m_bDeath = _bDeath; }
	void AddGold(int _iGold) { m_iGold += _iGold; }
	void AddSaveStage(tSave _tSave) { m_vecSaveStage.push_back(_tSave); }
	tSave& LastSave() { return m_vecSaveStage.back(); }
	void SetExit(bool _bExit) { m_bExit = _bExit; }
	bool GetExit() { return m_bExit; }
	bool IsSaveStage(STAGE _eStage);
	ITEM_TYPE GetItemType() { return m_eItemType; }
	void SetItemType(ITEM_TYPE _eType) { m_eItemType = _eType; }
};
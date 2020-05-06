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
	EQUIP_TYPE m_eEquipType;
	bool m_bItemChange;
	bool m_bPlayerStop;
	int m_iEquipPrice[(UINT)EQUIP_TYPE::END];


public:
	int Update();

public:
	void SetGold(int _iGold) { m_iGold = _iGold; }
	int  GetGold() { return m_iGold; }
	void SetDeath(bool _bDeath) { m_bDeath = _bDeath; }
	void AddGold(int _iGold) { m_iGold += _iGold; }
	void AddSaveStage(tSave _tSave) { m_vecSaveStage.push_back(_tSave); }
	void ClearSaveStage() { m_vecSaveStage.clear(); }
	tSave& LastSave() { return m_vecSaveStage.back(); }
	void SetExit(bool _bExit) { m_bExit = _bExit; }
	bool GetExit() { return m_bExit; }
	bool IsSaveStage(STAGE _eStage);
	ITEM_TYPE GetItemType() { return m_eItemType; }
	void SetItemType(ITEM_TYPE _eType) { m_eItemType = _eType; }
	void SetPlayerStop(bool _bStop) { m_bPlayerStop = _bStop; }
	bool GetPlayerStop() { return m_bPlayerStop; }
	void SetEquipment(EQUIP_TYPE _eEquip) {	m_eEquipType = _eEquip;}
	EQUIP_TYPE GetEquipment() { return m_eEquipType; }
	void SetEquipSetting();
	void SetEquipPrice(int _iIdx, int _iPrice) { m_iEquipPrice[_iIdx] = _iPrice; }
	int GetEquipPrice(int _iIdx) { return m_iEquipPrice[_iIdx]; }
};
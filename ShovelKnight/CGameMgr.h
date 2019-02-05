#pragma once
class CGameMgr
{
SINGLE(CGameMgr)

private:
	int m_iGold;

public:
	void SetGold(int _iGold) { m_iGold = _iGold; }
	int  GetGold() { return m_iGold; }
};
#pragma once

struct tKey
{
	KEY_TYPE	eType;
	KEY_STATE	eState;
};

class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKey>	m_vecKey;
	Vec2			m_vMousePos;
	vector<KEY_TYPE>	m_vecCombo;
	vector<KEY_TYPE> m_vecComboView;
	float           m_fTime;
	bool			m_bInput;
	bool            m_bKey;

public:
	bool GetKeyState(KEY_TYPE _eType, KEY_STATE _eState)
	{
		if (_eState == m_vecKey[(UINT)_eType].eState)
			return true;
		else
			return false;
	};

	const Vec2& GetMousePos() { return m_vMousePos; }
	const vector<KEY_TYPE>& GetCombo() { return m_vecCombo; }
	void ComboClear() { m_vecCombo.clear(); }

	void Combo();

public:
	void init();
	void update();
	void render(HDC _hdc);
};
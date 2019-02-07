#pragma once
#include "CStage.h"

struct tMonster
{
	bool bItem;
	Vec2 vPos;
	bool bDeath;
	bool bExit;
	M_TYPE eType;

	tMonster()
		:bItem(true), vPos{},bDeath(false),bExit(true), eType(M_TYPE::END)
	{}
	tMonster(bool _bItem, Vec2 _vPos, bool _bDeath, bool _bExit, M_TYPE _eType)
		:bItem(_bItem),vPos(_vPos),bDeath(_bDeath),bExit(_bExit),eType(_eType)
	{}
};

class CStageMove;
class CGameStage :
	public CStage
{
private:
	bool m_bInven;
	bool m_bInvenOn;
	bool m_bInvenOff;
	

protected:
	vector<CStageMove*> m_vNextStage;
	vector<tMonster> m_vMonster;
	DIR  m_eDir;
	Vec2  m_vStart;
	Vec2  m_vPlayerPos;
	Vec2  m_vPlayerStart;
	float m_fAccTime;

public:
	virtual void Init() {}
	virtual int Progress();
	virtual void Render(HDC _hdc);

public:
	vector<tMonster>& GetMonster() { return m_vMonster; }
	void SetDir(DIR _eDir) { m_eDir = _eDir; }

public:
	void Regen();
	void KeyInput();
	void StageMoveInit(wstring _wstrFileName);
	void CamRange();
	void CreateMonster();

public:
	virtual void Exit();

public:
	CGameStage();
	virtual ~CGameStage();
};
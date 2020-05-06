#pragma once
#include "CStage.h"

struct tMonster
{
	bool bItem;
	Vec2 vPos;
	bool bDeath;
	bool bExit;
	M_TYPE eType;
	DIR  eDir;

	tMonster()
		:bItem(true), vPos{},bDeath(false),bExit(true), eType(M_TYPE::END), eDir(DIR::RIGHT)
	{}
	tMonster(bool _bItem, Vec2 _vPos, bool _bDeath, bool _bExit, M_TYPE _eType, DIR _eDir = DIR::RIGHT)
		:bItem(_bItem),vPos(_vPos),bDeath(_bDeath),bExit(_bExit),eType(_eType),eDir(_eDir)
	{}
};

struct tSaveObj
{
	Vec2 vPos;
	bool bDeath;
	float fTimer;
	tSaveObj()
		:vPos{},bDeath(false), fTimer(0.f)
	{}
	tSaveObj(const Vec2& _vPos, bool _bDeath = false,float _fTimer = 0.f)
		:vPos(_vPos),bDeath(_bDeath), fTimer(_fTimer)
	{}
};

struct tFiniteObj
{
	Vec2 vPos;
	wstring wcsName;
	int  iIdx;
	bool bDeath;
	
	tFiniteObj()
		:iIdx(0), bDeath(false)
	{}
	tFiniteObj(const Vec2& _vPos,const wstring& _wcsName, int _iIdx = 0, bool _bDeath = false)
		:vPos(_vPos),wcsName(_wcsName), iIdx(_iIdx), bDeath(_bDeath)
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
	bool m_bInit;

protected:
	vector<CStageMove*> m_vNextStage;
	vector<tMonster> m_vMonster;
	vector<tSaveObj> m_vecSaveObj;
	vector<tFiniteObj> m_vecFiniteObj;
	vector<vector<tFiniteObj>> m_vecFiniteTile;

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
	vector<tSaveObj>& GetSaveObj() { return m_vecSaveObj; }
	vector<tFiniteObj>& GetFiniteObj() { return m_vecFiniteObj; }
	void SetDir(DIR _eDir) { m_eDir = _eDir; }
	DIR GetDir() { return m_eDir; }

public:
	void MonsterRegen();
	void ObjRegen();
	void KeyInput();
	void StageMoveInit(wstring _wstrFileName);
	void CamRange();
	void CreateMonster(int iIdx = 0);
	void CreateFiniteObj(int iIdx = 0);

public:
	virtual void Exit();
	virtual int LoadObj(wstring _strPath = L"");
	void ChangeTile(wstring _wstrFileName = L"",CTexture* _pTex = nullptr);

public:
	CGameStage();
	virtual ~CGameStage();
};
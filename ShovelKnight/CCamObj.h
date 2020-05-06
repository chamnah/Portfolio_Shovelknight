#pragma once
#include "CObj.h"
class CCollider;
class CAnimator;
class CCamObj :
	public CObj
{
protected:
	Vec2 m_vRealPos;
	Vec2    m_vPrePos;
	CCollider* m_pColl;
	CAnimator* m_pAnim;
	tSize  m_tSize;
	bool       m_bDeath;


public:
	void InitAnim();

public:
	void SetPrePos(float _fX, float _fY) { m_vPrePos = Vec2(_fX, _fY); }
	void SetPrePos(const Vec2& _vPos) { m_vPrePos = _vPos; }
	void SetRealPos(const Vec2& _vPos) { m_vRealPos = _vPos; }
	void SetRealPos(float _fX, float _fY) { m_vRealPos = Vec2(_fX, _fY);}
	const Vec2& GetPrePos() { return m_vPrePos; }
	const Vec2& GetRealPos() { return m_vRealPos; }
	CCollider* GetColl() { return m_pColl; }
	void SetDeath(bool _bDeath) { m_bDeath = _bDeath; }
	bool GetDeath() { return m_bDeath; }
	void SetSize(const tSize& _tSize) { m_tSize = _tSize; }

public:
	float Distance(const Vec2& _vFirst,const Vec2& _vSecond);

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	CCamObj();
	virtual ~CCamObj();
};
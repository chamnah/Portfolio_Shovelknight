#pragma once
#include "CObj.h"
#include "CCamMgr.h"
class CCollider;
class CCamObj :
	public CObj
{
protected:
	Vec2 m_vRealPos;
	Vec2    m_vPrePos;
	CCollider* m_pColl;


public:
	virtual void Init();

public:
	void SetPrePos(float _fX, float _fY) { m_vPrePos = Vec2(_fX, _fY); }
	void SetPrePos(const Vec2& _vPos) { m_vPrePos = _vPos; }
	void SetRealPos(const Vec2& _vPos) { m_vRealPos = _vPos; }
	void SetRealPos(float _fX, float _fY) { m_vRealPos = Vec2(_fX, _fY);}
	const Vec2& GetPrePos() { return m_vPrePos; }
	const Vec2& GetRealPos() { return m_vRealPos; }

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	CCamObj();
	virtual ~CCamObj();
};
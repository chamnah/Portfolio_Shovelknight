#pragma once
#include "CObj.h"
class CParticle :
	public CObj
{
private:
	Vec2 m_vRealPos;
	tSize  m_tSize;
	float m_fJump;
	float m_fGA;
	DIR m_eDir;
	float m_fSpeed;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetSize(const tSize& _tSize) { m_tSize = _tSize; }
	void SetRealPos(const Vec2& _vPos) { m_vRealPos = _vPos; };
	void SetDir(DIR _eDir) { m_eDir = _eDir; }
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }
	void SetJump(float _fJump) { m_fJump = _fJump; }

public:
	CParticle();
	virtual ~CParticle();
};
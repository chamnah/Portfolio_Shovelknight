#pragma once
#include "CCamObj.h"
class CMoveTile :
	public CCamObj
{
private:
	Vec2 m_vStartPos;
	Vec2 m_vEndPos;
	float m_fSpeed;

public:
	void SetStartPos(Vec2 _vPos) { m_vStartPos = _vPos; }
	void SetEndPos(Vec2 _vPos) { m_vEndPos = _vPos; }

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);

public:
	virtual void OnCollision(CCollider* _other);

public:
	CMoveTile();
	virtual ~CMoveTile();
};
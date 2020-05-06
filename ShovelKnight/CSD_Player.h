#pragma once
#include "CCamObj.h"
class CAnimator;
class CSD_Player :
	public CCamObj
{
private:
	Vec2       m_vEndPos;
	bool       m_bMove;
	float      m_fSpeed;

public:
	virtual int update();
	virtual void render(HDC _dc);


public:
	void SetEndPos(Vec2 _vPos) { m_vEndPos = _vPos; }
	void SetIsMove(bool _bMove) { m_bMove = _bMove; }
	bool GetMove() { return m_bMove; }

public:
	CSD_Player();
	~CSD_Player();
};
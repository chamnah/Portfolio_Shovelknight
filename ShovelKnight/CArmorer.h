#pragma once
#include "CCamObj.h"
class CArmorer :
	public CCamObj
{
private:
	bool m_bView;

public:
	virtual void Init();
	virtual int update();
	virtual void render(HDC _dc);
	virtual void OnCollision(CCollider* _other);

public:
	void SetView(bool _bView) { m_bView = _bView; }

public:
	CArmorer();
	virtual ~CArmorer();
};
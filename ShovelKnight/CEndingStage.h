#pragma once
#include "CGameStage.h"
class CAnimator;
class CEndingStage :
	public CGameStage
{
private:
	float m_fTime;
	bool  m_bFirst;

public:
	virtual void Init() {}
	virtual int Progress();
	virtual void Enter();

public:
	CEndingStage();
	virtual ~CEndingStage();
};
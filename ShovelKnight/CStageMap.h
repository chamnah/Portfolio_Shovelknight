#pragma once
#include "CGameStage.h"
class CStageMap :
	public CGameStage
{
private:
	Vec2 m_vPlayerPos;

public:
	virtual void Init();
	virtual int Progress();
	virtual void Enter();

public:
	CStageMap();
	~CStageMap();
};
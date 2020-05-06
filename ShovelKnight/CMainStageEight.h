#pragma once
#include "CGameStage.h"
class CMainStageEight :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageEight();
	virtual ~CMainStageEight();
};


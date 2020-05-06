#pragma once
#include "CGameStage.h"
class CMainStageOne :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageOne();
	virtual ~CMainStageOne();
};


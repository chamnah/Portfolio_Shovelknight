#pragma once
#include "CGameStage.h"
class CMainStageFour :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageFour();
	virtual ~CMainStageFour();
};


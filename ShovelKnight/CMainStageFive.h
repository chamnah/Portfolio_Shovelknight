#pragma once
#include "CGameStage.h"
class CMainStageFive :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageFive();
	virtual ~CMainStageFive();
};


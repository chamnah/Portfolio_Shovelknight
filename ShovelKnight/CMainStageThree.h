#pragma once
#include "CGameStage.h"
class CMainStageThree :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageThree();
	virtual ~CMainStageThree();
};
#pragma once
#include "CGameStage.h"
class CMainStageTwo :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageTwo();
	virtual ~CMainStageTwo();
};
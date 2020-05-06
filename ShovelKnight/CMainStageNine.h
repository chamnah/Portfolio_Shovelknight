#pragma once
#include "CGameStage.h"
class CMainStageNine :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CMainStageNine();
	virtual ~CMainStageNine();
};
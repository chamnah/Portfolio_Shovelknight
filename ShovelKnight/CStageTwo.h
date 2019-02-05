#pragma once
#include "CGameStage.h"
class CStageTwo :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CStageTwo();
	virtual ~CStageTwo();
};
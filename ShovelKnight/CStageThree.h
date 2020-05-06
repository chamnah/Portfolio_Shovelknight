#pragma once
#include "CGameStage.h"
class CStageThree :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CStageThree();
	virtual ~CStageThree();
};
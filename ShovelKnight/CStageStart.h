#pragma once
#include "CGameStage.h"
class CStageStart :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CStageStart();
	virtual ~CStageStart();
};
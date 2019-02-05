#pragma once
#include "CGameStage.h"
class CStageOne :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CStageOne();
	virtual ~CStageOne();
};
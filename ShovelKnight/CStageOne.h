#pragma once
#include "CStage.h"
class CStageOne :
	public CStage
{
public:
	virtual void Enter();

public:
	CStageOne();
	virtual ~CStageOne();
};
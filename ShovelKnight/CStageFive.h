#pragma once
#include "CGameStage.h"
class CStageFive :
	public CGameStage
{
public:
	virtual void Init();
	virtual void Enter();

public:
	CStageFive();
	virtual ~CStageFive();
};
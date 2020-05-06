#pragma once
#include "CGameStage.h"
class CStageKing :
	public CGameStage
{
private:
	bool m_bEvent;
	bool m_bExit;
	float m_fTime;
	float m_fY;
	float m_fAccTime;
	Vec2  m_vBoss;
	int   m_iNum;
	bool  m_bDeath;
	STAGE_COURSE  m_eCourse;
	bool m_bFirst;


public:
	virtual void Init();
	virtual int Progress();
	virtual void Enter();

public:
	STAGE_COURSE GetCourse() { return m_eCourse; }

public:
	CStageKing();
	~CStageKing();
};
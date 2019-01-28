#pragma once
#include "CStage.h"
enum class SELECT
{
	START,
	TOOL,
	EXIT
};
class CStageLogo : public CStage
{
private:
	HFONT m_Font;
	SELECT m_eSelect;

public:
	virtual int Progress();	
	virtual void Render(HDC _hdc);
	virtual void Enter();
	virtual void Exit();
public:
	CStageLogo();
	virtual ~CStageLogo();
};
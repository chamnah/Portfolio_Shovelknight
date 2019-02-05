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

public:
	void Start();

public:
	CStageLogo();
	virtual ~CStageLogo();
};
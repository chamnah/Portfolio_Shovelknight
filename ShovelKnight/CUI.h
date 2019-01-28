#pragma once
#include "CObj.h"
class CUI :
	public CObj
{
protected:
	vector<vector<CUI*>> m_vecChildUI; // 이중배열로 해도 괜찮을거 같다.
	Vec2         m_vFinalPos;
	CUI*		 m_pParent;
	bool		 m_bIsMouse;
	bool		 m_bMouseOnLB;

public:
	virtual int update();
	virtual void render(HDC _dc);
	virtual int ChildUpdate();
	virtual void ChildRender(HDC _dc);
	virtual void AddChildUI(UI_TYPE _eType,CUI* _UI);
	virtual void ClearUI(UI_TYPE _eType);

public:
	virtual bool IsMouseOn();

public:
	virtual void MouseOn() {} // 마우스가 막 올라온 시점
	virtual void MouseOut() {} // 마우스가 막 벗어난 시점
	virtual void MouseUp() {} // 마우스를 그 자리에서 눌렀다가 다시 뗀 시점
	virtual void MouseDown() {} // 마우스를 그 자리에 누른 시점

public:
	bool GetIsMouse() { return m_bIsMouse; }
	void SetIsMouse(bool _bIsMouse) { m_bIsMouse = _bIsMouse; }
	const vector<vector<CUI*>>& GetChildVector() { return m_vecChildUI; }
	void SetMouseOnLB(bool _bMouseOnLB) { m_bMouseOnLB = _bMouseOnLB; }
	bool GetMouseOnLB() { return m_bMouseOnLB; }
	const CUI* GetParent() { return m_pParent; }

public:
	CUI();
	~CUI();
};
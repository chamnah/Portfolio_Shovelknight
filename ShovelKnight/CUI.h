#pragma once
#include "CObj.h"
class CUI :
	public CObj
{
protected:
	vector<vector<CUI*>> m_vecChildUI; // ���߹迭�� �ص� �������� ����.
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
	virtual void MouseOn() {} // ���콺�� �� �ö�� ����
	virtual void MouseOut() {} // ���콺�� �� ��� ����
	virtual void MouseUp() {} // ���콺�� �� �ڸ����� �����ٰ� �ٽ� �� ����
	virtual void MouseDown() {} // ���콺�� �� �ڸ��� ���� ����

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
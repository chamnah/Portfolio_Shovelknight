#pragma once
#include "CCamObj.h"
class CImageObj :
	public CCamObj
{
private:
	int     m_iIndex;
	wstring m_wcsName;

public:
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetName(wstring _wcsName) { m_wcsName = _wcsName; }
	wstring& GetName() { return m_wcsName; }
	void SetIndex(int _iIndex) { m_iIndex = _iIndex; }

public:
	CImageObj();
	virtual ~CImageObj();
};
#pragma once

class CCore
{
	SINGLE(CCore);

private:
	HWND		m_hWnd;
	HINSTANCE   m_hInst;
	HDC			m_hDC;
	HBITMAP		m_hBit;
	HDC			m_memDC;
	Vec2		m_vResolution;

public:
	void init(HWND _hWnd);
	UINT progress();
	void ChageWindowSize(UINT _iLeft, UINT _iTop, UINT _iWidth, UINT _iHeight, bool _bMenu = false);
	const HDC& GetMainDC() { return m_hDC; }
	const HDC& GetMemDC() { return m_memDC; }
	const Vec2& GetResolution() { return m_vResolution; }
	const HWND& GetHwnd() { return m_hWnd; }
	void SetInstance(HINSTANCE _hInst) { m_hInst = _hInst; }
	HINSTANCE GetInstance() { return m_hInst; }

private:
	int update();
	void render();
};
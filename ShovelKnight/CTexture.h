#pragma once
#include "CRes.h"
class CTexture :
	public CRes
{
private:
	HBITMAP m_bit;
	HDC		m_dc;
	UINT    m_iWidth;
	UINT    m_iHeight;
	tRGB*   m_pPixel;
	wstring m_strPath;


public:
	static CTexture* Load(wstring _path, wstring _relative);
	HDC& GetDC() { return m_dc; }
	UINT GetWidth() { return m_iWidth; }
	UINT GetHeight() { return m_iHeight; }
	HBITMAP& GetBit() { return m_bit; }
	wstring& GetPath() { return m_strPath; }

public:
	CTexture();
	virtual ~CTexture();
};
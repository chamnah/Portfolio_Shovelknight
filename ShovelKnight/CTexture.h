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
	tRGB* GetPixel() { return m_pPixel; }
	void SetBit(HBITMAP _bit) { m_bit = _bit; }
	void SetDC(HDC _dc) { m_dc = _dc; }
	void SetWidth(UINT _width) { m_iWidth = _width; }
	void SetHeight(UINT _height) { m_iHeight = _height; }
	void SetSize(UINT _width, UINT _height) { m_iWidth = _width; m_iHeight = _height; }
	void SetPixel(tRGB* _pPixel) { m_pPixel = _pPixel; }
	void CreateTexture(HDC _dc,HBITMAP _bit,UINT _width,UINT _height,tRGB* _pRGB);

public:
	CTexture();
	virtual ~CTexture();
};
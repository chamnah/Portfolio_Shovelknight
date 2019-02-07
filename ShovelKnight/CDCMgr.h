#pragma once
class CTexture;
class CDCMgr
{
	SINGLE(CDCMgr);

private:
	BITMAP  m_Bit[(UINT)M_TYPE::END][7];
	HBITMAP m_hBit[(UINT)M_TYPE::END][7];
	HDC		m_hDC[(UINT)M_TYPE::END][7];

public:
	void SetChangePixel(M_TYPE _eType,int iIndex,tRGB _rgbOri, tRGB _rgbVal);
	HDC  GetDC(M_TYPE _eType, int iIndex) { return m_hDC[(UINT)_eType][iIndex]; }
};
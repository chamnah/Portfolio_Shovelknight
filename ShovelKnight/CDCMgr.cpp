#include "stdafx.h"
#include "CDCMgr.h"
#include "CResMgr.h"
#include "CTexture.h"

CDCMgr::CDCMgr()
	:m_Bit{}
{
	CTexture* pTex = TEX_LOAD(L"Beeto", L"Image\\Beeto.bmp");

	for (int i = 0; i < 7; ++i)
	{
		m_hBit[(UINT)M_TYPE::BEETO][i] = CreateCompatibleBitmap(pTex->GetDC(), pTex->GetWidth(), pTex->GetHeight());
		m_hDC[(UINT)M_TYPE::BEETO][i] = CreateCompatibleDC(pTex->GetDC());
		(HBITMAP)SelectObject(m_hDC[(UINT)M_TYPE::BEETO][i],m_hBit[(UINT)M_TYPE::BEETO][i]);
		BitBlt(m_hDC[(UINT)M_TYPE::BEETO][i],0,0, pTex->GetWidth(), pTex->GetHeight(), pTex->GetDC(),0,0,SRCCOPY);
		GetObject(m_hBit[(UINT)M_TYPE::BEETO][i], sizeof(BITMAP), &m_Bit[(UINT)M_TYPE::BEETO][i]);
	}
}

CDCMgr::~CDCMgr()
{
}

void CDCMgr::SetChangePixel(M_TYPE _eType, int iIndex, tRGB _rgbOri, tRGB _rgbVal)
{
	tRGB* pRGB = (tRGB*)m_Bit[(UINT)_eType][iIndex].bmBits;
	if (pRGB->dwColor == _rgbOri.dwColor)
	{
		pRGB->dwColor = _rgbVal.dwColor;
	}
}
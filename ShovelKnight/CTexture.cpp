#include "stdafx.h"
#include "CTexture.h"
#include "CCore.h"

CTexture * CTexture::Load(wstring _path,wstring _relative)
{
	HBITMAP bit = (HBITMAP)LoadImage(NULL, _path.c_str(), IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	HDC dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	if (NULL == bit || NULL == dc)
		return NULL;

	HBITMAP oldbit = (HBITMAP)SelectObject(dc, bit);
	DeleteObject(oldbit);

	BITMAP bm{};

	GetObject(bit, sizeof(bm), &bm); // 이거 한 이유는 간단히 크기 알려고

	CTexture* pTex = new CTexture;
	pTex->m_dc = dc;
	pTex->m_bit = bit;
	pTex->m_iWidth = bm.bmWidth;
	pTex->m_iHeight = bm.bmHeight;
	pTex->m_pPixel = (tRGB*)bm.bmBits;
	pTex->m_strPath = _relative;

	return pTex;
}

void CTexture::CreateTexture(HDC _dc, HBITMAP _bit, UINT _width, UINT _height, tRGB * _pRGB)
{
	m_dc = _dc;
	m_bit = _bit;
	m_iWidth = _width;
	m_iHeight = _height;
	m_pPixel = _pRGB;
}

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}
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

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}
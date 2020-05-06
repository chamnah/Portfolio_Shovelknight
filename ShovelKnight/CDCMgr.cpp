#include "stdafx.h"
#include "CDCMgr.h"
#include "CResMgr.h"
#include "CTexture.h"

CDCMgr::CDCMgr()
	: m_Tex{}
	, m_PlayerTex{}
	, m_MapTex{}
{
}

CDCMgr::~CDCMgr()
{
}

void CDCMgr::Init()
{
	CTexture* pPlayer;
	CTexture* pTexTemp[(UINT)M_TYPE::END];
	CTexture* pMap;
	pTexTemp[0] = TEX_LOAD(L"Beeto", L"Image\\Beeto.bmp");
	pTexTemp[1] = TEX_LOAD(L"BubbleDragon", L"Image\\BubbleDragon.bmp");
	pTexTemp[2] = TEX_LOAD(L"Boss", L"Image\\Boss.bmp");
	pTexTemp[3] = TEX_LOAD(L"Slime", L"Image\\Slime.bmp");
	pPlayer = TEX_LOAD(L"Player", L"Image\\Player_1.bmp");
	pMap = TEX_LOAD(L"Level2",L"Image\\Level2.bmp");
	CTexture* pTex;
	BITMAP bm{};

	for (int i = 0; i < (UINT)M_TYPE::END; ++i)
	{
		pTex = pTexTemp[i];
		for (int j = 0; j < 7; ++j)
			CreateTexture(pTex, m_Tex[i][j]);
	}

	for (int i = 0; i < 7; ++i)
		CreateTexture(pPlayer, m_PlayerTex[i]);

	CreateTexture(pMap,m_MapTex);

	SetDamagePixel(M_TYPE::BUBBLE_DRAGON, 0, tRGB(0, 184, 252, 0), tRGB(24, 96, 228, 0), tRGB(40, 0, 172, 0));
	SetDamagePixel(M_TYPE::KING, 0, tRGB(88,64,0, 0), tRGB(212, 0, 220, 0), tRGB(252, 120, 252, 0));
	SetDamagePixel(M_TYPE::SLIME, 0, tRGB(252, 192, 56, 0), tRGB(148, 136, 0, 0), tRGB(252, 0, 0, 0));

	SetPlayerCharge();
	SetPlayerPixel(&m_PlayerTex[1],tRGB(188,188,188,255),tRGB(124,124,124,255),tRGB(252,228,164,255));
	SetPlayerPixel(&m_PlayerTex[2], tRGB(0, 184, 248, 255), tRGB(0, 124, 172, 255), tRGB(120, 216, 248, 255));
	SetPlayerPixel(&m_PlayerTex[3], tRGB(248, 120, 152, 255), tRGB(132, 0, 148, 255), tRGB(88, 120, 248, 255));
	SetPlayerPixel(&m_PlayerTex[4], tRGB(120, 120, 120, 255), tRGB(44, 44, 44, 255), tRGB(0, 56, 248, 255));
	SetPlayerPixel(&m_PlayerTex[5], tRGB(16, 92, 228, 255), tRGB(0, 20, 136, 255), tRGB(16, 92, 228, 255));

	BlackandWhite();
}

void CDCMgr::CreateTexture(CTexture * _pTexOri, CTexture & _TexNew)
{
	_TexNew.SetBit(CreateCompatibleBitmap(_pTexOri->GetDC(), _pTexOri->GetWidth(), _pTexOri->GetHeight()));
	_TexNew.SetDC(CreateCompatibleDC(_pTexOri->GetDC()));
	_TexNew.SetSize(_pTexOri->GetWidth(), _pTexOri->GetHeight());
	HBITMAP OldBit = (HBITMAP)SelectObject(_TexNew.GetDC(), _TexNew.GetBit());
	DeleteObject(OldBit);
	BitBlt(_TexNew.GetDC(), 0, 0, _pTexOri->GetWidth(), _pTexOri->GetHeight(), _pTexOri->GetDC(), 0, 0, SRCCOPY);

	BITMAP bm{};

	GetObject(_TexNew.GetBit(), sizeof(BITMAP), &bm);
	_TexNew.SetPixel((tRGB*)bm.bmBits);
}

void CDCMgr::BlackandWhite()
{
	tRGB* pTemp = m_MapTex.GetPixel();
	int iWidth = m_MapTex.GetWidth();
	int iHeight = m_MapTex.GetHeight();

	for (UINT i = 0; i < m_MapTex.GetHeight(); ++i)
	{
		for (UINT j = 0; j < m_MapTex.GetWidth(); ++j)
		{
			if (pTemp[(i * iWidth) + j].dwColor == tRGB(0,255,0,0).dwColor)
				pTemp[(i * iWidth) + j].dwColor = tRGB(255,255,255,0).dwColor;
			
			else
				pTemp[(i * iWidth) + j].dwColor = tRGB(0, 0, 0, 0).dwColor;
		}
	}
}

void CDCMgr::SetPlayerCharge()
{
	tRGB* pTemp = m_PlayerTex[(UINT)EQUIP_TYPE::SHINING].GetPixel();
	int iWidth = m_PlayerTex[(UINT)EQUIP_TYPE::SHINING].GetWidth();
	int iHeight = m_PlayerTex[(UINT)EQUIP_TYPE::SHINING].GetHeight();

	for (UINT i = 0; i < iHeight; ++i)
	{
		for (UINT j = 0; j < iWidth; ++j)
		{
			if (pTemp[(i * iWidth) + j].dwColor == tRGB(0, 0, 0, 255).dwColor)
			{
				pTemp[(i * iWidth) + j].dwColor = tRGB(166, 10, 7, 255).dwColor;
			}
		}
	}
}

void CDCMgr::SetDamagePixel(M_TYPE _eType, int _iIndex, tRGB _pixel1, tRGB _pixel2, tRGB _pixel3)
{
	CTexture* pTexPalette = TEX_LOAD(L"Damage",L"Image\\Damage.bmp");

	for (int iIndex = 0; iIndex < 7; ++iIndex)
	{
		tRGB* pTemp = m_Tex[(UINT)_eType][iIndex].GetPixel();
		int iWidth = m_Tex[(UINT)_eType][iIndex].GetWidth();
		int iHeight = m_Tex[(UINT)_eType][iIndex].GetHeight();

		for (UINT i = 0; i < iHeight; ++i)
		{
			for (UINT j = 0; j < iWidth; ++j)
			{
				if (pTemp[(i * iWidth) + j].dwColor == _pixel1.dwColor)
				{
					pTemp[(i * iWidth) + j].dwColor = pTexPalette->GetPixel()[iIndex].dwColor;
				}
				else if (pTemp[(i * iWidth) + j].dwColor == _pixel2.dwColor)
				{
					pTemp[(i * iWidth) + j].dwColor = pTexPalette->GetPixel()[(iIndex + 1) % 6].dwColor;
				}
				else if (pTemp[(i * iWidth) + j].dwColor == _pixel3.dwColor)
				{
					pTemp[(i * iWidth) + j].dwColor = pTexPalette->GetPixel()[(iIndex + 2) % 5].dwColor;
				}
			}
		}
	}
}

void CDCMgr::SetPlayerPixel(CTexture* _pTex, tRGB _pixel1, tRGB _pixel2, tRGB _pixel3)
{
	tRGB* pTemp = _pTex->GetPixel();
	int iWidth = _pTex->GetWidth();
	int iHeight = _pTex->GetHeight();

	for (UINT i = 0; i < iHeight; ++i)
	{
		for (UINT j = 0; j < iWidth; ++j)
		{
			if (pTemp[(i * iWidth) + j].dwColor == tRGB(252,192,56,255).dwColor)
			{
				pTemp[(i * iWidth) + j].dwColor = _pixel1.dwColor;
			}
			else if (pTemp[(i * iWidth) + j].dwColor == tRGB(148, 136, 0, 255).dwColor)
			{
				pTemp[(i * iWidth) + j].dwColor = _pixel2.dwColor;
			}
			else if (pTemp[(i * iWidth) + j].dwColor == tRGB(72, 160, 252, 255).dwColor)
			{
				pTemp[(i * iWidth) + j].dwColor = _pixel3.dwColor;
			}
		}
	}
}
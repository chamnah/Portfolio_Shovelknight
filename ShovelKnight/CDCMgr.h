#pragma once
class CTexture;
class CDCMgr
{
	SINGLE(CDCMgr);

private:
	CTexture m_Tex[(UINT)M_TYPE::END][7];
	CTexture m_PlayerTex[(int)EQUIP_TYPE::END];
	CTexture m_MapTex;

public:
	void Init();
	void CreateTexture(CTexture* _pTex,CTexture& _Tex);
	void BlackandWhite();
public:
	void SetPlayerCharge();
	void SetDamagePixel(M_TYPE _eType,int _iIndex, tRGB _pixel1, tRGB _pixel2, tRGB _pixel3);
	void SetPlayerPixel(CTexture* _pTex, tRGB _pixel1, tRGB _pixel2, tRGB _pixel3);
	CTexture* GetTexture(M_TYPE _eType, int _iIndex) { return &m_Tex[(UINT)_eType][_iIndex]; }
	CTexture* GetPlayerTex(int _iIndex) { return &m_PlayerTex[_iIndex]; }
	CTexture* GetMapTex() { return &m_MapTex; }
};
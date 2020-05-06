#pragma once
class CTexture;

struct tData
{
	int iIdx;
	CTexture* pTexture;
	TILE_TYPE eType;

	tData() {}
	tData(int _iIdx,CTexture* _pTex,TILE_TYPE _eType)
		:iIdx(_iIdx),pTexture(_pTex),eType(_eType)
	{}
};

class CMouseEventMgr
{
	SINGLE(CMouseEventMgr);

private:
	vector<vector<tData>> m_vecIdx;
	POINT       m_ptSize;
	int m_iIdx;
	TILE_TYPE m_eType;
	Vec2 m_vSelectPos;
	MOUSE_MODE m_eMode;
	CTexture* m_pTex;
	wstring  m_wcsObject;

public:
	void Render(HDC _dc);

public:
	void SetTileIndex(int _iIdx) { m_iIdx = _iIdx; }
	int  GetTileIndex() { return m_iIdx; }
	void SetTileType(TILE_TYPE _eType) { m_eType = _eType; }
	TILE_TYPE GetType() { return m_eType; }
	void SetTexture(CTexture* pTex) { m_pTex = pTex; }
	CTexture* GetTexture() { return m_pTex; }
	void SetMode(MOUSE_MODE _eMode);
	MOUSE_MODE GetMode() { return m_eMode; }
	void SetCopyIdx(int _iHeight, tData _data);
	void SetReSize(int _iWeight,int _iHeight);
	POINT GetSize() { return m_ptSize; }
	vector<vector<tData>>& GetData() { return m_vecIdx; }
	void SetObjectName(wstring _wcsName) { m_wcsObject = _wcsName; }
	wstring& GetObjectName() { return m_wcsObject; }
};
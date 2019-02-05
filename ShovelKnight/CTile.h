#pragma once
#include "CCamObj.h"
class CTile :
	public CCamObj
{
private:
	HDC m_hDC;
	int m_iIdx;
	int m_TileWidthNum;
	TILE_TYPE m_eAttribute;

public:
	virtual void Init();
	virtual int update();
	void DCRender(HDC _dc, Vec2 _vPos = {});
	virtual void render(HDC _dc);
	void BitRender(HDC _dc);
	void TypeRender(HDC _dc);
	void CollRender(HDC _dc);
	
public:
	void SetDC(const HDC& _hdc) { m_hDC = _hdc; }
	void SetIdx(int _iIdx) { m_iIdx = _iIdx; }
	int  GetIndex() { return m_iIdx; }
	void SetTileType(TILE_TYPE _eType) { m_eAttribute = _eType; }
	TILE_TYPE GetTileType() { return m_eAttribute; }

public:
	CTile(Vec2 _vPos);
	virtual ~CTile();
};
#include "stdafx.h"
#include "CMouseEventMgr.h"
#include "CKeyMgr.h"

CMouseEventMgr::CMouseEventMgr()
	:m_iIdx(INT_MAX)
	,m_vSelectPos(Vec2(0,0))
	,m_eMode(MOUSE_MODE::NONE)
	,m_eType(TILE_TYPE::COLL)
	, m_ptSize({ 0,0 })
{
}

CMouseEventMgr::~CMouseEventMgr()
{
}

void CMouseEventMgr::Render(HDC _dc)
{
	if (m_vecIdx.size() > 0)
	{
		m_vSelectPos =  CKeyMgr::GetInst()->GetMousePos();

		for (UINT i = 0; i < m_vecIdx.size(); ++i)
		{
			for (UINT j = 0; j < m_vecIdx[i].size(); ++j)
			{
				int iTileCountX = (int)(m_vecIdx[i][j].pTexture->GetWidth() / (int)TILE_SIZE);

				int iCol = m_vecIdx[i][j].iIdx % iTileCountX;
				int iLow = m_vecIdx[i][j].iIdx / iTileCountX;

				BitBlt(_dc, (int)(m_vSelectPos.x - (TILE_SIZE / 2)) + j * TILE_SIZE, (int)(m_vSelectPos.y - (TILE_SIZE / 2)) + i * TILE_SIZE, TILE_SIZE, TILE_SIZE, m_vecIdx[i][j].pTexture->GetDC(), iCol * TILE_SIZE, iLow * TILE_SIZE, SRCCOPY);
			}
		}
	}
}

void CMouseEventMgr::SetMode(MOUSE_MODE _eMode)
{
	if (m_eMode == MOUSE_MODE::TILE_COPY && _eMode != MOUSE_MODE::TILE_COPY) // 카피모드 이외에 다른 버튼을 눌렀을 경우 카피모드 종료
	{
		for (UINT i = 0; i < m_vecIdx.size(); ++i)
		{
			m_vecIdx[i].clear();
		}
		m_vecIdx.clear();
	}
	m_eMode = _eMode;
}

void CMouseEventMgr::SetCopyIdx(int _iHeight, tData _data)
{
	if (_iHeight > m_ptSize.y)
		assert(NULL && L"MouseVector 초과");
	m_vecIdx[_iHeight].push_back(_data);
}

void CMouseEventMgr::SetReSize(int _iWeight,int _iHeight)
{
	if (m_vecIdx.size() > 0)
	{
		for (UINT i = 0; i < m_vecIdx.size(); ++i)
		{
			m_vecIdx[i].clear();
		}
		m_vecIdx.clear();
	}

	m_ptSize.x = _iWeight;
	m_ptSize.y = _iHeight;
	m_vecIdx.resize(m_ptSize.y);
}
#include "stdafx.h"
#include "CLightning.h"
#include "CStageMgr.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CAnimator.h"
#include "CTile.h"
#include "CMonster.h"
#include "CCore.h"

CLightning::CLightning()
	:m_iEndX(0)
	, m_iEndY(0)
	, m_fSpeed(0.f)
	, m_bTrace(false)
	, m_iTraceID(0)
	, m_iTraceIdx(0)
{
	m_vScale = Vec2(50,48);
	m_vOffset = Vec2(0,4);
	m_fSpeed = 400.f;
	m_tSize = tSize(13,15);
	m_pTex = TEX_LOAD(L"Lightning",L"Image\\Lightning.bmp");
	m_pAnim = new CAnimator(this);
	m_pAnim->AddAnimation(L"Create", m_pTex, RECT{0,m_tSize.y * 0,m_tSize.x,m_tSize.y},4,0.2f);
	m_pAnim->AddAnimation(L"Idle", m_pTex, RECT{ 0,m_tSize.y * 1,m_tSize.x,m_tSize.y }, 10, 0.2f);
	m_pAnim->PlayAnim(L"Idle",true);
	m_eType = OBJ_TYPE::SKILL;
	m_eSkillType = SKILL_TYPE::LIGHTNIG;
}

CLightning::~CLightning()
{
}

void CLightning::Init()
{

	CCamObj::Init();
	m_vecNode.resize(CStageMgr::GetInst()->GetTileSizeY());
	
	for (UINT i = 0; i < m_vecNode.size(); ++i)
	{
		m_vecNode[i].resize(CStageMgr::GetInst()->GetTileSizeX());
		for (UINT j = 0; j < m_vecNode[i].size(); ++j)
		{
			m_vecNode[i][j].iCurX = j;
			m_vecNode[i][j].iCurY = i;
			if (((CTile*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][i * CStageMgr::GetInst()->GetTileSizeX() + j])->GetTileType() == TILE_TYPE::COLL)
				m_vecNode[i][j].bObstacle = true;
		}
	}

	NearFind();
	if (!m_bTrace)
		return;

	AStar();
}

int CLightning::update()
{
	if (m_bDeath)
	{
		CreateEffect(TEX_LOAD(L"LightningEffect", L"Image\\LightningEffect.bmp"), m_vRealPos, 5);
		return INT_MAX;
	}


	if (!m_bTrace)
	{
		if (m_fTime >= 1.f)
		{
			CreateEffect(TEX_LOAD(L"LightningEffect", L"Image\\LightningEffect.bmp"), m_vRealPos, 5);
			return INT_MAX;
		}
		m_fTime += DT;
	}
	else
	{
		if (CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER].size() != 0 &&
			((CMonster*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER][m_iTraceIdx])->GetID() == m_iTraceID)
		{
			tSize tPos = TilePos(((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER][m_iTraceIdx])->GetRealPos());

			if (tPos.x != m_iEndX || tPos.y != m_iEndY)
			{
				m_iEndX = tPos.x;
				m_iEndY = tPos.y;
				AStarClear();
				AStar();
			}
		}

		if (m_listFind.size() != 0)
		{
			Vec2 vPos = ((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::TILE][m_listFind.front()->iCurX + CStageMgr::GetInst()->GetTileSizeX() * m_listFind.front()->iCurY])->GetRealPos();

			vPos.x += 32;
			vPos.y += 32;

			if (Moving(vPos))
			{
				m_listFind.pop_front();
			}
		}
		else
		{
			CreateEffect(TEX_LOAD(L"LightningEffect", L"Image\\LightningEffect.bmp"), m_vRealPos, 5);
			return INT_MAX;
		}
	}
	m_vPos = CCamMgr::GetInst()->GetRealPos(m_vRealPos.x, m_vRealPos.y);

	if (m_vPos.x < -100 || m_vPos.y < -100 ||
		m_vPos.x > CCore::GetInst()->GetResolution().x + 100 || m_vPos.y > CCore::GetInst()->GetResolution().y + 100)
		return INT_MAX;
	CSkill::update();
	return 0;
}

void CLightning::AStarClear()
{
	for (UINT i = 0; i < m_vecInit.size(); ++i)
	{
		m_vecInit[i]->bOpen = false;
		m_vecInit[i]->bClose = false;
		m_vecInit[i]->pParent = nullptr;
	}

	m_vecInit.clear();

	while (m_queueOpen.empty() == 0)
	{
		m_queueOpen.pop();
	}

	m_listFind.clear();
}

void CLightning::AddOpenList(int _iX, int _iY, tNode* _pParent, int _iFrom)
{
	if (_iX < 0 || _iY < 0 || CStageMgr::GetInst()->GetTileSizeX() <= _iX || CStageMgr::GetInst()->GetTileSizeY() <= _iY
		|| m_vecNode[_iY][_iX].bClose || m_vecNode[_iY][_iX].bObstacle) // 타일 범위를 벗어난다.
		return;

	Cost(_iX,_iY, _pParent,_iFrom);

	if (!m_vecNode[_iY][_iX].bOpen)
	{
		m_vecNode[_iY][_iX].bOpen = true;
		m_queueOpen.push(&m_vecNode[_iY][_iX]);
		m_vecInit.push_back(&m_vecNode[_iY][_iX]);
	}
}

void CLightning::AStar()
{
	int iX = (int)m_vRealPos.x / TILE_SIZE;
	int iY = (int)m_vRealPos.y / TILE_SIZE;

	if (iX == m_iEndX && iY == m_iEndY) // 시작과 동시에 목표물을 찾은 케이스
		return;

	m_vecNode[iY][iX].bOpen = true;
	m_queueOpen.push(&m_vecNode[iY][iX]);
	m_vecInit.push_back(&m_vecNode[iY][iX]);

	tNode* pNode = nullptr;

	while (true)
	{
		pNode = m_queueOpen.top();
		m_queueOpen.pop();
		pNode->bClose = true;

		if (pNode->iCurX == m_iEndX && pNode->iCurY == m_iEndY)
			break;

		AddOpenList(pNode->iCurX + 1, pNode->iCurY, pNode, pNode->fFrom + 1);
		AddOpenList(pNode->iCurX - 1, pNode->iCurY, pNode, pNode->fFrom + 1);
		AddOpenList(pNode->iCurX, pNode->iCurY + 1, pNode, pNode->fFrom + 1);
		AddOpenList(pNode->iCurX, pNode->iCurY - 1, pNode, pNode->fFrom + 1);
		
		if(BlockCheck(pNode->iCurX - 1, pNode->iCurY) && BlockCheck(pNode->iCurX, pNode->iCurY - 1))
			AddOpenList(pNode->iCurX - 1, pNode->iCurY - 1, pNode, pNode->fFrom + 1.5);
		if (BlockCheck(pNode->iCurX + 1, pNode->iCurY) && BlockCheck(pNode->iCurX, pNode->iCurY - 1))
		AddOpenList(pNode->iCurX + 1, pNode->iCurY - 1, pNode, pNode->fFrom + 1.5);
		if (BlockCheck(pNode->iCurX - 1, pNode->iCurY) && BlockCheck(pNode->iCurX, pNode->iCurY + 1))
		AddOpenList(pNode->iCurX - 1, pNode->iCurY + 1, pNode, pNode->fFrom + 1.5);
		if (BlockCheck(pNode->iCurX + 1, pNode->iCurY) && BlockCheck(pNode->iCurX, pNode->iCurY + 1))
		AddOpenList(pNode->iCurX + 1, pNode->iCurY + 1, pNode, pNode->fFrom + 1.5);
	}

	m_listFind.push_front(pNode);

	while (true)
	{
		pNode = pNode->pParent;

		if (pNode == nullptr)
			break;
		m_listFind.push_front(pNode);
	}
}

void CLightning::NearFind()
{
	float fDis = 10000.f;
	for (UINT i = 0; i < CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER].size(); ++i)
	{
		float fTemp = Distance(((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER][i])->GetRealPos(),m_vRealPos);
		if (fDis > fTemp)
		{
			m_iTraceIdx = i;
			m_iTraceID = ((CMonster*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER][i])->GetID();
			m_vDest = ((CCamObj*)CStageMgr::GetInst()->GetObjVector()[(UINT)OBJ_TYPE::MONSTER][i])->GetRealPos();
			fDis = fTemp;
		}
	}

	if (fDis == 10000.f) // 현재 범위안에 몬스터가 없다.
		return;

	m_bTrace = true;
	m_iEndX = m_vDest.x / TILE_SIZE;
	m_iEndY = m_vDest.y / TILE_SIZE;
}

void CLightning::Cost(int _iX, int _iY,tNode* _pParent,int _iFrom)
{
	if (m_vecNode[_iY][_iX].bOpen)
	{
		if (m_vecNode[_iY][_iX].fFrom > _iFrom)
		{
			m_vecNode[_iY][_iX].fFrom = _iFrom;
			m_vecNode[_iY][_iX].pParent = _pParent;
		}
	}
	else
	{
		m_vecNode[_iY][_iX].fFrom = _iFrom;
		m_vecNode[_iY][_iX].pParent = _pParent;
		m_vecNode[_iY][_iX].fDest = sqrtf(powf(_iX - m_iEndX,2.f) + powf(_iY - m_iEndY, 2.f));
		m_vecNode[_iY][_iX].fFinal = m_vecNode[_iY][_iX].fFrom + m_vecNode[_iY][_iX].fDest;
	}
}

bool CLightning::Moving(const Vec2 & _vPos)
{
	if (abs(m_vRealPos.y - _vPos.y) > 20.f || abs(m_vRealPos.x - _vPos.x) > 20.f)
	{
		float fDir = Distance(_vPos, m_vRealPos);
		float fDirX = (_vPos.x - m_vRealPos.x) / fDir;
		float fDirY = (_vPos.y - m_vRealPos.y) / fDir;
		m_vRealPos.x += (m_fSpeed * DT) * fDirX;
		m_vRealPos.y += (m_fSpeed * DT) * fDirY;
		return false;
	}
	return true;
}

tSize CLightning::TilePos(const Vec2 & _vPos)
{
	tSize tPos = {};

	tPos.x = (int)_vPos.x / TILE_SIZE;
	tPos.y = (int)_vPos.y / TILE_SIZE;

	return tPos;
}

bool CLightning::BlockCheck(int _iX, int _iY)
{
	if (_iX < 0 || _iY < 0 || CStageMgr::GetInst()->GetTileSizeX() <= _iX || CStageMgr::GetInst()->GetTileSizeY() <= _iY)
	{
		return true;
	}
	if (m_vecNode[_iY][_iX].bObstacle)
		return false;

	return true;
}

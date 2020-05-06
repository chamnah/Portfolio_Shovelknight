#pragma once
#include "CSkill.h"

struct tNode
{
	bool bOpen;
	bool bClose;
	bool bObstacle;
	int  iCurX;
	int  iCurY;
	tNode* pParent;
	float fDest;
	float fFinal;
	float fFrom;

	tNode()
		:bOpen(false),bClose(false),bObstacle(false),iCurX(0),iCurY(0),pParent(nullptr),fDest(0.f),fFinal(0.f),fFrom(0.f)
	{}
};

struct cmp {
	bool operator()(tNode* t, tNode* u) {
		return t->fFinal > u->fFinal;
	}
};

class CLightning :
	public CSkill
{
private:
	vector<tNode*> m_vecInit;
	vector<vector<tNode>> m_vecNode;
	priority_queue<tNode*,vector<tNode*>,cmp> m_queueOpen;
	Vec2 m_vDest;
	int  m_iEndX;
	int  m_iEndY;
	list<tNode*> m_listFind;
	float m_fSpeed;
	bool m_bTrace;
	int   m_iTraceID;
	int   m_iTraceIdx;
	

public:
	virtual void Init();
	virtual int update();

public:
	void AStarClear();
	void AddOpenList(int _iX, int _iY, tNode* _pParent, int _iFrom);
	void AStar();
	void NearFind();
	void Cost(int _iX,int _iY, tNode* _pParent, int _iFrom);
	bool Moving(const Vec2& _vPos);
	tSize TilePos(const Vec2& _vPos);
	bool BlockCheck(int _iX,int _iY);
	
public:
	CLightning();
	virtual ~CLightning();
};
#pragma once
#include "CCamObj.h"
class CAnimator;
class CSavePoint :
	public CCamObj
{
private:
	CAnimator* m_pAnim;
	bool       m_bNoFirst;
	bool	   m_bSave;
	STAGE	   m_eStage;
	CTexture*  m_pFrontTex;
	CTexture*  m_pChildTex;
	Vec2       m_vChildPos;

public:
	virtual void Init();
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other);
	virtual int update();
	virtual void render(HDC _dc);

	void Test(const Vec2 && test);

public:
	void SetStage(STAGE _eStage) { m_eStage = _eStage; }

public:
	CSavePoint();
	virtual ~CSavePoint();
};
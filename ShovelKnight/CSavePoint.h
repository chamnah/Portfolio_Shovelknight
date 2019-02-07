#pragma once
#include "CCamObj.h"
class CAnimator;
class CSavePoint :
	public CCamObj
{
private:
	CAnimator* m_pAnim;
	bool       m_bNoFirst;
	bool  m_bSave;
	tSize     m_tSize;
	STAGE m_eStage;
	CTexture* m_pFrontTex;
	CTexture* m_pChildTex;
	Vec2      m_vChildPos;

public:
	virtual void Init();
	virtual DIR OnCollisionEnter(CCollider* _other);
	virtual int update();
	virtual void render(HDC _dc);

public:
	void SetStage(STAGE _eStage) { m_eStage = _eStage; }

public:
	CSavePoint();
	virtual ~CSavePoint();
};
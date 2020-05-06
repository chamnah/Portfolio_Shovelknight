#pragma once
#include "CDynamicObj.h"
class CMonster :
	public CDynamicObj
{
protected:
	int   m_iID;
	float m_fTime;
	M_TYPE m_eMType;

public:
	void SetID(int _iID) { m_iID = _iID; }
	int  GetID() { return m_iID; }
	void CreateEffect(const Vec2& _vPos,CTexture* _pTex,int _iSize);
	void SetMType(M_TYPE _eType) { m_eMType = _eType; }
	M_TYPE GetMType() { return m_eMType; }

public:
	CMonster();
	virtual ~CMonster();
};
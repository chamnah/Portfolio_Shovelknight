#pragma once

#include "stdafx.h"
#include "CTexture.h"

class CCollider;
class CObj
{
protected:
	Vec2	m_vPos;
	Vec2	m_vScale;
	CTexture* m_pTex;
	Vec2    m_vOffset;
	OBJ_TYPE m_eType;

public:
	virtual void Init() {}
	virtual int update() = 0;
	virtual void render(HDC _dc) = 0;
	virtual void CollisionRender(HDC _dc);
	virtual int lateupdate() { return 0; };

public:
	const Vec2& GetPos() { return m_vPos; }
	const Vec2& GetScale() { return m_vScale; }
	void SetPos(const Vec2& _vPos) { m_vPos = _vPos; }
	void SetPos(float _fX, float _fY) { m_vPos = Vec2(_fX, _fY); }
	void SetScale(const Vec2& _vScale) { m_vScale = _vScale; }
	void SetTexture(CTexture* _Tex) { m_pTex = _Tex; }
	CTexture* GetTexture() { return m_pTex; }
	OBJ_TYPE GetType() { return m_eType; }

public:
	virtual void OnCollisionEnter(CCollider* _other) {}
	virtual void OnCollision(CCollider* _other) {}
	virtual void OnCollisionExit(CCollider* _other) {}


public:
	CObj();
	virtual ~CObj(); // »ó¼Ó
};
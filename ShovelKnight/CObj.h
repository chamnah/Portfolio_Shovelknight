#pragma once

#include "stdafx.h"
#include "CTexture.h"

class CCollider;
class CObj
{
protected:
	Vec2	m_vPos;
	Vec2	m_vScale;
	CTexture* m_pTex; // 이거는 원본이 전혀 다른데 있으므로 상관없다.
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
	void SetType(OBJ_TYPE _eType) { m_eType = _eType; }
	OBJ_TYPE GetType() { return m_eType; }
	void SetOffset(const Vec2& vOffset) { m_vOffset = vOffset; }

public:
	virtual DIR OnCollisionEnter(CCollider* _mine, CCollider* _other) { return DIR::NONE; }
	virtual void OnCollision(CCollider* _other) {}
	virtual void OnCollisionExit(CCollider* _other) {}

public:
	CObj();
	virtual ~CObj(); // 상속
};
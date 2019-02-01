#pragma once
class CCamMgr
{
	SINGLE(CCamMgr)

private:
	Vec2  m_vLook;
	Vec2  m_vPreLook;
	Vec2  m_vDiff;
	float m_fSpeed;
	Vec2  m_vPlayerPos;

public:
	void update();
	const Vec2 GetRealPos(float _x, float _y) { return Vec2(_x - m_vDiff.x, _y - m_vDiff.y); }
	const Vec2 GetCamMousePos(float _x, float _y) { return Vec2(_x + m_vDiff.x, _y + m_vDiff.y); }
	const Vec2 GetCamMousePos(Vec2& _vPos) { return _vPos + m_vDiff; }
	
	void SetLook(float _x, float _y);
	void SetPlayerPos(float _x, float _y) { m_vPlayerPos = Vec2(_x, _y); }
	const Vec2& GetLook() { return m_vLook; }
	void SetSpeed(float _speed) { m_fSpeed = _speed; }
	const float GetSpeed() { return m_fSpeed; }
	const Vec2& GetPreLook() { return m_vPreLook; }
	const Vec2& GetDifference() { return m_vDiff; }
};
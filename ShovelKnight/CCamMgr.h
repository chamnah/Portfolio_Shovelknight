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
	Vec2  m_vLeftEnd;
	Vec2  m_vRightEnd;
	bool  m_bStop;
	bool  m_bControl;
	float m_fAcc;
	bool  m_bShaking;
public:
	void update();
	const Vec2 GetRealPos(float _x, float _y) { return Vec2(_x - m_vDiff.x, _y - m_vDiff.y); }
	const Vec2 GetCamMousePos(float _x, float _y) { return Vec2(_x + m_vDiff.x, _y + m_vDiff.y); }
	const Vec2 GetCamMousePos(Vec2& _vPos) { return _vPos + m_vDiff; }
	
	void SetLook(float _x, float _y);
	const Vec2& GetLook() { return m_vLook; }
	void SetPlayerPos(float _x, float _y) { m_vPlayerPos = Vec2(_x, _y); }
	void SetSpeed(float _speed) { m_fSpeed = _speed; }
	const float GetSpeed() { return m_fSpeed; }
	const Vec2& GetPreLook() { return m_vPreLook; }
	const Vec2& GetDifference() { return m_vDiff; }
	void SetStop(bool _bStop) { m_bStop = _bStop; }
	void SetLeftEnd(Vec2& _vPos) { m_vLeftEnd = _vPos; }
	void SetRightEnd(Vec2& _vPos) { m_vRightEnd = _vPos; }
	void SetControl(bool _bControl) { m_bControl = _bControl; }
	void SetShake(bool _bShake) { m_bShaking = _bShake; }

public:
	void ScrollCamera(Vec2& _vStart, Vec2& _vEnd, float _fTime);
	bool IsMove();
	void ScrollInit();
	void Shaking();

};
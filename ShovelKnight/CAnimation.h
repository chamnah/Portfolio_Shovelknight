#pragma once
struct tAnimFrame
{
	RECT   rectSize;
	float  fDuration;
};

class CObj;
class CTexture;
class CAnimation
{
private:
	vector<tAnimFrame> m_vecAnimFrame;
	int    m_iCurFrame;
	CObj*  m_pOwner;
	CTexture* m_pTex;
	float m_fAccTime;
	float m_fDuration;
	bool  m_bFinish;

public:
	int Update();
	void Render(HDC _dc);

public:
	void SetFinish(bool _bFinish) { m_bFinish = _bFinish; }
	bool GetFinish() { return m_bFinish; }
	void SetFrameIdx(int _iFrameIdx);

public:
	CAnimation(CTexture * _pTex, RECT _rect, int _iMaxCount, CObj* _pOwner, float _fDuration);
	~CAnimation();
};
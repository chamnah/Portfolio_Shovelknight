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
	Vec2  m_vOffset;
	Vec2  m_vTexPos;

public:
	int Update();
	void Render(HDC _dc);
	void AlphaRender(HDC _dc);

public:
	void SetFinish(bool _bFinish) { m_bFinish = _bFinish; }
	bool GetFinish() { return m_bFinish; }
	void SetFrameIdx(int _iFrameIdx);
	int  GetFrame() { return m_iCurFrame; }
	void SetFrame(int _iFrame) { m_iCurFrame = _iFrame; }
	const vector<tAnimFrame>& GetAnimFrame() { return m_vecAnimFrame; }
	void SetOffset(Vec2 _vOffset) { m_vOffset = _vOffset; }
	void SetTexPos(Vec2 _vTex) { m_vTexPos = _vTex; }

public:
	CAnimation(CTexture * _pTex, RECT _rect, int _iMaxCount, CObj* _pOwner, float _fDuration,Vec2 _vOffset = Vec2(0,0));
	~CAnimation();
};
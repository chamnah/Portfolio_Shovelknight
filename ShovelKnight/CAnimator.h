#pragma once

class CAnimation;
class CTexture;
class CObj;
class CAnimator
{
private:
	map<wstring, CAnimation*> m_mapAnim;
	CAnimation*				  m_pCurAnim;
	CObj*					  m_pOwner;
	bool					  m_bRepeat;
	wstring					  m_strCurKey;

public:
	int update();
	void render(HDC _dc, bool _bAlpha = false);

public:
	void AddAnimation(wstring _key,CTexture* _pTex,RECT _rect,int _iMaxCount, float _fDuration);
	void PlayAnim(wstring _key, bool _bRepeat);
	void ReStartAnim();

public:
	void SetOwner(CObj* _pObj) { m_pOwner = _pObj; }
	wstring& GetCurKey() { return m_strCurKey; };
	bool GetCurFinish();
	int  GetCurFrame();
	
public:
	CAnimator(CObj* _pOwner);
	~CAnimator();
};
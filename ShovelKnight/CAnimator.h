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
	void AddAnimation(wstring _key,CTexture* _pTex,RECT _rect,int _iMaxCount, float _fDuration,Vec2 _vOffset = Vec2(0,0),Vec2 _vTex = Vec2(0,0));
	void PlayAnim(wstring _key, bool _bRepeat);
	void ReStartAnim();
	void SceneRender(HDC _dc, const Vec2& _vPos,const wstring& _wcsKey,int _iFrame);

public:
	void SetOwner(CObj* _pObj) { m_pOwner = _pObj; }
	wstring& GetCurKey() { return m_strCurKey; };
	bool GetCurFinish();
	void SetCurFinish(bool _bFinish);
	int  GetCurFrame();
	void SetCurFrame(int _iFrame);
	
public:
	CAnimator(CObj* _pOwner);
	~CAnimator();
};
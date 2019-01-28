#pragma once
#include "CTexture.h"
#include "CPathMgr.h"
class CResMgr
{
	SINGLE(CResMgr);

private:
	map<wstring, CTexture*> m_mapTex;
public:
	template<typename T>
	CRes* Load(wstring _key, wstring _path = L"");
};

template<typename T>
inline CRes* CResMgr::Load(wstring _key, wstring _path)
{
	if (typeid(T).hash_code() == typeid(CTexture*).hash_code())
	{
		map<wstring, CTexture*>::iterator iter = m_mapTex.find(_key);

		// 만약 이미 존재한다면
		if (iter != m_mapTex.end())
			return iter->second;

		if (_path == L"")
			return NULL;

		wstring strPath = CPathMgr::GetResPath();
		strPath += _path;

		CTexture* pTex = CTexture::Load(strPath, _path);

		if (NULL == pTex)
			return NULL;

		m_mapTex.insert(make_pair(_key, pTex));

		return pTex;
	}
	return NULL;
}
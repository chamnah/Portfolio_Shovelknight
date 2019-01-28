#include "stdafx.h"
#include "CResMgr.h"


CResMgr::CResMgr()
{
}


CResMgr::~CResMgr()
{
	map<wstring, CTexture*>::iterator iter = m_mapTex.begin();
	for (; iter != m_mapTex.end();++iter)
		if(iter->second != NULL)
		delete iter->second;
}
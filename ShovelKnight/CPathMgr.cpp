#include "stdafx.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_szResPath[255] = L"";

void CPathMgr::init()
{
	// Resources ���������� ��θ� ���Ѵ�.
	GetCurrentDirectory(255, g_szResPath);

	// �������� ��� 
	for (UINT i = wcslen(g_szResPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szResPath[i])
		{
			g_szResPath[i + 1] = 0;
			break;
		}
	}
	// -> bin -> Resources
	wcscat_s(g_szResPath, L"bin\\Resources\\");
}

wchar_t * CPathMgr::GetResPath()
{
	return g_szResPath;
}
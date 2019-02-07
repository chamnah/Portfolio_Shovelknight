#include "stdafx.h"
#include "CCore.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CStageMgr.h"
#include "CCamMgr.h"
#include "CCollisionMgr.h"
#include "CGameMgr.h"

CCore::CCore()
	: m_hWnd(NULL)
	, m_hDC(NULL)
	, m_hBit(NULL)
{
}

CCore::~CCore()
{
	ReleaseDC(m_hWnd, m_hDC);
	DeleteObject(m_hBit);
	DeleteDC(m_memDC);
}

void CCore::init(HWND _hWnd)
{	
	m_hWnd = _hWnd;		   // ���� ������ �ڵ�
	m_hDC = GetDC(m_hWnd); // ���� ������ DC 

	// ���� �۾����� ����� ������� ������ ������ ũ�⸦ ������ִ� �Լ�
	ChageWindowSize(0, 0, 1600, 900);

	// ī�� ������ ���� �����
	m_hBit = CreateCompatibleBitmap(m_hDC, (int)m_vResolution.x, (int)m_vResolution.y); // ���� ������ �� ������ ũ���� ��Ʈ�� �̹��� ���۸� ����.
	m_memDC = CreateCompatibleDC(m_hDC);			   // �޸� ��Ʈ�ʰ� ��������� DC �ϳ� ����
	HBITMAP oldBit = (HBITMAP)SelectObject(m_memDC, m_hBit); // DC �� Bitmap �� �����ϰ�, DC �� ������ �ִ� ���� ��Ʈ���� ����

	DeleteObject(oldBit);								// DC �� ������ �ִ� ���� Bitmap �� ������

	// Manager �ʱ�ȭ
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CPathMgr::init();
	CCamMgr::GetInst()->SetLook(m_vResolution.x / 2, m_vResolution.y / 2);
	CStageMgr::GetInst()->Init();
}

UINT CCore::progress()
{
	if (update() == INT_MAX)
		return INT_MAX;
	render();
	return 0;
}

void CCore::ChageWindowSize(UINT _iLeft, UINT _iTop, UINT _iWidth, UINT _iHeight, bool _bMenu)
{
	RECT rt{ 0,	0,	(int)_iWidth, (int)_iHeight };
	UINT iStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	AdjustWindowRect(&rt, iStyle, _bMenu);
	
	SetWindowPos(m_hWnd, NULL, _iLeft, _iTop, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);

	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;
}

int CCore::update()
{
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();
	CGameMgr::GetInst()->Update();
	if (CStageMgr::GetInst()->Update() == INT_MAX)
		return INT_MAX;
	
	CCamMgr::GetInst()->update();
	CCollisionMgr::GetInst()->Update();

	return 0;
}

void CCore::render()
{
	// ȭ�� �����
	//Rectangle(m_memDC, -1, -1, (int)m_vResolution.x + 1, (int)m_vResolution.y + 1);
	BitBlt(m_memDC, 0, 0, (int)m_vResolution.x, (int)m_vResolution.y, m_hDC, 0, 0, BLACKNESS);
	CStageMgr::GetInst()->Render(m_memDC);
	CTimeMgr::GetInst()->render(m_memDC);
	BitBlt(m_hDC, 0, 0, (int)m_vResolution.x, (int)m_vResolution.y, m_memDC, 0, 0, SRCCOPY);
}
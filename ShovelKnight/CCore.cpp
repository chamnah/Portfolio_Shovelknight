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
#include "CDCMgr.h"
#include "SoundMgr.h"

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
	m_hWnd = _hWnd;		   // 메인 윈도우 핸들
	m_hDC = GetDC(m_hWnd); // 메인 윈도우 DC 

	// 실제 작업공간 사이즈를 얻기위해 얻어야할 윈도우 크기를 계산해주는 함수
	ChageWindowSize(0, 0, 1600, 900);

	// 카피 위도우 버퍼 만들기
	m_hBit = CreateCompatibleBitmap(m_hDC, (int)m_vResolution.x, (int)m_vResolution.y); // 메인 윈도우 와 동일한 크기의 비트맵 이미지 버퍼를 만듬.
	m_memDC = CreateCompatibleDC(m_hDC);			   // 메모리 비트맵과 연결시켜줄 DC 하나 만듬
	HBITMAP oldBit = (HBITMAP)SelectObject(m_memDC, m_hBit); // DC 와 Bitmap 을 연결하고, DC 가 가지고 있던 이전 비트맵을 받음

	DeleteObject(oldBit);								// DC 가 가지고 있던 이전 Bitmap 을 삭제함
	
	// Manager 초기화
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CPathMgr::init();
	CDCMgr::GetInst()->Init();
	CCamMgr::GetInst()->SetLook(m_vResolution.x / 2, m_vResolution.y / 2);
	CSoundMgr::GetInst()->init();
	CStageMgr::GetInst()->Init();	
	CSoundMgr::GetInst()->LoadSound(L"MainTheme.wav", L"BGM_01");
	CSoundMgr::GetInst()->LoadSound(L"Strike the Earth!.wav", L"BGM_02");
	CSoundMgr::GetInst()->LoadSound(L"The Forlorn Sanctum.wav", L"BGM_03");
	CSoundMgr::GetInst()->LoadSound(L"A Return to Order.wav", L"BGM_04");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\title_cursor_move.wav", L"CursorMove");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_ui_startgame.wav", L"StartGame");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\player_jump.wav", L"Jump");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\player_attack.wav", L"Attack");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\coin.wav", L"Coin");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_beetle_flip.wav", L"Flip");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_beetle_land.wav", L"Land");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_shovel_upgrade_charge_slash.wav", L"Slash");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_knight_hurt.wav", L"Hurt");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_enchantress_wand.wav", L"Wand");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_enchantress_fireball.wav", L"FireBall");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_enchantress_transform_mu.wav", L"TransForm");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_glass_break_large.wav", L"GlassLarge");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_glass_break_med.wav", L"GlassMed");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_glass_break_small.wav", L"GlassSmall");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_enchantress_platforms_skew.wav", L"End");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_knight_digpile.wav", L"DigPile");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_explode_large.wav", L"Explode");
	CSoundMgr::GetInst()->LoadSound(L"Effect\\sfx_knight_die.wav", L"Die");
	CSoundMgr::GetInst()->PlayBGM(L"BGM_01");
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
	CCamMgr::GetInst()->update();
	if (CStageMgr::GetInst()->Update() == INT_MAX)
		return INT_MAX;
	CCollisionMgr::GetInst()->Update();
	return 0;
}

void CCore::render()
{
	// 화면 지우기
	//Rectangle(m_memDC, -1, -1, (int)m_vResolution.x + 1, (int)m_vResolution.y + 1);
	BitBlt(m_memDC, 0, 0, (int)m_vResolution.x, (int)m_vResolution.y, m_hDC, 0, 0, BLACKNESS);
	CStageMgr::GetInst()->Render(m_memDC);
	CKeyMgr::GetInst()->render(m_memDC);
	CTimeMgr::GetInst()->render(m_memDC);
	BitBlt(m_hDC, 0, 0, (int)m_vResolution.x, (int)m_vResolution.y, m_memDC, 0, 0, SRCCOPY);
}
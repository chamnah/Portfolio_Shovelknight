#include "stdafx.h"
#include "CKeyMgr.h"
#include "CCore.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

int g_vkIdx[(UINT)KEY_TYPE::END] =
{
	'1', // KEY_1
	'2', // KEY_2
	'3', // KEY_3
	'4', // KEY_4
	'5', // KEY_5
	'6', // KEY_6
	'7', // KEY_7
	'8', // KEY_8
	'9', // KEY_9
	'0', // KEY_0
	'Q', // KEY_Q
	'W', // KEY_W,
	'E', // KEY_E
	'R', // KEY_R,
	'T', // KEY_T,
	'Y', // KEY_Y,
	'U', // KEY_U,
	'I', // KEY_I,
	'O', // KEY_O,
	'P', // KEY_P,
	'A', // KEY_A,
	'S', // KEY_S,
	'D', // KEY_D,
	'F', // KEY_F,
	'G', // KEY_G,
	'H', // KEY_H,
	'J', // KEY_J,
	'K', // KEY_K,
	'L', // KEY_L,
	'Z', // KEY_Z,
	'X', // KEY_X,
	'C', // KEY_C,
	'V', // KEY_V,
	'B', // KEY_B,
	'N', // KEY_N,
	'M', // KEY_M,

	VK_F1,	// KEY_F1,
	VK_F2,	// KEY_F2,
	VK_F3,	// KEY_F3,
	VK_F4,	// KEY_F4,
	VK_F5,	// KEY_F5,
	VK_F6,	// KEY_F6,
	VK_F7,	// KEY_F7,
	VK_F8,	// KEY_F8,
	VK_F9,	// KEY_F9,
	VK_F10,	// KEY_F10,
	VK_F11,	// KEY_F11,
	VK_F12,	// KEY_F12,
	VK_LCONTROL,// KEY_CTRL,
	VK_LMENU,	// KEY_ALT,
	VK_LSHIFT,	// KEY_LSHIFT,
	VK_RSHIFT,	// KEY_RSHIFT,
	VK_TAB,		// KEY_TAB,
	VK_CAPITAL,	// KEY_CAP,
	VK_RETURN,	// KEY_ENTER,
	VK_ESCAPE,	// KEY_ESC,
	VK_SPACE,	// KEY_SPACE,
	VK_UP,		// KEY_UP,
	VK_DOWN,	// KEY_DOWN,
	VK_LEFT,	// KEY_LEFT,
	VK_RIGHT,	// KEY_RIGHT,
	VK_NUMPAD1,	// KEY_NUM1,
	VK_NUMPAD2,	// KEY_NUM2,
	VK_NUMPAD3,	// KEY_NUM3,
	VK_NUMPAD4,	// KEY_NUM4,
	VK_NUMPAD5,	// KEY_NUM5,
	VK_NUMPAD6,	// KEY_NUM6,
	VK_NUMPAD7,	// KEY_NUM7,
	VK_NUMPAD8,	// KEY_NUM8,
	VK_NUMPAD9,	// KEY_NUM9,
	VK_NUMPAD0,	// KEY_NUM0,	
	VK_LBUTTON,
	VK_RBUTTON
};

CKeyMgr::CKeyMgr()
	:m_vMousePos(0,0)
	, m_fTime(0.f)
	, m_bInput(false)
	, m_bKey(false)
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
	for (UINT i = 0; i < (UINT)KEY_TYPE::END; ++i)
	{
		m_vecKey.push_back(tKey{ (KEY_TYPE)i, KEY_STATE::NONE });
	}
}

void CKeyMgr::update()
{
	for (UINT i = 0; i < m_vecKey.size(); ++i)
	{
		// 키가 눌렸는지 체크
		if (GetAsyncKeyState(g_vkIdx[(UINT)m_vecKey[i].eType]) & 0x8000)
		{
			// 키가 눌렸는데, 이전에 TAB(최초 누름) 상태였다면...
			if (KEY_STATE::TAB == m_vecKey[i].eState)
			{
				// 막 눌린상태 -> HOLD 유지
				m_vecKey[i].eState = KEY_STATE::HOLD;
			}
			else if (KEY_STATE::HOLD != m_vecKey[i].eState)
			{
				// Away, None -> Tab
				m_vecKey[i].eState = KEY_STATE::TAB;
			}
		}
		else
		{
			// AWAY -> NONE
			// HOLD, TAB -> AWAY
			// NONE -> NONE
			if (KEY_STATE::AWAY == m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::NONE;
			}
			else if (KEY_STATE::NONE != m_vecKey[i].eState)
			{
				m_vecKey[i].eState = KEY_STATE::AWAY;
			}
		}
	}

	Combo();

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(CCore::GetInst()->GetHwnd(), &pt);

	m_vMousePos = pt;
}

void CKeyMgr::render(HDC _hdc)
{
	if (KEY(KEY_TYPE::KEY_2, KEY_STATE::AWAY))
		m_bKey = !m_bKey;


	if(m_bKey)
	{ 
		CTexture* pTex = TEX_LOAD(L"ComboAlpha", L"Image\\ComboAlpha.bmp");

		for (UINT i = 0; i < m_vecComboView.size(); ++i)
		{
			if (m_vecComboView[i] == KEY_TYPE::KEY_Z)
				TransparentBlt(_hdc, i * 64, 200, 64, 64, pTex->GetDC(), 0 * 64, 0, 64, 64, RGB(0, 255, 0));

			else if (m_vecComboView[i] == KEY_TYPE::KEY_RIGHT)
				TransparentBlt(_hdc, i * 64, 200, 64, 64, pTex->GetDC(), 1 * 64, 0, 64, 64, RGB(0, 255, 0));

			else if (m_vecComboView[i] == KEY_TYPE::KEY_LEFT)
				TransparentBlt(_hdc, i * 64, 200, 64, 64, pTex->GetDC(), 2 * 64, 0, 64, 64, RGB(0, 255, 0));

			else if (m_vecComboView[i] == KEY_TYPE::KEY_UP)
				TransparentBlt(_hdc, i * 64, 200, 64, 64, pTex->GetDC(), 3 * 64, 0, 64, 64, RGB(0, 255, 0));

			else if (m_vecComboView[i] == KEY_TYPE::KEY_DOWN)
				TransparentBlt(_hdc, i * 64, 200, 64, 64, pTex->GetDC(), 4 * 64, 0, 64, 64, RGB(0, 255, 0));
		}
	}
}

void CKeyMgr::Combo()
{
	if (KEY(KEY_TYPE::KEY_LEFT, KEY_STATE::TAB))
	{
		m_bInput = true;
		m_fTime = 0.f;
		m_vecCombo.push_back(KEY_TYPE::KEY_LEFT);
		m_vecComboView.push_back(KEY_TYPE::KEY_LEFT);
	}
	else if (KEY(KEY_TYPE::KEY_RIGHT, KEY_STATE::TAB))
	{
		m_bInput = true;
		m_fTime = 0.f;
		m_vecCombo.push_back(KEY_TYPE::KEY_RIGHT);
		m_vecComboView.push_back(KEY_TYPE::KEY_RIGHT);
	}
	else if (KEY(KEY_TYPE::KEY_UP, KEY_STATE::TAB))
	{
		m_bInput = true;
		m_fTime = 0.f;
		m_vecCombo.push_back(KEY_TYPE::KEY_UP);
		m_vecComboView.push_back(KEY_TYPE::KEY_UP);
	}
	else if (KEY(KEY_TYPE::KEY_DOWN, KEY_STATE::TAB))
	{
		m_bInput = true;
		m_fTime = 0.f;
		m_vecCombo.push_back(KEY_TYPE::KEY_DOWN);
		m_vecComboView.push_back(KEY_TYPE::KEY_DOWN);
	}
	else if (KEY(KEY_TYPE::KEY_Z, KEY_STATE::TAB))
	{
		m_bInput = true;
		m_fTime = 0.f;
		m_vecCombo.push_back(KEY_TYPE::KEY_Z);
		m_vecComboView.push_back(KEY_TYPE::KEY_Z);
	}

	if(m_bInput)
		m_fTime += DT;

	if (m_fTime > 0.2f)
	{
		m_vecComboView.clear();
		m_vecCombo.clear();
		m_bInput = false;
		m_fTime = 0.f;
	}

	if (m_vecCombo.size() >= 5)
	{
		m_vecComboView.clear();
		m_vecCombo.clear();
	}
}
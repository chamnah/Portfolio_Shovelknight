#include "stdafx.h"
#include "CScoreUI.h"
#include "CGameMgr.h"

CScoreUI::CScoreUI()
{
	m_vScale = Vec2(100, 40);
	m_Font = CreateFont(35, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Silkscreen");
}

CScoreUI::~CScoreUI()
{
	DeleteObject(m_Font);
}

void CScoreUI::render(HDC _dc)
{
	HFONT OldFont = (HFONT)SelectObject(_dc, m_Font);
	wchar_t wcsGold[20] = L"";
	RECT rt = RECT{(int)m_vPos.x,(int)m_vPos.y,(int)m_vPos.x + (int)m_vScale.x,(int)m_vPos.y + (int)m_vScale.y};
	_itow_s(CGameMgr::GetInst()->GetGold(), wcsGold,10);
	DrawText(_dc, wcsGold,-1, &rt,DT_LEFT);

	(HFONT)SelectObject(_dc, OldFont);
}
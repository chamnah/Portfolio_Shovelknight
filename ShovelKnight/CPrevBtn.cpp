#include "stdafx.h"
#include "CPrevBtn.h"
#include "CResMgr.h"
#include "CKeyMgr.h"
#include "CPanelUI.h"

CPrevBtn::CPrevBtn()
{
	m_pTex = (CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Prev",L"Image\\UP.bmp");
	m_vScale = Vec2((int)m_pTex->GetWidth(),(int)m_pTex->GetHeight());
}

CPrevBtn::~CPrevBtn()
{
}

void CPrevBtn::MouseUp()
{
	((CPanelUI*)m_pParent)->PrevPage();
}

void CPrevBtn::render(HDC _dc)
{
	BitBlt(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, m_pTex->GetWidth(), m_pTex->GetHeight(), m_pTex->GetDC(), 0, 0, SRCCOPY); 
	
	if (m_bIsMouse) // 이건 쓸 코드이다 지우지 말고 여기다가 선택되면 나올것이나 정하자
	{
		HPEN Pen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		HPEN Old = (HPEN)SelectObject(_dc, Pen);

		MoveToEx(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y, NULL);
		LineTo(_dc, (int)(m_vFinalPos.x + m_vScale.x), (int)m_vFinalPos.y);
		LineTo(_dc, (int)(m_vFinalPos.x + m_vScale.x), (int)(m_vFinalPos.y + m_vScale.y));
		LineTo(_dc, (int)m_vFinalPos.x, (int)(m_vFinalPos.y + m_vScale.y));
		LineTo(_dc, (int)m_vFinalPos.x, (int)m_vFinalPos.y);

		SelectObject(_dc, Old);
		DeleteObject(Pen);
	}
}
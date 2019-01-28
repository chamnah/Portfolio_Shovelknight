#include "stdafx.h"
#include "CStageTool.h"
#include "CCore.h"
#include "Resource.h"
#include "CTile.h"
#include "CStageMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CPanelUI.h"
#include "CBtnUI.h"
#include <afxdlgs.h>
#include "CMouseEventMgr.h"

CStageTool::CStageTool()
	:m_hMenu(NULL)
	, m_bCollView(false)
{
	m_hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDC_SHOVELKNIGHT));
	m_ptIdxPos = POINT{INT_MAX,INT_MAX};
	m_Font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, L"Rix�̴Ϲ��� R");
}

CStageTool::~CStageTool()
{
	CStage::Exit();
	DeleteObject(m_Font);
}

int CStageTool::Progress()
{
	CStage::Update();

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_1, KEY_STATE::AWAY))
		m_bCollView = !m_bCollView;

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_ENTER, KEY_STATE::TAB))
	{
		CStageMgr::GetInst()->ChangeStage(STAGE::START);
	}

	if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_PICK)
		TilePicking();
	else if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_COPY)
	{
		TileCopy();
		TileCopyPinking();
	}

	return 0;
}

void CStageTool::Enter() // ó�� ������ ���ϵ�
{
	(CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level1", L"Image\\Level1.bmp");
	(CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level2", L"Image\\Level2.bmp");
	
	Vec2 vSize = CCore::GetInst()->GetResolution();
	CCore::GetInst()->ChageWindowSize(0, 0, (UINT)vSize.x, (UINT)vSize.y, true);
	SetMenu(CCore::GetInst()->GetHwnd(), m_hMenu);
	CStage::CreateTile(20, 20, TILE_SIZE);

	CPanelUI* pObj = new CPanelUI;
	pObj->SetPos(100,100);
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pObj);
	pObj->SetBtn((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level1"));
	pObj->Init();
}

void CStageTool::Exit()
{
	SetMenu(CCore::GetInst()->GetHwnd(), NULL);
	CStage::Exit();
}

void CStageTool::Render(HDC _hdc)
{
	Vec2 vDiff = CCamMgr::GetInst()->GetDifference();

	if (vDiff.x < 0)
		vDiff.x = 0;
	if(vDiff.y < 0)
		vDiff.y = 0;
	
	int iStartCol = (int)vDiff.x / TILE_SIZE;
	int iStartLow = (int)vDiff.y / TILE_SIZE;
	int iEndCol = (int)CCore::GetInst()->GetResolution().x / TILE_SIZE;
	int iEndLow = (int)CCore::GetInst()->GetResolution().y / TILE_SIZE;

	if (iStartCol + iEndCol >= (int)GetTileSizeX())
		iEndCol = GetTileSizeX() - 1 - iStartCol;

	if (iStartLow + iEndLow >= (int)GetTileSizeY())
		iEndLow = GetTileSizeY() - 1 - iStartLow;

	for (int i = iStartLow; i <= iStartLow + iEndLow; ++i)
	{
		for (int j = iStartCol; j <= iStartCol + iEndCol; ++j)
		{
			((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * GetTileSizeX() + j])->BitRender(_hdc);
		}
	}

	if (m_bCollView)
	{
		for (int i = iStartLow; i <= iStartLow + iEndLow; ++i)
		{
			for (int j = iStartCol; j <= iStartCol + iEndCol; ++j)
			{
				((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * GetTileSizeX() + j])->TypeRender(_hdc);
			}
		}
	}

	for (UINT i = 0; i < m_vObj.size(); ++i)
	{
		if(i == (UINT)OBJ_TYPE::TILE)
			continue;
		for (UINT j = 0; j < m_vObj[i].size(); ++j)
		{
			m_vObj[i][j]->render(_hdc);
		}
	}

	CMouseEventMgr::GetInst()->Render(_hdc);

	wchar_t SizeX[50] = {};
	wchar_t SizeY[50] = {};
	wsprintf(SizeX,L"SizeX : %d",m_iTileSizeX,50);
	wsprintf(SizeY, L"SizeY : %d", m_iTileSizeY, 50);

	SetTextColor(_hdc,RGB(255, 255,255));
	//SetBkColor(_hdc,RGB(255,255,255));
	//SetBkMode(_hdc, OPAQUE);
	HFONT OldFont = (HFONT)SelectObject(_hdc, m_Font);
	TextOut(_hdc, CCore::GetInst()->GetResolution().x - 150, 10,SizeX,wcslen(SizeX));
	TextOut(_hdc, CCore::GetInst()->GetResolution().x - 150, 40, SizeY, wcslen(SizeY));
	SelectObject(_hdc,OldFont);
}

void CStageTool::TilePicking()
{
	if (CMouseEventMgr::GetInst()->GetTileIndex() != INT_MAX && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB)) // ó�� Ŭ���� ��ǥ
	{
		Vec2 StartMousePos = CKeyMgr::GetInst()->GetMousePos();
		StartMousePos = CCamMgr::GetInst()->GetCamMousePos(StartMousePos);

		m_ptIdxPos.x = (int)StartMousePos.x / TILE_SIZE;
		m_ptIdxPos.y = (int)StartMousePos.y / TILE_SIZE;

		if (OnUI() || (m_ptIdxPos.x < 0 || m_ptIdxPos.y < 0 || m_ptIdxPos.x >= (int)GetTileSizeX() || m_ptIdxPos.y >= (int)GetTileSizeY()))
			m_ptIdxPos = POINT{ INT_MAX,INT_MAX };
	}

	else if (m_ptIdxPos.x != INT_MAX && !OnUI() && CMouseEventMgr::GetInst()->GetTileIndex() != INT_MAX && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY)) // ���콺�� �� ��ǥ
	{
		Vec2 EndMousePos = CKeyMgr::GetInst()->GetMousePos();
		EndMousePos = CCamMgr::GetInst()->GetCamMousePos(EndMousePos);
		
		int iCol = (int)EndMousePos.x / TILE_SIZE;
		int iLow = (int)EndMousePos.y / TILE_SIZE;

		if (iCol < 0)
			iCol = 0;
		if (iLow < 0)
			iLow = 0;
		if (iCol >= (int)GetTileSizeX())
			iCol = GetTileSizeX() - 1;
		if (iLow >= (int)GetTileSizeY())
			iLow = GetTileSizeY() - 1;

		// �巡���� ���⺰�� ����
		if (m_ptIdxPos.y <= iLow && m_ptIdxPos.x <= iCol)
		{
			for (int i = m_ptIdxPos.y; i <= iLow; ++i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * GetTileSizeX());
					m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->SetTexture(CMouseEventMgr::GetInst()->GetTexture());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetIdx(CMouseEventMgr::GetInst()->GetTileIndex());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetTileType(CMouseEventMgr::GetInst()->GetType());
				}
			}
		}
		else if (m_ptIdxPos.y >= iLow && m_ptIdxPos.x <= iCol)
		{
			for (int i = m_ptIdxPos.y; i >= iLow; --i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * GetTileSizeX());
					m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->SetTexture(CMouseEventMgr::GetInst()->GetTexture());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetIdx(CMouseEventMgr::GetInst()->GetTileIndex());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetTileType(CMouseEventMgr::GetInst()->GetType());
				}
			}
		}
		else if (m_ptIdxPos.y <= iLow && m_ptIdxPos.x >= iCol)
		{
			for (int i = m_ptIdxPos.y; i <= iLow; ++i)
			{
				for (int j = m_ptIdxPos.x; j >= iCol; --j)
				{
					int iIdx = j + (i * GetTileSizeX());
					m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->SetTexture(CMouseEventMgr::GetInst()->GetTexture());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetIdx(CMouseEventMgr::GetInst()->GetTileIndex());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetTileType(CMouseEventMgr::GetInst()->GetType());
				}
			}
		}
		else if (m_ptIdxPos.y >= iLow && m_ptIdxPos.x >= iCol)
		{
			for (int i = m_ptIdxPos.y; i >= iLow; --i)
			{
				for (int j = m_ptIdxPos.x; j >= iCol; --j)
				{ 
					int iIdx = j + (i * GetTileSizeX());
					m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->SetTexture(CMouseEventMgr::GetInst()->GetTexture());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetIdx(CMouseEventMgr::GetInst()->GetTileIndex());
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])->SetTileType(CMouseEventMgr::GetInst()->GetType());
				}
			}
		}
	}
}

void CStageTool::SaveTile()
{
	wstring strPath = CPathMgr::GetResPath();
	strPath += L"Tile\\";

	// FileDialog
	wchar_t szFilter[50] = L"";

	CFileDialog dlg(FALSE, L"tile", L"*.tile",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	strPath = dlg.GetPathName();

	// strPath ��η� ���� ����
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	// Ÿ�� ������ ���Ͽ� �����Ŵ
	// 1. Tile �� ����, ���� ���� �� Ÿ�� ������
	int iXCount = GetTileSizeX();
	int iYCount = GetTileSizeY();
	fwrite(&iXCount, sizeof(int), 1, pFile);
	fwrite(&iYCount, sizeof(int), 1, pFile);

	int iTileSize = TILE_SIZE;
	fwrite(&iTileSize, sizeof(int), 1, pFile);

	// 2. �� Ÿ���� �����ϴ� �̹��� �ε���
	int iImageIdx = 0;
	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		iImageIdx = ((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i])->GetIndex();
		fwrite(&iImageIdx, sizeof(int), 1, pFile);
	}

	int iTileType = 0;

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		iTileType = (int)((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i])->GetTileType();
		fwrite(&iTileType, sizeof(int), 1, pFile);
	}

	const wchar_t* Path = L"";
	wchar_t Key[MAX_PATH] = {};
	int PathSize = 0;
	int Num = 0;

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		Path = ((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i])->GetTexture()->GetPath().c_str();
		PathSize = wcslen(Path);
		
		for (UINT i = wcslen(Path) - 1; i >= 0; --i)
		{
			if ('\\' == Path[i])
			{
				for (UINT j = i + 1; j < wcslen(Path); ++j)
				{
					if ('.' == Path[j])
					{
						break;
					}

					Key[Num] = Path[j];
					++Num;
				}
				break;
			}
		}
		// ������� ���� �� ���⼭ ������
		fwrite(&PathSize, sizeof(int), 1, pFile);
		fwrite(&Num, sizeof(int), 1, pFile);
		fwrite(Key, sizeof(wchar_t), wcslen(Key), pFile);
		fwrite(Path, sizeof(wchar_t), wcslen(Path), pFile);
		Num = 0;
	}

	fclose(pFile);
}

// �̺κ��� �ذ��غ���
void CStageTool::TileCopy()
{	
	if (!OnUI() && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB)) // ó�� Ŭ���� ��ǥ
	{
		Vec2 StartMousePos = CKeyMgr::GetInst()->GetMousePos();
		StartMousePos = CCamMgr::GetInst()->GetCamMousePos(StartMousePos);

		m_ptIdxPos.x = (int)StartMousePos.x / TILE_SIZE;
		m_ptIdxPos.y = (int)StartMousePos.y / TILE_SIZE;

		if ((m_ptIdxPos.x < 0 || m_ptIdxPos.y < 0 || m_ptIdxPos.x >= (int)GetTileSizeX() || m_ptIdxPos.y >= (int)GetTileSizeY()))
			m_ptIdxPos = POINT{ INT_MAX,INT_MAX };
	}

	else if (!OnUI() && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY)) // ���콺�� �� ��ǥ
	{
		Vec2 EndMousePos = CKeyMgr::GetInst()->GetMousePos();
		EndMousePos = CCamMgr::GetInst()->GetCamMousePos(EndMousePos);

		int iCol = (int)EndMousePos.x / TILE_SIZE;
		int iLow = (int)EndMousePos.y / TILE_SIZE;

		if (iCol < 0)
			iCol = 0;
		if (iLow < 0)
			iLow = 0;
		if (iCol >= (int)GetTileSizeX())
			iCol = GetTileSizeX() - 1;
		if (iLow >= (int)GetTileSizeY())
			iLow = GetTileSizeY() - 1;

		int iNum = 0;

		// �巡���� ���⺰�� ����
		if (m_ptIdxPos.y <= iLow && m_ptIdxPos.x <= iCol)
		{
			CMouseEventMgr::GetInst()->SetReSize((iCol + 1) - m_ptIdxPos.x, (iLow + 1) - m_ptIdxPos.y);
			for (int i = m_ptIdxPos.y; i <= iLow; ++i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * GetTileSizeX());
					CMouseEventMgr::GetInst()->SetCopyIdx(iNum, tData(OBJ_TILE->GetIndex(), OBJ_TILE->GetTexture(), OBJ_TILE->GetTileType()));
				}
				++iNum;
			}
		}
		// ������� ��ġ�� �ȴ� 
		else if (m_ptIdxPos.y >= iLow && m_ptIdxPos.x <= iCol)
		{
			CMouseEventMgr::GetInst()->SetReSize((iCol + 1) - m_ptIdxPos.x, (m_ptIdxPos.y + 1) - iLow);
			for (int i = iLow; i <= m_ptIdxPos.y; ++i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * GetTileSizeX());
					CMouseEventMgr::GetInst()->SetCopyIdx(iNum, tData(OBJ_TILE->GetIndex(), OBJ_TILE->GetTexture(), OBJ_TILE->GetTileType()));
				}
				++iNum;
			}
		}
		else if (m_ptIdxPos.y <= iLow && m_ptIdxPos.x >= iCol)
		{
			CMouseEventMgr::GetInst()->SetReSize((m_ptIdxPos.x + 1) - iCol, (iLow + 1) - m_ptIdxPos.y);
			for (int i = m_ptIdxPos.y; i <= iLow; ++i)
			{
				for (int j = iCol; j <= m_ptIdxPos.x; ++j)
				{
					int iIdx = j + (i * GetTileSizeX());
					CMouseEventMgr::GetInst()->SetCopyIdx(iNum, tData(OBJ_TILE->GetIndex(), OBJ_TILE->GetTexture(), OBJ_TILE->GetTileType()));
				}
				++iNum;
			}
		}
		else if (m_ptIdxPos.y >= iLow && m_ptIdxPos.x >= iCol)
		{
			CMouseEventMgr::GetInst()->SetReSize((m_ptIdxPos.x + 1) - iCol, (m_ptIdxPos.y + 1) - iLow);
			for (int i = iLow; i <= m_ptIdxPos.y; ++i)
			{
				for (int j = iCol; j <= m_ptIdxPos.x; ++j)
				{
					int iIdx = j + (i * GetTileSizeX());
					CMouseEventMgr::GetInst()->SetCopyIdx(iNum, tData(OBJ_TILE->GetIndex(), OBJ_TILE->GetTexture(), OBJ_TILE->GetTileType()));
				}
				++iNum;
			}
		}
	}
}
void CStageTool::TileCopyPinking()
{
	if (!OnUI() && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_RBTN,KEY_STATE::AWAY))
	{
		Vec2 vPos = CKeyMgr::GetInst()->GetMousePos();
		vPos = CCamMgr::GetInst()->GetCamMousePos(vPos);

		POINT pt = {};
		POINT ptEnd = CMouseEventMgr::GetInst()->GetSize();
		pt.x = (int)vPos.x / TILE_SIZE;
		pt.y = (int)vPos.y / TILE_SIZE;

		int iWidth = 0;
		int iHeight = 0;

		int iSizeX = CMouseEventMgr::GetInst()->GetData()[0].size();
		int iSizeY = CMouseEventMgr::GetInst()->GetData().size();

		for (int i = pt.y; i < pt.y + ptEnd.y; ++i)
		{
			for (int j = pt.x; j < pt.x+ptEnd.x; ++j)
			{
				int iIdx = j + (i * GetTileSizeX());
				m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->SetTexture(CMouseEventMgr::GetInst()->GetData()[iHeight][iWidth].pTexture);
				OBJ_TILE->SetIdx(CMouseEventMgr::GetInst()->GetData()[iHeight][iWidth].iIdx);
				OBJ_TILE->SetTileType(CMouseEventMgr::GetInst()->GetData()[iHeight][iWidth].eType);
				++iWidth;
			}
			++iHeight;
			iWidth = 0;
		}
	}
}

bool CStageTool::OnUI()
{
	bool bPick = false;
	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::UI].size(); ++i) // ���콺�� Ÿ������ �ִ��� �˻� ������ Ÿ�� �浹�� ���´�.
	{
		if (((CUI*)m_vObj[(UINT)OBJ_TYPE::UI][i])->GetIsMouse())
			bPick = true;
	}

	return bPick;
}

	
INT_PTR CALLBACK ChangeTileSize(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			UINT iWidth = GetDlgItemInt(hDlg,IDC_TILE_WIDTH,NULL,false);
			UINT iHeight = GetDlgItemInt(hDlg, IDC_TILE_HEIGHT, NULL, false);

			CStage* pCurStage = CStageMgr::GetInst()->GetCurStage();
			

			pCurStage->ChangeTile(iWidth, iHeight);
			CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2, CCore::GetInst()->GetResolution().y / 2);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
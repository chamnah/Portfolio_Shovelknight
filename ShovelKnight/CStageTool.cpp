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
#include <io.h>
#include "CLayerPanelUI.h"
#include "CLayerUI.h"

INT_PTR CALLBACK    ChangeScale(HWND, UINT, WPARAM, LPARAM);

CStageTool::CStageTool()
	:m_hMenu(NULL)
	, m_bCollView(false)
{
	m_hMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDC_SHOVELKNIGHT));
	m_ptIdxPos = POINT{INT_MAX,INT_MAX};
	m_Font = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"power pixel-7");
}

CStageTool::~CStageTool()
{
	CStage::Exit();
	DeleteObject(m_Font);
}

int CStageTool::Progress()
{
	UICheck();
	CStage::Update();

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_1, KEY_STATE::AWAY))
		m_bCollView = !m_bCollView;

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_0, KEY_STATE::TAB))
	{
		CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2, CCore::GetInst()->GetResolution().y / 2);
		CStageMgr::GetInst()->ChangeStage(STAGE::START);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_2, KEY_STATE::TAB))
	{
		DialogBox(CCore::GetInst()->GetInstance() , MAKEINTRESOURCE(IDD_DIALOG1), CCore::GetInst()->GetHwnd(), ChangeScale);
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

void CStageTool::Enter() // 처음 들어오면 할일들
{
	TEX_LOAD(L"Level1", L"Image\\Level1.bmp");
	TEX_LOAD(L"Level2", L"Image\\Level2.bmp");
	
	Vec2 vSize = CCore::GetInst()->GetResolution();
	CCore::GetInst()->ChageWindowSize(0, 0, (UINT)vSize.x, (UINT)vSize.y, true);
	SetMenu(CCore::GetInst()->GetHwnd(), m_hMenu);
	CStage::CreateTile(20, 20, TILE_SIZE);

	CPanelUI* pPanel = new CPanelUI;
	pPanel->SetPos(100,100);
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pPanel);
	pPanel->SetBtn((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level1"));
	pPanel->Init();

	CLayerPanelUI* pLayerPanel = new CLayerPanelUI;
	pLayerPanel->SetPos(1300, 600);
	pLayerPanel->SetScale(Vec2(300,100));
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pLayerPanel);

	CLayerUI* pLayer = new CLayerUI;
	pLayerPanel->AddChildUI(UI_TYPE::LAYER, pLayer);
	
	_finddata_t fd;
	long handle = 0;
	int  result = 1;
	string strFile = "";
	wstring wstr = L"";

	handle = _findfirst("..\\bin\\Resources\\Image\\Tool_View\\*.bmp",&fd);

	if (handle == -1)
		assert(false && L"아무것도 없다 경로 잘못 쓴듯");

	while (result != -1)
	{
		strFile = fd.name;
		pPanel->VecFilePush(wstr.assign(strFile.begin(), strFile.end()));
		result = _findnext(handle,&fd);
	}

	_findclose(handle);
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

	if (iStartCol + iEndCol >= (int)CStageMgr::GetInst()->GetTileSizeX())
		iEndCol = CStageMgr::GetInst()->GetTileSizeX() - 1 - iStartCol;

	if (iStartLow + iEndLow >= (int)CStageMgr::GetInst()->GetTileSizeY())
		iEndLow = CStageMgr::GetInst()->GetTileSizeY() - 1 - iStartLow;

	for (int i = iStartLow; i <= iStartLow + iEndLow; ++i)
	{
		for (int j = iStartCol; j <= iStartCol + iEndCol; ++j)
		{
			((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * CStageMgr::GetInst()->GetTileSizeX() + j])->BitRender(_hdc);
		}
	}

	if (m_bCollView)
	{
		for (int i = iStartLow; i <= iStartLow + iEndLow; ++i)
		{
			for (int j = iStartCol; j <= iStartCol + iEndCol; ++j)
			{
				((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * CStageMgr::GetInst()->GetTileSizeX() + j])->TypeRender(_hdc);
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
	wsprintf(SizeX,L"SizeX : %d", CStageMgr::GetInst()->GetTileSizeX(),50);
	wsprintf(SizeY, L"SizeY : %d", CStageMgr::GetInst()->GetTileSizeY(), 50);

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
	if (CMouseEventMgr::GetInst()->GetTileIndex() != INT_MAX && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB)) // 처음 클릭한 좌표
	{
		Vec2 StartMousePos = CKeyMgr::GetInst()->GetMousePos();
		StartMousePos = CCamMgr::GetInst()->GetCamMousePos(StartMousePos);

		m_ptIdxPos.x = (int)StartMousePos.x / TILE_SIZE;
		m_ptIdxPos.y = (int)StartMousePos.y / TILE_SIZE;

		if (OnUI() || (m_ptIdxPos.x < 0 || m_ptIdxPos.y < 0 || m_ptIdxPos.x >= (int)CStageMgr::GetInst()->GetTileSizeX() || m_ptIdxPos.y >= (int)CStageMgr::GetInst()->GetTileSizeY()))
			m_ptIdxPos = POINT{ INT_MAX,INT_MAX };
	}

	else if (m_ptIdxPos.x != INT_MAX && !OnUI() && CMouseEventMgr::GetInst()->GetTileIndex() != INT_MAX && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY)) // 마우스를 뗀 좌표
	{
		Vec2 EndMousePos = CKeyMgr::GetInst()->GetMousePos();
		EndMousePos = CCamMgr::GetInst()->GetCamMousePos(EndMousePos);
		
		int iCol = (int)EndMousePos.x / TILE_SIZE;
		int iLow = (int)EndMousePos.y / TILE_SIZE;

		if (iCol < 0)
			iCol = 0;
		if (iLow < 0)
			iLow = 0;
		if (iCol >= (int)CStageMgr::GetInst()->GetTileSizeX())
			iCol = CStageMgr::GetInst()->GetTileSizeX() - 1;
		if (iLow >= (int)CStageMgr::GetInst()->GetTileSizeY())
			iLow = CStageMgr::GetInst()->GetTileSizeY() - 1;

		// 드래그한 방향별로 정리
		if (m_ptIdxPos.y <= iLow && m_ptIdxPos.x <= iCol)
		{
			for (int i = m_ptIdxPos.y; i <= iLow; ++i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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

	// strPath 경로로 파일 생성
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	// 타일 정보를 파일에 저장시킴
	// 1. Tile 의 가로, 세로 개수 와 타일 사이즈
	int iXCount = CStageMgr::GetInst()->GetTileSizeX();
	int iYCount = CStageMgr::GetInst()->GetTileSizeY();
	fwrite(&iXCount, sizeof(int), 1, pFile);
	fwrite(&iYCount, sizeof(int), 1, pFile);

	int iTileSize = TILE_SIZE;
	fwrite(&iTileSize, sizeof(int), 1, pFile);

	// 2. 각 타일의 참조하는 이미지 인덱스
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

		fwrite(&PathSize, sizeof(int), 1, pFile);
		fwrite(&Num, sizeof(int), 1, pFile);
		fwrite(Key, sizeof(wchar_t), wcslen(Key), pFile);
		fwrite(Path, sizeof(wchar_t), wcslen(Path), pFile);
		Num = 0;
	}

	fclose(pFile);
}

// 이부분을 해결해보자
void CStageTool::TileCopy()
{	
	if (!OnUI() && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB)) // 처음 클릭한 좌표
	{
		Vec2 StartMousePos = CKeyMgr::GetInst()->GetMousePos();
		StartMousePos = CCamMgr::GetInst()->GetCamMousePos(StartMousePos);

		m_ptIdxPos.x = (int)StartMousePos.x / TILE_SIZE;
		m_ptIdxPos.y = (int)StartMousePos.y / TILE_SIZE;

		if ((m_ptIdxPos.x < 0 || m_ptIdxPos.y < 0 || m_ptIdxPos.x >= (int)CStageMgr::GetInst()->GetTileSizeX() || m_ptIdxPos.y >= (int)CStageMgr::GetInst()->GetTileSizeY()))
			m_ptIdxPos = POINT{ INT_MAX,INT_MAX };
	}

	else if (!OnUI() && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY)) // 마우스를 뗀 좌표
	{
		Vec2 EndMousePos = CKeyMgr::GetInst()->GetMousePos();
		EndMousePos = CCamMgr::GetInst()->GetCamMousePos(EndMousePos);

		int iCol = (int)EndMousePos.x / TILE_SIZE;
		int iLow = (int)EndMousePos.y / TILE_SIZE;

		if (iCol < 0)
			iCol = 0;
		if (iLow < 0)
			iLow = 0;
		if (iCol >= (int)CStageMgr::GetInst()->GetTileSizeX())
			iCol = CStageMgr::GetInst()->GetTileSizeX() - 1;
		if (iLow >= (int)CStageMgr::GetInst()->GetTileSizeY())
			iLow = CStageMgr::GetInst()->GetTileSizeY() - 1;

		int iNum = 0;

		// 드래그한 방향별로 정리
		if (m_ptIdxPos.y <= iLow && m_ptIdxPos.x <= iCol)
		{
			CMouseEventMgr::GetInst()->SetReSize((iCol + 1) - m_ptIdxPos.x, (iLow + 1) - m_ptIdxPos.y);
			for (int i = m_ptIdxPos.y; i <= iLow; ++i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
					CMouseEventMgr::GetInst()->SetCopyIdx(iNum, tData(OBJ_TILE->GetIndex(), OBJ_TILE->GetTexture(), OBJ_TILE->GetTileType()));
				}
				++iNum;
			}
		}
		// 여기부터 고치면 된다 
		else if (m_ptIdxPos.y >= iLow && m_ptIdxPos.x <= iCol)
		{
			CMouseEventMgr::GetInst()->SetReSize((iCol + 1) - m_ptIdxPos.x, (m_ptIdxPos.y + 1) - iLow);
			for (int i = iLow; i <= m_ptIdxPos.y; ++i)
			{
				for (int j = m_ptIdxPos.x; j <= iCol; ++j)
				{
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
					int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
				int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
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
	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::UI].size(); ++i) // 마우스가 타일위에 있는지 검사 있으면 타일 충돌을 막는다.
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
			
			/* 레이어 dc 크기 변경
			for (UINT i = 0; i < pCurStage->GetObj(OBJ_TYPE::UI).size(); ++i)
			{
				if (((CUI*)pCurStage->GetObj(OBJ_TYPE::UI)[i])->GetUIType() == UI_TYPE::LAYER)
				{
					for (UINT j = 0; j < ((CUI*)pCurStage->GetObj(OBJ_TYPE::UI)[i])->GetChildUI(UI_TYPE::LAYER).size();++j)
					{
						((CLayerUI*)((CUI*)pCurStage->GetObj(OBJ_TYPE::UI)[i])->GetChildUI(UI_TYPE::LAYER)[j])->ChangeDCSize(tSize(iWidth, iHeight));
					}
				}
			}*/

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

INT_PTR CALLBACK ChangeScale(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			UINT iWidth = GetDlgItemInt(hDlg, IDC_SCALE_X, NULL, false);
			UINT iHeight = GetDlgItemInt(hDlg, IDC_SCALE_Y, NULL, false);

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
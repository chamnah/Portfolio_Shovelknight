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
#include "CImageObj.h"
#include "CTimeMgr.h"
#include "CCamMgr.h"
#include "CHiddenTile.h"
#include "SoundMgr.h"

INT_PTR CALLBACK    ChangeScale(HWND, UINT, WPARAM, LPARAM);

CStageTool::CStageTool()
	:m_hMenu(NULL)
	, m_bCollView(false)
	, m_iHiddeniIdx((UINT)OBJ_TYPE::HIDDEN)
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

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_0, KEY_STATE::TAB)) // 다른 스테이지로 이동
	{
		CCamMgr::GetInst()->SetLook(CCore::GetInst()->GetResolution().x / 2, CCore::GetInst()->GetResolution().y / 2);
		CCamMgr::GetInst()->ScrollInit();
		CStageMgr::GetInst()->SetTileSizeX(0);
		CStageMgr::GetInst()->SetTileSizeY(0);
		CStageMgr::GetInst()->ChangeStage(STAGE::LOGO);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_2, KEY_STATE::TAB))
	{
		DialogBox(CCore::GetInst()->GetInstance() , MAKEINTRESOURCE(IDD_DIALOG1), CCore::GetInst()->GetHwnd(), ChangeScale); // 그냥 다이얼 로그 떠지나 실험
	}

	if (CMouseEventMgr::GetInst()->GetType() == TILE_TYPE::HIDDEN)
		HiddenTilePicking();
	else if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_PICK)
		TilePicking();
	else if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::TILE_COPY)
	{
		TileCopy();
		TileCopyPinking();
	}
	else if (CMouseEventMgr::GetInst()->GetMode() == MOUSE_MODE::OBJ_PICK)
	{
		ObjPicking();
	}

	return 0;
}

void CStageTool::Enter() // 처음 들어오면 할일들
{
	CSoundMgr::GetInst()->SetBGMVolume(0);

	TEX_LOAD(L"Level1", L"Image\\Level1.bmp");
	TEX_LOAD(L"Level2", L"Image\\Level2.bmp");
	
	/*Vec2 vSize = CCore::GetInst()->GetResolution();
	CCore::GetInst()->ChageWindowSize(0, 0, (UINT)vSize.x, (UINT)vSize.y, true);*/ // 이거는 메뉴바 만큼 사이즈를 늘리기 위해서 만든것이다.
	SetMenu(CCore::GetInst()->GetHwnd(), m_hMenu);
	CStage::CreateTile(20, 20, TILE_SIZE);

	CPanelUI* pPanel = new CPanelUI;
	pPanel->SetPos(100,100);
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pPanel);
	pPanel->SetBtn((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Level1"));
	pPanel->Init();
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

	for (UINT i = 0; i < m_vecHidden.size(); ++i)
	{
		for (UINT j = 0; j < m_vecHidden[i].size(); ++j)
		{
			m_vecHidden[i][j]->render(_hdc);
		}
	}

	CMouseEventMgr::GetInst()->Render(_hdc);

	wchar_t SizeX[50] = {};
	wchar_t SizeY[50] = {};
	wchar_t Index[50] = {};
	wsprintf(SizeX,L"SizeX : %d", CStageMgr::GetInst()->GetTileSizeX(),50);
	wsprintf(SizeY, L"SizeY : %d", CStageMgr::GetInst()->GetTileSizeY(), 50);
	wsprintf(Index, L"Index : %d", m_iHiddeniIdx, 50);

	SetTextColor(_hdc,RGB(255, 255,255));
	//SetBkColor(_hdc,RGB(255,255,255));
	//SetBkMode(_hdc, OPAQUE);
	HFONT OldFont = (HFONT)SelectObject(_hdc, m_Font);
	TextOut(_hdc, CCore::GetInst()->GetResolution().x - 150, 10,SizeX,wcslen(SizeX));
	TextOut(_hdc, CCore::GetInst()->GetResolution().x - 150, 40, SizeY, wcslen(SizeY));
	TextOut(_hdc, CCore::GetInst()->GetResolution().x - 150, 70, Index, wcslen(Index));

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

void CStageTool::SaveObject()
{
	wstring strPath = CPathMgr::GetResPath();
	strPath += L"Object\\";

	// FileDialog
	wchar_t szFilter[50] = L"";

	CFileDialog dlg(FALSE, L"obj", L"*.obj",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strPath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	strPath = dlg.GetPathName();

	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	UINT iSize = m_vObj[(UINT)OBJ_TYPE::OBJECT].size();
	fwrite(&iSize, sizeof(int), 1, pFile);
	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::OBJECT].size(); ++i)
	{
		const wchar_t* FileName = ((CImageObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT][i])->GetName().c_str();
		int iNameLength = wcslen(FileName) + 1;

		fwrite(&iNameLength, sizeof(int), 1, pFile);
		fwrite(FileName, sizeof(wchar_t), iNameLength, pFile);

		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT][i])->GetRealPos();
		fwrite(&vPos.x, sizeof(float), 1, pFile);
		fwrite(&vPos.y, sizeof(float), 1, pFile);
	}

	iSize = m_vObj[(UINT)OBJ_TYPE::HIDDEN].size();
	fwrite(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::HIDDEN].size(); ++i)
	{
		const wchar_t* FileName = L"Hidden";
		int iNameLength = wcslen(FileName) + 1;

		fwrite(&iNameLength, sizeof(int), 1, pFile);
		fwrite(FileName, sizeof(wchar_t), iNameLength, pFile);

		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::HIDDEN][i])->GetRealPos();
		fwrite(&vPos.x, sizeof(float), 1, pFile);
		fwrite(&vPos.y, sizeof(float), 1, pFile);

		int iIdx = ((CHiddenTile*)m_vObj[(UINT)OBJ_TYPE::HIDDEN][i])->GetIndex();

		fwrite(&iIdx, sizeof(int), 1, pFile);
	}

	iSize = m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO].size();
	fwrite(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO].size(); ++i)
	{
		const wchar_t* FileName = L"HiddenTwo";
		int iNameLength = wcslen(FileName) + 1;

		fwrite(&iNameLength, sizeof(int), 1, pFile);
		fwrite(FileName, sizeof(wchar_t), iNameLength, pFile);

		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO][i])->GetRealPos();
		fwrite(&vPos.x, sizeof(float), 1, pFile);
		fwrite(&vPos.y, sizeof(float), 1, pFile);

		int iIdx = ((CHiddenTile*)m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO][i])->GetIndex();

		fwrite(&iIdx, sizeof(int), 1, pFile);
	}

	fclose(pFile);
}

int CStageTool::LoadObj(wstring _strPath)
{
	wstring strPath = CPathMgr::GetResPath();
	if (_strPath == L"")
	{
		strPath += L"Object\\";

		// FileDialog
		wchar_t szFilter[50] = L"";
		CFileDialog dlg(FALSE, L"obj", L"*.obj",
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

		dlg.m_ofn.lpstrInitialDir = strPath.c_str();

		if (dlg.DoModal() != IDOK)
			return true;

		strPath = dlg.GetPathName();
	}
	else
	{
		strPath += _strPath;
	}

	ClearObj((int)OBJ_TYPE::OBJECT);

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::OBJECT].size(); ++i)
		delete m_vObj[(UINT)OBJ_TYPE::OBJECT][i];

	m_vObj[(UINT)OBJ_TYPE::OBJECT].clear();

	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");
	
	Vec2 vPos = Vec2(0, 0);
	int iNameLength = 0;
	int iSize = 0;
	wstring wcsKey = L"Tool_View";
	wstring wcsPath = L"Image\\Tool_View\\";
	wstring wcsName;
	wstring wcsType = L".bmp";

	fread(&iSize, sizeof(int), 1, pFile);

	for (int i = 0; i < iSize; ++i)
	{
		wchar_t wcsFileName[255] = L"";
		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(wcsFileName, sizeof(wchar_t), iNameLength, pFile);
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		wcsName = wcsFileName;

		CImageObj* pObj = new CImageObj;
		pObj->SetRealPos(vPos);
		pObj->SetName(wcsFileName);
		pObj->SetTexture(TEX_LOAD(wcsKey + wcsFileName, wcsPath + wcsName + wcsType));
		pObj->SetSize(tSize(pObj->GetTexture()->GetWidth(), pObj->GetTexture()->GetHeight()));
		pObj->SetScale(Vec2(pObj->GetTexture()->GetWidth() * 4, pObj->GetTexture()->GetHeight() * 4));
		m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
	}

	iNameLength = 0;
	

	iSize = 0;
	fread(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		wchar_t wcsFileName2[255] = L"";
		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(wcsFileName2, sizeof(wchar_t), iNameLength, pFile);

		Vec2 vPos = Vec2();
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		CHiddenTile* pObj = new CHiddenTile;
		pObj->SetRealPos(vPos);
		pObj->SetPos(vPos);
		pObj->Init();
		m_vObj[(UINT)OBJ_TYPE::HIDDEN].push_back(pObj);

		int iIdx = 0; 

		fread(&iIdx, sizeof(int), 1, pFile);

		((CHiddenTile*)m_vObj[(UINT)OBJ_TYPE::HIDDEN][i])->SetIndex(iIdx);
	}

	iNameLength = 0;
	

	iSize = 0;
	fread(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		wchar_t wcsFileName2[255] = L"";
		fread(&iNameLength, sizeof(int), 1, pFile);
		fread(wcsFileName2, sizeof(wchar_t), iNameLength, pFile);

		Vec2 vPos = Vec2();
		fread(&vPos.x, sizeof(float), 1, pFile);
		fread(&vPos.y, sizeof(float), 1, pFile);

		CHiddenTile* pObj = new CHiddenTile;
		pObj->SetRealPos(vPos);
		pObj->SetPos(vPos);
		pObj->Init();
		m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO].push_back(pObj);

		int iIdx = 0;

		fread(&iIdx, sizeof(int), 1, pFile);

		((CHiddenTile*)m_vObj[(UINT)OBJ_TYPE::HIDDEN_TWO][i])->SetIndex(iIdx);
	}

	fclose(pFile);

	return 0;
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
		POINT ptEnd = CMouseEventMgr::GetInst()->GetSize(); // 타일의 실제 크기 0이 포함되지 않는 진짜 크기
		pt.x = (int)vPos.x / TILE_SIZE;
		pt.y = (int)vPos.y / TILE_SIZE;

		if (pt.x < 0 || pt.y < 0 || CStageMgr::GetInst()->GetTileSizeX() <= pt.x
			|| CStageMgr::GetInst()->GetTileSizeY() <= pt.y)
			return;
		int iWidth = 0;
		int iHeight = 0;

		int iSizeX = CMouseEventMgr::GetInst()->GetData()[0].size();
		int iSizeY = CMouseEventMgr::GetInst()->GetData().size();

		if (pt.x + ptEnd.x > CStageMgr::GetInst()->GetTileSizeX())
		{
			int iX = pt.x + ptEnd.x;
			ptEnd.x -= (iX - CStageMgr::GetInst()->GetTileSizeX());
		}

		if (pt.y + ptEnd.y > CStageMgr::GetInst()->GetTileSizeY())
		{
			int iY = pt.y + ptEnd.y;
			ptEnd.y -= (iY - CStageMgr::GetInst()->GetTileSizeY());
		}

		for (int i = pt.y; i < pt.y + ptEnd.y; ++i)
		{
			for (int j = pt.x; j < pt.x+ptEnd.x; ++j)
			{
				int iIdx = j + (i * CStageMgr::GetInst()->GetTileSizeX());
				m_vObj[(UINT)OBJ_TYPE::TILE][iIdx]->SetTexture(CMouseEventMgr::GetInst()->GetData()[iHeight][iWidth].pTexture);
				OBJ_TILE->SetIdx(CMouseEventMgr::GetInst()->GetData()[iHeight][iWidth].iIdx);
				OBJ_TILE->SetTileType(CMouseEventMgr::GetInst()->GetType());
				++iWidth;
			}
			++iHeight;
			iWidth = 0;
		}
	}
}

// 여기서 부터 만들면 된다.
void CStageTool::ObjPicking()
{
	if (!OnUI() && KEY(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY))
	{
		Vec2 StartMousePos = CKeyMgr::GetInst()->GetMousePos();
		StartMousePos = CCamMgr::GetInst()->GetCamMousePos(StartMousePos);

		if (StartMousePos.x < 0 || StartMousePos.x > 1600 ||
			StartMousePos.y < 0 || StartMousePos.y > 900)
			return;

		if (CMouseEventMgr::GetInst()->GetObjectName() == L"DirtySand_S")
		{
			int iIdxX = (int)StartMousePos.x / TILE_SIZE;
			int iIdxY = (int)StartMousePos.y / TILE_SIZE;
			StartMousePos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdxX + iIdxY * CStageMgr::GetInst()->GetTileSizeX()])->GetRealPos();
			StartMousePos.x += 32.f;
			StartMousePos.y += 32.f;
		}
		
		else if (CMouseEventMgr::GetInst()->GetObjectName() == L"DirtySand")
		{
			int iIdxX = (int)StartMousePos.x / TILE_SIZE;
			int iIdxY = (int)StartMousePos.y / TILE_SIZE;
			StartMousePos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdxX + iIdxY * CStageMgr::GetInst()->GetTileSizeX()])->GetRealPos();
			StartMousePos.x += 64.f;
			StartMousePos.y += 64.f;
		}

		CImageObj* pObj = new CImageObj;
		pObj->SetRealPos(StartMousePos);
		pObj->SetPos(CCamMgr::GetInst()->GetRealPos(StartMousePos.x, StartMousePos.y));
		pObj->SetTexture(CMouseEventMgr::GetInst()->GetTexture());
		pObj->SetSize(tSize(CMouseEventMgr::GetInst()->GetTexture()->GetWidth(), CMouseEventMgr::GetInst()->GetTexture()->GetHeight()));
		pObj->SetScale(Vec2(CMouseEventMgr::GetInst()->GetTexture()->GetWidth() * 4, CMouseEventMgr::GetInst()->GetTexture()->GetHeight() * 4));
		pObj->SetName(CMouseEventMgr::GetInst()->GetObjectName());
		m_vObj[(UINT)OBJ_TYPE::OBJECT].push_back(pObj);
	}
	
	if (m_vObj[(UINT)OBJ_TYPE::OBJECT].empty())
		return;
	
	if (KEY(KEY_TYPE::KEY_RBTN, KEY_STATE::AWAY))
	{
		delete m_vObj[(UINT)OBJ_TYPE::OBJECT].back();
		m_vObj[(UINT)OBJ_TYPE::OBJECT].pop_back();
	}
	else if (KEY(KEY_TYPE::KEY_W, KEY_STATE::TAB) || KEY(KEY_TYPE::KEY_W, KEY_STATE::HOLD))
	{
		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->GetRealPos();
		vPos.y -= 20.f * DT;
		((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->SetRealPos(vPos);
	}
	else if (KEY(KEY_TYPE::KEY_S, KEY_STATE::TAB) || KEY(KEY_TYPE::KEY_S, KEY_STATE::HOLD))
	{
		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->GetRealPos();
		vPos.y += 20.f * DT;
		((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->SetRealPos(vPos);
	}
	else if (KEY(KEY_TYPE::KEY_A, KEY_STATE::TAB) || KEY(KEY_TYPE::KEY_A, KEY_STATE::HOLD))
	{
		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->GetRealPos();
		vPos.x -= 20.f * DT;
		((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->SetRealPos(vPos);
	}
	else if (KEY(KEY_TYPE::KEY_D, KEY_STATE::TAB) || KEY(KEY_TYPE::KEY_D, KEY_STATE::HOLD))
	{
		Vec2 vPos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->GetRealPos();
		vPos.x += 20.f * DT;
		((CCamObj*)m_vObj[(UINT)OBJ_TYPE::OBJECT].back())->SetRealPos(vPos);
	}
}

void CStageTool::HiddenTilePicking()
{
	if (!OnUI() && KEY(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY))
	{
		Vec2 StartMousePos = CKeyMgr::GetInst()->GetMousePos();
		StartMousePos = CCamMgr::GetInst()->GetCamMousePos(StartMousePos);

		int iIdxX = (int)StartMousePos.x / TILE_SIZE;
		int iIdxY = (int)StartMousePos.y / TILE_SIZE;
		StartMousePos = ((CCamObj*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdxX + iIdxY * CStageMgr::GetInst()->GetTileSizeX()])->GetRealPos();

		CHiddenTile* pTile = new CHiddenTile;
		pTile->SetRealPos(StartMousePos);
		pTile->SetPos(StartMousePos);
		pTile->SetTexture(CMouseEventMgr::GetInst()->GetTexture());
		pTile->SetIndex(CMouseEventMgr::GetInst()->GetTileIndex());
		pTile->Init();
		//m_vecHidden[m_iHiddeniIdx].push_back(pTile);
			m_vObj[m_iHiddeniIdx].push_back(pTile);
	}
	else if (KEY(KEY_TYPE::KEY_RBTN, KEY_STATE::AWAY))
	{
		if (m_vObj[m_iHiddeniIdx].size() > 0)
		{
			delete m_vObj[m_iHiddeniIdx].back();
			m_vObj[m_iHiddeniIdx].pop_back();
		}
	}
	else if (KEY(KEY_TYPE::KEY_Q, KEY_STATE::AWAY))
		m_iHiddeniIdx = (UINT)OBJ_TYPE::HIDDEN;
	else if (KEY(KEY_TYPE::KEY_E, KEY_STATE::AWAY))
		m_iHiddeniIdx = (UINT)OBJ_TYPE::HIDDEN_TWO;
}

bool CStageTool::OnUI()
{
	bool bPick = false;
	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::UI].size(); ++i) // UI위에 있냐 없냐
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
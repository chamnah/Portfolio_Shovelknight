#include "stdafx.h"
#include "CStage.h"
#include "CTile.h"
#include "CKeyMgr.h"
#include "CStageMgr.h"
#include "CCore.h"
#include "CUI.h"
#include "CPathMgr.h"
#include <afxdlgs.h>
#include "CResMgr.h"

void CStage::Exit()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		for (UINT j = 0; j < m_vObj[i].size(); ++j)
		{
			delete m_vObj[i][j];
			m_vObj[i][j] = nullptr;
		}
		m_vObj[i].clear();
	}
}

void CStage::Render(HDC _hdc)
{
	static bool bColl = false;

	if (m_vObj[(UINT)OBJ_TYPE::TILE].size() != 0)
		m_vObj[(UINT)OBJ_TYPE::TILE][0]->render(_hdc);

	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		if (i == (UINT)OBJ_TYPE::TILE)
			continue;
		for (UINT j = 0; j < m_vObj[i].size(); ++j)
		{
			m_vObj[i][j]->render(_hdc);
		}
	}

	if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_1, KEY_STATE::TAB))
		bColl = !bColl;

	if (bColl)
	{
		if (m_vObj[(UINT)OBJ_TYPE::TILE].size() != 0)
		{
			Vec2 vDiff = CCamMgr::GetInst()->GetDifference();

			if (vDiff.x < 0)
				vDiff.x = 0;
			if (vDiff.y < 0)
				vDiff.y = 0;

			int iStartCol = int(vDiff.x / TILE_SIZE);
			int iStartLow = int(vDiff.y / TILE_SIZE);
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
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * GetTileSizeX() + j])->CollRender(_hdc);
				}
			}
		}
		/*
		for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
		{
			for (UINT j = 0; j < m_vObj[i].size(); ++j)
			{
				m_vObj[i][j]->CollisionRender(_hdc);
			}
		}
		*/
	}


	
}

int CStage::Update()
{
	Vec2 vDiff = CCamMgr::GetInst()->GetDifference();

	if (vDiff.x < 0)
		vDiff.x = 0;
	if (vDiff.y < 0)
		vDiff.y = 0;

	int iStartCol = int(vDiff.x / TILE_SIZE);
	int iStartLow = int(vDiff.y / TILE_SIZE);
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
			((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * GetTileSizeX() + j])->update();
		}
	}
	
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		if (i == (UINT)OBJ_TYPE::TILE)
			continue;
		for (UINT j = 0; j < m_vObj[i].size(); ++j)
		{
			m_vObj[i][j]->update();
		}
	}

	UICheck();
	
	return 0;
}

void CStage::ClearObj(int _iObj)
{
	for (UINT j = 0; j < m_vObj[_iObj].size(); ++j)
	{
		delete m_vObj[_iObj][j];
	}
	m_vObj[_iObj].clear();

}

void CStage::CreateTile(int iSizeX, int iSizeY, int iTileSize)
{
	m_iTileSizeX = iSizeX;
	m_iTileSizeY = iSizeY;

	for (UINT i = 0; i < m_iTileSizeY; ++i)
	{
		for (UINT j = 0; j < m_iTileSizeX; ++j)
		{
			CObj* pObj = new CTile(Vec2((j * iTileSize),(i * iTileSize)));
			m_vObj[(UINT)OBJ_TYPE::TILE].push_back(pObj);
		}
	}
}

void CStage::ChangeTile(int iSizeX, int iSizeY)
{
	vector<int> vecIdx;

	for (int i = 0; i < iSizeY; ++i)
	{
		for (int j = 0; j < iSizeX; ++j)
		{
			if (j >= (int)m_iTileSizeX || i >= (int)m_iTileSizeY)
				vecIdx.push_back(0);
			else
				vecIdx.push_back(((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][j + m_iTileSizeX * i])->GetIndex());
		}
	}

	ClearObj((int)OBJ_TYPE::TILE);

	for (int i = 0; i < iSizeY; ++i)
	{
		for (int j = 0; j < iSizeX; ++j)
		{
			CObj* pObj = new CTile(Vec2((j * TILE_SIZE), (i * TILE_SIZE)));
			((CTile*)pObj)->SetIdx(vecIdx[j + iSizeX * i]);
			m_vObj[(UINT)OBJ_TYPE::TILE].push_back(pObj);
		}
	}

	m_iTileSizeX = iSizeX;
	m_iTileSizeY = iSizeY;
}

void CStage::TileDCRender(HDC _dc)
{
	if (m_vObj[(UINT)OBJ_TYPE::TILE].empty())
	{
		assert(NULL);
	}

	for (UINT j = 0; j < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++j)
	{
		((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][j])->DCRender(_dc);
	}

	((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][0])->SetDC(_dc);
}

void CStage::UICheck()
{
	if (m_vObj[(UINT)OBJ_TYPE::UI].empty())
		return;

	vector<CObj*>::reverse_iterator riter = m_vObj[(UINT)OBJ_TYPE::UI].rbegin();

	for (; riter != m_vObj[(UINT)OBJ_TYPE::UI].rend(); ++riter)
	{
		if (UICheck_PreOrder((CUI*)*riter)) // 모든 UI에 마우스 올라왔는지 여부와 어떤 UI에 올라왔는지 혹은 클릭햇는지 까지 잡고 있다.
		{
			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB)) // 여기서는 포커싱만 바꿈
			{
				CObj* pUI = *riter;
				m_vObj[(UINT)OBJ_TYPE::UI].erase((++riter).base());
				m_vObj[(UINT)OBJ_TYPE::UI].push_back(pUI);
			}
			break; // 브레이크를 여기서 하는 것이 만약 포커싱된 녀석이 마우스가 올라가 있다면 다음 녀석은 검사도 안하고 탈출 하겠다는 의미이다.
			      // 포커싱이 안되어 있는 부분을 클릭하면 바뀌는 현상이 벌어진다.
		}
	}
}

bool CStage::UICheck_PreOrder(CUI* _pUI)
{
	// 일단 포커싱이 일어난 녀석이 먼저 들어올것이다.
	vector<CUI*> vecUI;
	vecUI.push_back(_pUI);

	// 이거는 매 프레임 마다 올라왔는지 여부를 검사한다.
	while (true) // 모든 UI에 마우스가 올라 왔는지 안 올라왔는지 값을 넣기 위해서 돌리는 구문이다.
	{
		if (vecUI.empty())
			break;

		CUI* pUI = vecUI.back();
		vecUI.pop_back();

		MouseOnOff(pUI); 

		for (UINT i = (UINT)UI_TYPE::NONE; i < (UINT)UI_TYPE::END; ++i)
		{
			for (UINT j = 0; j < pUI->GetChildVector()[i].size(); ++j)
			{
				vecUI.push_back(pUI->GetChildVector()[i][j]);
			}
		}
	}

	if(_pUI->GetIsMouse())
	vecUI.push_back(_pUI);
	CUI* pTargetUI = NULL;
	while (true)
	{
		if (vecUI.empty())
			break;

		pTargetUI = vecUI.back();
		vecUI.pop_back();

		for (int i = (UINT)UI_TYPE::END - 1; i >= 0; --i)
		{
			for (int j = pTargetUI->GetChildVector()[i].size() - 1; j >= 0; --j)
			{
				if(j == 0)
					int ia = 0;
				if (pTargetUI->GetChildVector()[i][j]->GetIsMouse())
					vecUI.push_back(pTargetUI->GetChildVector()[i][j]);
			}
		}
	}

	if (NULL != pTargetUI)
		MouseLBTN(pTargetUI);
	
	return pTargetUI;
}   

void CStage::MouseOnOff(CUI* _pUI)
{
	if (_pUI->IsMouseOn()) // 마우스가 UI 위에 올라와 있다
	{
		if (!_pUI->GetIsMouse())
			_pUI->MouseOn();
		_pUI->SetIsMouse(true);
	}
	else
	{
		if(_pUI->GetIsMouse())
			_pUI->MouseOut();
		_pUI->SetIsMouse(false);
	}
}

void CStage::MouseLBTN(CUI * _pUI)
{
	if (_pUI->GetIsMouse() && CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB))
	{
		_pUI->MouseDown();
		_pUI->SetMouseOnLB(true);
	}
	else if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::AWAY))
	{
		if (_pUI->GetIsMouse() && _pUI->GetMouseOnLB())
		{
			_pUI->MouseUp();
		}
		_pUI->SetMouseOnLB(false);
	}
}

void CStage::LoadTile(wstring _strPath)
{
	wstring strPath = CPathMgr::GetResPath();
	if (_strPath == L"")
	{
		strPath += L"Tile\\";
	
		// FileDialog
		wchar_t szFilter[50] = L"";
		CFileDialog dlg(FALSE, L"tile", L"*.tile",
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

		dlg.m_ofn.lpstrInitialDir = strPath.c_str();

		if (dlg.DoModal() != IDOK)
			return;

		strPath = dlg.GetPathName();
	}
	else
	{
		strPath += _strPath;
	}
	
	// strPath 경로로 파일 생성
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	// 타일 정보를 파일에 저장시킴
	// 1. Tile 의 가로, 세로 개수 와 타일 사이즈
	int iXCount = 0;
	int iYCount = 0;
	fread(&iXCount, sizeof(int), 1, pFile);
	fread(&iYCount, sizeof(int), 1, pFile);

	int iSize = iXCount * iYCount;

	int iTileSize = 0;
	fread(&iTileSize, sizeof(int), 1, pFile);

	ClearObj((int)OBJ_TYPE::TILE);
	CreateTile(iXCount, iYCount, iTileSize);

	// 2. 각 타일의 참조하는 이미지 인덱스
	int iImageIdx = 0;
	for (int i = 0; i < iSize; ++i)
	{
		iImageIdx = 0;
		fread(&iImageIdx, sizeof(int), 1, pFile);
		((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i])->SetIdx(iImageIdx);
	}

	int iTileType = 0;

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		fread(&iTileType, sizeof(int), 1, pFile);
		((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i])->SetTileType((TILE_TYPE)iTileType);
	}

	wchar_t Path[MAX_PATH] = L"";
	wchar_t Key[MAX_PATH] = L"";
	int PathSize = 0;
	int KeySize = 0;

	for (UINT i = 0; i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		fread(&PathSize,sizeof(int),1,pFile);
		fread(&KeySize, sizeof(int), 1, pFile);
		fread(Key, sizeof(wchar_t), KeySize, pFile);
		fread(Path,sizeof(wchar_t), PathSize,pFile);
		
		m_vObj[(UINT)OBJ_TYPE::TILE][i]->SetTexture(TEX_LOAD(Key, Path));
	}

	fclose(pFile);
}

CStage::CStage()
{
	m_vObj.resize((UINT)OBJ_TYPE::END);
}

CStage::~CStage()
{
	Exit();

	m_vObj.clear();
}
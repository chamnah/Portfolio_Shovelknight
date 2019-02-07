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
#include "CPlayer.h"
#include "CTextUI.h"
#include "CHpUI.h"
#include "CScoreUI.h"
#include "CGameMgr.h"

void CStage::Exit()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		if (!CStageMgr::GetInst()->GetCheck(i))
		{
			for (UINT j = 0; j < m_vObj[i].size(); ++j)
			{
				delete m_vObj[i][j];
				m_vObj[i][j] = nullptr;
			}
		}
		m_vObj[i].clear();
	}
}

void CStage::ReSetExit()
{
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		if (!CStageMgr::GetInst()->GetCheck(i))
		{
			for (UINT j = 0; j < m_vObj[i].size(); ++j)
			{
				delete m_vObj[i][j];
				m_vObj[i][j] = nullptr;
			}
			m_vObj[i].clear();
		}
	}
}

void CStage::Render(HDC _hdc)
{
	static bool bColl = false;

	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		if (i == (UINT)OBJ_TYPE::TILE)
		{
			if(m_vObj[(UINT)OBJ_TYPE::TILE].size() != 0)
				m_vObj[(UINT)OBJ_TYPE::TILE][0]->render(_hdc);
			continue;
		}

		if (CStageMgr::GetInst()->GetCurState() == STAGE::START)
			int i = 0;
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

			if (iStartCol + iEndCol >= (int)CStageMgr::GetInst()->GetTileSizeX())
				iEndCol = CStageMgr::GetInst()->GetTileSizeX() - 1 - iStartCol;

			if (iStartLow + iEndLow >= (int)CStageMgr::GetInst()->GetTileSizeY())
				iEndLow = CStageMgr::GetInst()->GetTileSizeY() - 1 - iStartLow;

			for (int i = iStartLow; i <= iStartLow + iEndLow; ++i)
			{
				for (int j = iStartCol; j <= iStartCol + iEndCol; ++j)
				{
					((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * CStageMgr::GetInst()->GetTileSizeX() + j])->CollRender(_hdc);
				}
			}
		}
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

	if (iStartCol + iEndCol >= (int)CStageMgr::GetInst()->GetTileSizeX())
		iEndCol = CStageMgr::GetInst()->GetTileSizeX() - 1 - iStartCol;

	if (iStartLow + iEndLow >= (int)CStageMgr::GetInst()->GetTileSizeY())
		iEndLow = CStageMgr::GetInst()->GetTileSizeY() - 1 - iStartLow;

	for (int i = iStartLow; i <= iStartLow + iEndLow; ++i)
	{
		for (int j = iStartCol; j <= iStartCol + iEndCol; ++j)
		{
			((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i * CStageMgr::GetInst()->GetTileSizeX() + j])->update();
		}
	}
	
	for (UINT i = 0; i < (UINT)OBJ_TYPE::END; ++i)
	{
		if (i == (UINT)OBJ_TYPE::TILE)
			continue;

		vector<CObj*>::iterator iter = m_vObj[i].begin();

		for (iter; iter != m_vObj[i].end();)
		{
			if ((*iter)->update() == INT_MAX)
			{
				delete *iter;
				iter = m_vObj[i].erase(iter);
			}
			else
				++iter;
		}
	}
	
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
	CStageMgr::GetInst()->SetTileSizeX(iSizeX);
	CStageMgr::GetInst()->SetTileSizeY(iSizeY);

	for (UINT i = 0; i < iSizeY; ++i)
	{
		for (UINT j = 0; j < iSizeX; ++j)
		{
			CObj* pObj = new CTile(Vec2((j * iTileSize) + m_vStartPos.x,(i * iTileSize) + m_vStartPos.y));
			m_vObj[(UINT)OBJ_TYPE::TILE].push_back(pObj);
		}
	}
}

void CStage::ChangeTile(int iSizeX, int iSizeY)
{
	vector<int> vecIdx;
	vector<int> vecType;

	for (int i = 0; i < iSizeY; ++i)
	{
		for (int j = 0; j < iSizeX; ++j)
		{
			if (j >= (int)CStageMgr::GetInst()->GetTileSizeX() || i >= (int)CStageMgr::GetInst()->GetTileSizeY())
			{
				vecIdx.push_back(0);
				vecType.push_back(0);
			}
			else
			{
				vecIdx.push_back(((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][j + CStageMgr::GetInst()->GetTileSizeX() * i])->GetIndex());
				vecType.push_back(int(((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][j + CStageMgr::GetInst()->GetTileSizeX() * i])->GetTileType()));
			}
		}
	}

	ClearObj((int)OBJ_TYPE::TILE);

	for (int i = 0; i < iSizeY; ++i)
	{
		for (int j = 0; j < iSizeX; ++j)
		{
			CObj* pObj = new CTile(Vec2((j * TILE_SIZE), (i * TILE_SIZE)));
			((CTile*)pObj)->SetIdx(vecIdx[j + iSizeX * i]);
			((CTile*)pObj)->SetTileType(TILE_TYPE(vecType[j + iSizeX * i]));
			m_vObj[(UINT)OBJ_TYPE::TILE].push_back(pObj);
		}
	}

	CStageMgr::GetInst()->SetTileSizeX(iSizeX);
	CStageMgr::GetInst()->SetTileSizeY(iSizeY);
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
		if (UICheck_PreOrder((CUI*)*riter)) // ��� UI�� ���콺 �ö�Դ��� ���ο� � UI�� �ö�Դ��� Ȥ�� Ŭ���޴��� ���� ��� �ִ�.
		{
			if (CKeyMgr::GetInst()->GetKeyState(KEY_TYPE::KEY_LBTN, KEY_STATE::TAB)) // ���⼭�� ��Ŀ�̸� �ٲ�
			{
				CObj* pUI = *riter;
				m_vObj[(UINT)OBJ_TYPE::UI].erase((++riter).base());
				m_vObj[(UINT)OBJ_TYPE::UI].push_back(pUI);
			}
			break; // �극��ũ�� ���⼭ �ϴ� ���� ���� ��Ŀ�̵� �༮�� ���콺�� �ö� �ִٸ� ���� �༮�� �˻絵 ���ϰ� Ż�� �ϰڴٴ� �ǹ��̴�.
			      // ��Ŀ���� �ȵǾ� �ִ� �κ��� Ŭ���ϸ� �ٲ�� ������ ��������.
		}
	}
}

bool CStage::UICheck_PreOrder(CUI* _pUI)
{
	// �ϴ� ��Ŀ���� �Ͼ �༮�� ���� ���ð��̴�.
	vector<CUI*> vecUI;
	vecUI.push_back(_pUI);

	// �̰Ŵ� �� ������ ���� �ö�Դ��� ���θ� �˻��Ѵ�.
	while (true) // ��� UI�� ���콺�� �ö� �Դ��� �� �ö�Դ��� ���� �ֱ� ���ؼ� ������ �����̴�.
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
	if (_pUI->IsMouseOn()) // ���콺�� UI ���� �ö�� �ִ�
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

void CStage::ArriveTile()
{
	for (UINT i = 0;i < m_vObj[(UINT)OBJ_TYPE::TILE].size(); ++i)
	{
		((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][i])->SetRealPos(m_vObj[(UINT)OBJ_TYPE::TILE][i]->GetPos() - m_vStartPos);
	}
}

void CStage::ReStart()
{
	CObj* pObj = new CPlayer;
	pObj->SetPos(CGameMgr::GetInst()->LastSave().vPos);
	pObj->Init();
	m_vObj[(UINT)OBJ_TYPE::PLAYER].push_back(pObj);

	pObj = new CUI;
	pObj->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"HUD", L"Image\\HUD.bmp"));
	pObj->SetPos(0, 0);
	m_vObj[(UINT)OBJ_TYPE::UI].push_back(pObj);

	CTextUI* pTexUI = new CTextUI;
	pTexUI->SetPos(35, 5);
	pTexUI->SetType(GOLD);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	pTexUI = new CTextUI;
	pTexUI->SetPos(335, 5);
	pTexUI->SetType(ITEM);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	pTexUI = new CTextUI;
	pTexUI->SetPos(670, 5);
	pTexUI->SetType(LIFE);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	pTexUI = new CTextUI;
	pTexUI->SetPos(1370, 5);
	pTexUI->SetType(BOSS);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pTexUI);

	CScoreUI* pScore = new CScoreUI;
	pScore->SetPos(50, 30);
	((CUI*)pObj)->AddChildUI(UI_TYPE::NONE, pScore);

	CHpUI* pHP = nullptr;

	for (int i = 0; i < (((CDynamicObj*)m_vObj[(UINT)OBJ_TYPE::PLAYER][0])->GetMaxHP() / 2); ++i)
	{
		pHP = new CHpUI;
		pHP->SetTexture((CTexture*)CResMgr::GetInst()->Load<CTexture*>(L"Life", L"Image\\Life.bmp"));
		pHP->SetPos(670 + i * 35, 33);
		pHP->SetScale(Vec2(32, 32));
		((CUI*)pObj)->AddChildUI(UI_TYPE::HP, pHP);
	}
}

void CStage::LoadTile(wstring _strPath, Vec2 vPos)
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
	
	// strPath ��η� ���� ����
	FILE* pFile = NULL;
	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	// Ÿ�� ������ ���Ͽ� �����Ŵ
	// 1. Tile �� ����, ���� ���� �� Ÿ�� ������
	int iXCount = 0;
	int iYCount = 0;
	fread(&iXCount, sizeof(int), 1, pFile);
	fread(&iYCount, sizeof(int), 1, pFile);

	int iSize = iXCount * iYCount;

	int iTileSize = 0;
	fread(&iTileSize, sizeof(int), 1, pFile);

	CreateTile(iXCount, iYCount, iTileSize);

	// 2. �� Ÿ���� �����ϴ� �̹��� �ε���
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
	:m_vPos{}
	, m_vStartPos{}
{
	m_vObj.resize((UINT)OBJ_TYPE::END);
}

CStage::~CStage()
{
	Exit();

	m_vObj.clear();
}
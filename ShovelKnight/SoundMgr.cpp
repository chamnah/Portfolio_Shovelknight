#include "stdafx.h"

#include "SoundMgr.h"
#include "CPathMgr.h"

#include "CCore.h"

// 전역 변수
bool g_bBGM = true;

CSoundMgr::CSoundMgr(void)
	: m_pSound(NULL)
	, m_pBGM(NULL)
	, m_lVolume(100)
	, m_bOffSound(false)
{
}

CSoundMgr::~CSoundMgr(void)
{
	map<wstring, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuffer.begin();
	for (iter; iter != m_mapSoundBuffer.end(); ++iter)
	{
		iter->second->Release();
	}
}

int CSoundMgr::init(void)
{
	if(FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
	{
		MessageBox(NULL, L"사운드디바이스생성실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	//사운드 디바이스 협조레벨 설정.
	HWND hWnd = CCore::GetInst()->GetHwnd();
	if(FAILED(m_pSound->SetCooperativeLevel( hWnd, DISCL_EXCLUSIVE))) // Flag값 정리
	{
		MessageBox(NULL, L"사운드디바이스 협조레벨 설정", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	return true;
}

int CSoundMgr::update(void)
{
	if( !m_strCurBGM.empty())
	{
		if( !g_bBGM && !m_bOffSound)
		{
			StopSound(m_strCurBGM, false);
			m_bOffSound = true; // 소리를 껏다.
		}		
		else if( g_bBGM && m_bOffSound)
		{
			PlayBGM(m_strCurBGM);
			m_bOffSound = false;
		}
	}
	return true;
}

int CSoundMgr::LoadWavSound(TCHAR *_pFileName,const TCHAR *_pResourceKey)
{
	HMMIO	hFile; // File Handle

	//CreateFile
	hFile = mmioOpen(_pFileName, NULL, MMIO_READ);//wave파일을 연다.

	//Chunk 청크 구조체, 문자열로 색인을 인식해서 WaveFormat 및 버퍼선언정보를 읽어온다.
	MMCKINFO	pParent;
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W','A','V','E');
	mmioDescend(hFile, &pParent, NULL, MMIO_FINDRIFF);

	MMCKINFO	pChild;
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX	wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, (char*)&wft, sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	DSBUFFERDESC	BuffInfo;
	memset(&BuffInfo, 0, sizeof(DSBUFFERDESC));
	BuffInfo.dwBufferBytes = pChild.cksize;
	BuffInfo.dwSize = sizeof(DSBUFFERDESC);
	BuffInfo.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE| DSBCAPS_CTRLVOLUME;
	BuffInfo.lpwfxFormat = &wft;

	LPDIRECTSOUNDBUFFER	pSoundBuff;

	if(FAILED(m_pSound->CreateSoundBuffer(&BuffInfo, &pSoundBuff, NULL)))
	{
		MessageBox(NULL, L"사운드버퍼생성실패", L"", MB_OK);
		return false;
	}

	void* pWrite1 = NULL;
	void* pWrite2 = NULL;
	DWORD dwlength1, dwlength2;

	pSoundBuff->Lock(0, pChild.cksize, &pWrite1, &dwlength1
		, &pWrite2, &dwlength2, 0L);

	if(pWrite1 > 0)
		mmioRead(hFile, (char*)pWrite1, dwlength1);
	if(pWrite2 > 0)
		mmioRead(hFile, (char*)pWrite2, dwlength2);

	pSoundBuff->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);	

	mmioClose(hFile, 0);

	// 맵에 사운드를 담는다.
	m_mapSoundBuffer.insert(make_pair(_pResourceKey, pSoundBuff));

	return true;
}

int CSoundMgr::LoadSound(const TCHAR *_pFileName,const TCHAR *_pResourceKey)
{
	if( !m_pSound )
		return false; // 사운드 객체 생성되지 않음

	if (m_mapSoundBuffer.find(_pFileName) != m_mapSoundBuffer.end())// 이미 파일이 로드되어있다.
		return false;
	
	// 파일명을 리소스 경로와 합친다.
	if( _pFileName == NULL )
		return true;

	TCHAR	szPath[255] = {0};
	wchar_t* pPath = CPathMgr::GetResPath();
	wcscpy_s(szPath, 255, pPath);
	
	wcscat_s(szPath, 255, L"\\Sound\\");
	wcscat_s(szPath, 255, _pFileName);

	// 확장자 이름 구별하기
	TCHAR _pExtension[10] = {0}; 
	
	int iSize = _tcslen( _pFileName );
	int iStart = 0;
		
	for(int i = 0; i < iSize; ++i)
	{
		if( _pFileName[i] == '.' )
		{
			iStart = i + 1;
			break;
		}
	}
	
	for( int i = iStart, j = 0; i < iSize; ++i, ++j)
	{
		_pExtension[j] = _pFileName[i];
	}

	if( !_tcscmp( _pExtension, _T("wav") ) ) // WAV 파일 로드
	{
		if( false == LoadWavSound(szPath, _pResourceKey))
			return false;
	}
	//else if( !_tcscmp( _pExtension, _T("ogg") ) ) // OGG 파일 로드
	//{
	//	if(RET_FAILED == LoadOggSound(szPath,_pResourceKey))
	//		return RET_FAILED;
	//}

	return true;
}

// Sound Play //
int CSoundMgr::Play( const wstring& _strSoundKey, const bool& _bLoop)
{
	// SoundMgr 에 해당 키로 등록된 사운드가 없다면
	map<wstring, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuffer.find( _strSoundKey);
	if( iter == m_mapSoundBuffer.end() )
	{
		MessageBox(NULL, L"사운드 재생 오류", L"사운드 파일 없음", MB_OK);
		return false;
	}
	
	// Play 함수의 1번째 2번째 인자는 0 으로 이미 예약되어있다.
	// 3번째 변수는 사운드를 반복재생 할 것인지 아닌지를 결정한다.
	if( _bLoop )
		iter->second->Play(0, 0, DSBPLAY_LOOPING);
	else
		iter->second->Play(0, 0, 0);

	return true;
}

int CSoundMgr::PlayBGM(const wstring& _strSoundKey, const bool& _bLoop)
{
	// SoundMgr 에 해당 키로 등록된 사운드가 없다면
	map<wstring, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuffer.find( _strSoundKey);
	if( iter == m_mapSoundBuffer.end() )
	{
		MessageBox(NULL, L"사운드 재생 오류", L"사운드 파일 없음", MB_OK);
		return false;
	}
	
	m_strCurBGM = _strSoundKey;

	// BGM 은 1개로 유지한다.
	//SUCCEEDED(m_pBGM->GetStatus((LPDWORD)DSBSTATUS_PLAYING))
	if(m_pBGM)
	{
		// BGM 이 있고, BGM 이 재생중이라면 Stop
		m_pBGM->Stop();
		m_pBGM->SetCurrentPosition( 0 );
	}

	// 새로운 BGM 으로 바꾼다.
	m_pBGM = iter->second;
	m_pBGM->SetVolume( m_lVolume );
	
	if( _bLoop )
		m_pBGM->Play(0, 0, DSBPLAY_LOOPING);
	else
		m_pBGM->Play(0, 0, 0);

	return true;
}

void CSoundMgr::StopSound(const wstring& _strSoundKey, const bool& _bReset)
{
	map<wstring, LPDIRECTSOUNDBUFFER>::iterator iter = m_mapSoundBuffer.find( _strSoundKey);
	if( iter != m_mapSoundBuffer.end() )
	{
		iter->second->Stop();

		if (m_pBGM == iter->second)
			m_pBGM = NULL;

		if( _bReset )
			iter->second->SetCurrentPosition( 0 );
	}
}

void CSoundMgr::SetBGMVolume(const double& _dVolume)
{
	double dVolume = _dVolume;
	if( dVolume > 100.0 )
		dVolume = 100.0;
	else if( dVolume <= 0.0 )
		dVolume = 0.00001;

	// 1 ~ 100 사이값을 데시벨 단위로 변경
	m_lVolume = (LONG)(-2000.0 * log10( 100.0 / dVolume ));
	if( m_lVolume < -10000 )
		m_lVolume = -10000;

	if( m_pBGM )
		m_pBGM->SetVolume( m_lVolume );
}

const long& CSoundMgr::GetBGMVolume(void)
{
	return m_lVolume;
}
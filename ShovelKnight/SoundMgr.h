#pragma once

class CSoundMgr
{
public:
	static CSoundMgr* GetInst() { static CSoundMgr mgr; return &mgr; }

private:
	LPDIRECTSOUND8		m_pSound; // 사운드카드 대표 객체

	// Sound Buffer Container //
	map<wstring, LPDIRECTSOUNDBUFFER> m_mapSoundBuffer; // Key 값으로 관리	

private:
	LPDIRECTSOUNDBUFFER				  m_pBGM;	// 배경음 버퍼
	LONG							  m_lVolume;// 배경음 볼륨(단위 데시벨) -10000 ~ 0 사이의 값
	wstring							  m_strCurBGM; // 현재 재생중인 BGM
	bool							  m_bOffSound;

public:
	int init(void);
	int update(void);
	int LoadSound(const TCHAR* _pFileName,const TCHAR* _pResourceKey);

public: // Playe Sound //
	int Play( const wstring& _strSoundKey, const bool& _bLoop);
	int PlayBGM( const wstring& _strSoundKey, const bool& _bLoop = true);

public: // Stop Sound //
	// bReset == true Stop 되는 사운드가 처음위치로 리셋될지 아닐지
	void StopSound( const wstring& _strSoundKey, const bool& _bReset = true);

private:
	int  LoadWavSound( TCHAR* _pFileName ,const TCHAR* _pResourceKey);	

	// Set //
public:
	void SetBGMVolume(const  double& _dVolume); // 1 ~ 100 사이값

public:
	// Get //
	const long& GetBGMVolume(void);

public:
	CSoundMgr(void);
	~CSoundMgr(void);
};
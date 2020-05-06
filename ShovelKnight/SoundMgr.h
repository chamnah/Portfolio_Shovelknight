#pragma once

class CSoundMgr
{
public:
	static CSoundMgr* GetInst() { static CSoundMgr mgr; return &mgr; }

private:
	LPDIRECTSOUND8		m_pSound; // ����ī�� ��ǥ ��ü

	// Sound Buffer Container //
	map<wstring, LPDIRECTSOUNDBUFFER> m_mapSoundBuffer; // Key ������ ����	

private:
	LPDIRECTSOUNDBUFFER				  m_pBGM;	// ����� ����
	LONG							  m_lVolume;// ����� ����(���� ���ú�) -10000 ~ 0 ������ ��
	wstring							  m_strCurBGM; // ���� ������� BGM
	bool							  m_bOffSound;

public:
	int init(void);
	int update(void);
	int LoadSound(const TCHAR* _pFileName,const TCHAR* _pResourceKey);

public: // Playe Sound //
	int Play( const wstring& _strSoundKey, const bool& _bLoop);
	int PlayBGM( const wstring& _strSoundKey, const bool& _bLoop = true);

public: // Stop Sound //
	// bReset == true Stop �Ǵ� ���尡 ó����ġ�� ���µ��� �ƴ���
	void StopSound( const wstring& _strSoundKey, const bool& _bReset = true);

private:
	int  LoadWavSound( TCHAR* _pFileName ,const TCHAR* _pResourceKey);	

	// Set //
public:
	void SetBGMVolume(const  double& _dVolume); // 1 ~ 100 ���̰�

public:
	// Get //
	const long& GetBGMVolume(void);

public:
	CSoundMgr(void);
	~CSoundMgr(void);
};
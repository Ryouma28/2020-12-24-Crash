//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �\���̒�`
//=============================================================================
typedef struct
{
	char *pFilename;			// �t�@�C����
	int nCntLoop;				// ���[�v�J�E���g
} SOUNDPARAM;

//=============================================================================
// �T�E���h�t�@�C��
//=============================================================================
typedef enum
{
	SOUND_LABEL_BGM_TiTle = 0,				// �^�C�g��
	SOUND_LABEL_BGM_Game,					// �Q�[��
	SOUND_LABEL_BGM_Result,					// ���U���g
	SOUND_LABEL_SE_Car_Near_Ring,			// ����
	SOUND_LABEL_SE_Roll,					// �]����
	SOUND_LABEL_SE_Decision,				// ����
	SOUND_LABEL_MAX
} SOUND_LABEL;

//=============================================================================
// �N���X��`
//=============================================================================
class CSoundCallback : public IXAudio2VoiceCallback {
public:
	CSoundCallback() { }
	~CSoundCallback() { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 BytesRequired) { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd(void) { }
	void STDMETHODCALLTYPE OnStreamEnd(void) { SetEvent(m_hEvent); }
	void STDMETHODCALLTYPE OnBufferStart(void *pBufferContext) { SetEvent(m_hEvent); }
	void STDMETHODCALLTYPE OnBufferEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnLoopEnd(void *pBufferContext) { }
	void STDMETHODCALLTYPE OnVoiceError(void *pBufferContext, HRESULT Error) { }
private:
	HANDLE m_hEvent = NULL;
};

class CSound
{
public:
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	void SetVolume(SOUND_LABEL label, float fVolume);

	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SubmixVoice *m_pSubmixVoice = NULL;					// �T�u�~�b�N�X�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X

	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/sound/bgm/Title.wav", -1 },						// �^�C�g��
		{ "data/sound/bgm/game.wav", -1 },						// �Q�[��
		{ "data/sound/bgm/Result.wav", -1 },					// ���U���g
		{ "data/sound/se/explosion.wav", 0 },					// ����
		{ "data/sound/se/korokoro.wav", 0 },					// �]����
		{ "data/sound/se/explosion.wav", 0 },					// ����				
	};
};
#endif
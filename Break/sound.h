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
	SOUND_LABEL_BGM_Character_Select,		// �L�����N�^�[�Z���N�g
	SOUND_LABEL_BGM_Puzzle,					// �p�Y��
	SOUND_LABEL_BGM_Race,					// ���[�X
	SOUND_LABEL_BGM_Result,					// ���U���g
	SOUND_LABEL_SE_Car_Near_Ring,			// �Ԃ̋߂��ɂȂ�����炷
	SOUND_LABEL_SE_Cheers,					// ����
	SOUND_LABEL_SE_Cursor,					// �J�[�\��
	SOUND_LABEL_SE_CurveWarning,			// �J�[�u����
	SOUND_LABEL_SE_Decision,				// ����
	SOUND_LABEL_SE_Draft_1,					// �h���t�g1
	SOUND_LABEL_SE_Draft_2,					// �h���t�g2
	SOUND_LABEL_SE_Draft_3,					// �h���t�g3
	SOUND_LABEL_SE_EndPuzzle_Standby,		// �p�Y�����������ăX�^���o�C
	SOUND_LABEL_SE_GearChange,				// �M�A�`�F���W
	SOUND_LABEL_SE_Horn,					// �N���N�V����
	SOUND_LABEL_SE_MaxSpeed,				// �}�b�N�X�X�s�[�h
	SOUND_LABEL_SE_PartsInset_StatusBig,	// �s�[�X�Z�b�g(�X�e�[�^�X��)
	SOUND_LABEL_SE_PartsInset_StatusSmall,	// �s�[�X�Z�b�g(�X�e�[�^�X��)
	SOUND_LABEL_SE_PartsInset_UIBig,		// �s�[�X�Z�b�g(UI��)
	SOUND_LABEL_SE_PartsInset_UISmall,		// �s�[�X�Z�b�g(UI��)
	SOUND_LABEL_SE_PullOut_1,				// ��������1
	SOUND_LABEL_SE_PullOut_2,				// ��������2
	SOUND_LABEL_SE_PullOut_3,				// ��������3
	SOUND_LABEL_SE_PuzzleComplete_1,		// �p�Y������1
	SOUND_LABEL_SE_PuzzleComplete_2,		// �p�Y������2
	SOUND_LABEL_SE_RaceStart,				// ���[�X�J�n
	SOUND_LABEL_SE_RaceStart_Overlapping,	// ���[�X�J�n�d��
	SOUND_LABEL_SE_Speed50Km,				// �X�s�[�h50�L���ɂȂ�����
	SOUND_LABEL_SE_Speed100Km,				// �X�s�[�h100�L���ɂȂ�����
	SOUND_LABEL_SE_SpeedUpPanel,			// �X�s�[�h�p�l��
	SOUND_LABEL_SE_StopHolding,				// ��~
	SOUND_LABEL_SE_WithParts,				// �p�[�c�t��
	SOUND_LABEL_SE_kyuin,					// ���ア��
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
		{ "data/sound/bgm/TiTle.wav", -1 },						// �^�C�g��
		{ "data/sound/bgm/CharacterSelect.wav", -1 },			// �L�����N�^�[�Z���N�g
		{ "data/sound/bgm/Puzzle.wav", -1 },					// �p�Y��
		{ "data/sound/bgm/race.wav", -1 },						// ���[�X
		{ "data/sound/bgm/Result.wav", -1 },					// ���U���g
		{ "data/sound/se/car_near_Ring.wav", 0 },				// �Ԃ̋߂��ɂȂ�����炷
		{ "data/sound/se/Cheers.wav", 0 },						// ����
		{ "data/sound/se/Cursor.wav", 0 },						// �J�[�\��
		{ "data/sound/se/CurveWarning.wav", 0 },				// �J�[�u����
		{ "data/sound/se/Decision.wav", 0 },					// ����
		{ "data/sound/se/draft_1.wav", 0 },						// �h���t�g1
		{ "data/sound/se/draft_2.wav", 0 },						// �h���t�g2
		{ "data/sound/se/draft_3.wav", 0 },						// �h���t�g3
		{ "data/sound/se/EndPuzzle_Standby.wav", 0 },			// �p�Y�����������ăX�^���o�C
		{ "data/sound/se/GearChange.wav", 0 },					// �M�A�`�F���W
		{ "data/sound/se/Horn.wav", 0 },						// �N���N�V����
		{ "data/sound/se/MaxSpeed.wav", -1 },					// �}�b�N�X�X�s�[�h
		{ "data/sound/se/PartsInset_StatusBig.wav", 0 },		// �s�[�X�Z�b�g(�X�e�[�^�X��)
		{ "data/sound/se/PartsInset_StatusSmall.wav", 0 },		// �s�[�X�Z�b�g(�X�e�[�^�X��)
		{ "data/sound/se/PartsInset_UIBig.wav", 0 },			// �s�[�X�Z�b�g(UI��)
		{ "data/sound/se/PartsInset_UISmall.wav", 0 },			// �s�[�X�Z�b�g(UI��)
		{ "data/sound/se/PullOut_1.wav", 0 },					// ��������1
		{ "data/sound/se/PullOut_2.wav", 0 },					// ��������2
		{ "data/sound/se/PullOut_3.wav", 0 },					// ��������3
		{ "data/sound/se/PuzzleComplete_1.wav", 0 },			// �p�Y������1
		{ "data/sound/se/PuzzleComplete_2.wav", 0 },			// �p�Y������2
		{ "data/sound/se/RaceStart.wav", 0 },					// ���[�X�J�n
		{ "data/sound/se/RaceStart_Overlapping.wav", 0 },		// ���[�X�J�n�d��
		{ "data/sound/se/Speed50Km.wav", 0 },					// �X�s�[�h50�L���ɂȂ�����
		{ "data/sound/se/Speed100Km.wav", 0 },					// �X�s�[�h100�L���ɂȂ�����
		{ "data/sound/se/SpeedUpPanel.wav", 0 },				// �X�s�[�h�p�l��
		{ "data/sound/se/StopHolding.wav", 0 },					// ��~
		{ "data/sound/se/WithParts.wav", 0 },					// �p�[�c�t��
		{ "data/sound/se/�L���C��_01.wav", 0 },					// ���ア��							
	};
};
#endif
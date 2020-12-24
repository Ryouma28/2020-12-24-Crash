//=============================================================================
//
// サウンド処理 [sound.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 構造体定義
//=============================================================================
typedef struct
{
	char *pFilename;			// ファイル名
	int nCntLoop;				// ループカウント
} SOUNDPARAM;

//=============================================================================
// サウンドファイル
//=============================================================================
typedef enum
{
	SOUND_LABEL_BGM_TiTle = 0,				// タイトル
	SOUND_LABEL_BGM_Game,					// ゲーム
	SOUND_LABEL_BGM_Result,					// リザルト
	SOUND_LABEL_SE_Car_Near_Ring,			// 爆発
	SOUND_LABEL_SE_Roll,					// 転がる
	SOUND_LABEL_SE_Decision,				// 決定
	SOUND_LABEL_MAX
} SOUND_LABEL;

//=============================================================================
// クラス定義
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

	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// マスターボイス
	IXAudio2SubmixVoice *m_pSubmixVoice = NULL;					// サブミックスボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス

	BYTE *m_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

	// 各音素材のパラメータ
	SOUNDPARAM m_aParam[SOUND_LABEL_MAX] =
	{
		{ "data/sound/bgm/Title.wav", -1 },						// タイトル
		{ "data/sound/bgm/game.wav", -1 },						// ゲーム
		{ "data/sound/bgm/Result.wav", -1 },					// リザルト
		{ "data/sound/se/explosion.wav", 0 },					// 爆発
		{ "data/sound/se/korokoro.wav", 0 },					// 転がる
		{ "data/sound/se/explosion.wav", 0 },					// 決定				
	};
};
#endif
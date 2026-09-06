//=============================================================================
//
// サウンド処理 [sound.h]
// Author : AKIRA TANAKA
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_	// 二重インクルード防止

//****************************************************
// インクルードファイル
//****************************************************
#include "main.h"

//****************************************************
// サウンドクラス
//****************************************************
class CSound
{
public:

	//****************************************************
	// サウンド一覧
	//****************************************************
	enum SOUND_LABEL
	{
		SOUND_LABEL_00 = 0,	// ダミー
		SOUND_LABEL_MAX,
	};

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ

	HRESULT Init(HWND hWnd);				// 初期設定
	void Uninit(void);						// 終了処理
	HRESULT PlaySound(SOUND_LABEL label);	// 再生
	void Stop(SOUND_LABEL label);			// 停止 (選択)
	void Stop(void);						// 停止 (全て)
	void SetVol(int label);					// 音量を設定
	float GetVol(int label);				// 音量を取得

private:

	//****************************************************
	// サウンド情報の構造体
	//****************************************************
	struct SOUNDINFO
	{
		const char* pFilename;	// ファイル名
		int nCntLoop;			// ループカウント
		float fVolume;			// 音量
	};

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);	// チャンクのチェック
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータの読み込み

	IXAudio2* m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE* m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];				// サウンド情報 (仮)
};

#endif	// _SOUND_H_

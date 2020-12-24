//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	 MAX_RANK 6							// スコアの最大数

//=============================================================================
// 前方宣言
//=============================================================================
class CNumber;

//=============================================================================
// クラス定義
//=============================================================================
class CRanking
{
public:
	CRanking();										// コンストラクタ
	~CRanking();									// デストラクタ
	HRESULT Init(void);								// 初期化処理
	void Uninit(void);								// 開放処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理
	static void SetResultIndex(int nScore);			// 結果の設定

private:
	void ConvertRanking(int nNumber);				// 結果の整理処理
	void SaveRanking(void);							// ランキング保存処理
	void LoadRanking(void);							// ランキング読み込み処理

	CNumber *m_apRank[MAX_RANK - 1];				// Top5
	CUi *m_pUi;
	int m_aScore[MAX_RANK];							// 全スコア
	static int m_nResult;							// 今回の得点
};
#endif
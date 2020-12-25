//=============================================================================
//
// エンドシグナル処理 [endSignal.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _ENDSIGNAL_H_
#define _ENDSIGNAL_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// クラス定義
//=============================================================================
class CEndSignal : public CScene
{
public:
	CEndSignal(PRIORITY obj);					// コンストラクタ
	~CEndSignal();							// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 開放処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	static CEndSignal *Create(void);			// クリエイト処理

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	CUi *m_pUi;
	bool m_bEnd;
};
#endif
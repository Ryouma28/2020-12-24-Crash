//=============================================================================
//
// fever処理 [fever.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _FEVER_H_
#define _FEVER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// 前方宣言
//=============================================================================
class CTime;
class CScene2D;

//=============================================================================
// クラス定義
//=============================================================================
class CFever : public CScene
{
public:
	CFever(PRIORITY obj);					// コンストラクタ
	~CFever();							// デストラクタ
	HRESULT Init(void);						// 初期化処理
	void Uninit(void);						// 開放処理
	void Update(void);						// 更新処理
	void Draw(void);						// 描画処理

	static CFever *Create(void);			// クリエイト処理

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	CTime *m_pTime;
	CUi *m_pUi;

	CScene2D *m_pScene2D;

	bool m_bEnd;
};
#endif
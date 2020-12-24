//=============================================================================
//
// sceneX処理 [sceneX.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// 列挙体定義
//=============================================================================
typedef enum
{
	SHADERTYPE_LAMBERT = 0,
	SHADERTYPE_TOON,
	SHADERTYPE_MAX
} SHADERTYPE;

//=============================================================================
// クラス定義
//=============================================================================
class CSceneX : public CScene
{
public:
	CSceneX(PRIORITY obj);				// コンストラクタ
	~CSceneX();							// デストラクタ
	HRESULT Init(void);					// 初期化処理
	void Uninit(void);					// 開放処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理
	void DrawSnow(void);				// ボールの描画

	static CSceneX *Create(void);		// クリエイト処理
	void BindModel(LPD3DXMESH pMesh, DWORD nNumMat, LPD3DXBUFFER pBuffMat);	// モデルの設定

	void SetColor(D3DXCOLOR col) { m_col = col; }					// 色を設定する
	void SetSize(D3DXVECTOR3 size) { m_size = size; }				// 大きさを設定する
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }				// 回転値を設定
	void SetShader(SHADERTYPE type) { m_ShaderType = type; }		// シェーダータイプを設定

	static void Load(void);											// シェーダーの初期化処理

	D3DXCOLOR GetColor(void) { return m_col; }						// 色の取得
	D3DXVECTOR3 GetSize(void) { return m_size; }					// 大きさの取得
	D3DXVECTOR3 GetRotation(void) { return m_rot; }					// 回転値の取得
	SHADERTYPE GetShaderType(void) { return m_ShaderType; }			// シェーダータイプの取得
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }				// ワールドマトリックス情報の取得
	void SetvecAxis(D3DXVECTOR3 axis) { m_vecAxis = axis; }			// 回転軸の設定
	D3DXVECTOR3 GetvecAxis(void) { return m_vecAxis; }				// 回転軸の取得
	void SetfValueRot(float fValueRot) { m_fValueRot = fValueRot; }	// 回転角の設定
	float GetfValueRot(void) { return m_fValueRot; }				// 回転角の取得

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void);

private:
#ifdef _DEBUG
	void Debug(void);
#endif
	void SetShaderParameter(LPD3DXEFFECT &pShader);

	LPDIRECT3DTEXTURE9	*m_pTexture;								// テクスチャへのポインタ
	D3DXVECTOR3 m_size;												// 大きさ
	D3DXVECTOR3 m_rot;												// 回転量
	D3DXCOLOR	m_col;												// 色
	D3DXMATRIX	m_mtxWorld;											// ワールドマトリックス

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;								// 頂点バッファへのポインタ
	LPD3DXMESH		m_pMesh;										// メッシュ情報へのポインタ
	DWORD			m_nNumMat;										// マテリアル情報の数
	LPD3DXBUFFER	m_pBuffMat;										// マテリアル情報へのポインタ
	SHADERTYPE		m_ShaderType;									// シェーダータイプ

	int m_nLife;													//ライフ
	int m_nLifeMax;
	float m_fAngle;													//角度
	float m_fLength;												//長さ

	D3DXMATRIX m_mtxRot;	//回転マトリックス(保存用)
	D3DXQUATERNION m_quat;	//クォータニオン
	D3DXVECTOR3 m_vecAxis;	//回転軸
	float m_fValueRot;		//回転角(回転量)

};
#endif
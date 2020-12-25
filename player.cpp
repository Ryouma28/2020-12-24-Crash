//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "scene3D.h"
#include "meshField.h"
#include "model.h"
#include "meshCapsule.h"
#include "colliderSphere.h"
#include "meshCube.h"
#include "enemy.h"
#include "camera.h"
#include "colliderBox.h"
#include "stage.h"
#include "sound.h"
#include "scene2D.h"
#include "effect.h"
#include "result.h"
#include "fade.h"
#include "takaseiLibrary.h"
#include "sceneX.h"
#include "speed.h"
#include "wall.h"
#include "object.h"
#include "number.h"
#include "network.h"
#include "distanceNext.h"
#include "ui.h"
#include "shadow.h"
#include "meshSphere.h"
#include "counter.h"

//=============================================================================
// 前方宣言
//=============================================================================
LPD3DXMESH CPlayer::m_pMeshModel = NULL;
LPD3DXBUFFER CPlayer::m_pBuffMatModel = NULL;
DWORD CPlayer::m_nNumMatModel = NULL;

//=============================================================================
// マクロ定義
//=============================================================================
#define	SCRIPT_CAR01 "data/animation/car01.txt"		// 車01のモデル情報アドレス
#define	SCRIPT_CAR02 "data/animation/car02.txt"		// 車01のモデル情報アドレス
#define	SCRIPT_CAR03 "data/animation/car03.txt"		// 車01のモデル情報アドレス
#define	SCRIPT_CAR04 "data/animation/car04.txt"		// 車01のモデル情報アドレス

#define ROT_AMOUNT 0.1f								// 回転の差を減らしていく量
#define ROT_SPEED_DRIFT 0.5f						// ドリフト時回転速度
#define MODEL_FRONT 2								// モデル前輪番号
#define MODEL_REAR 1								// モデル後輪番号
#define MODEL_TIRE 2								// タイヤモデルの数
#define CAMERA_ROT_SPEED 0.4f						// カメラの回転速度
#define TIRE_ROT_SPEED 0.1f							// タイヤの回転速度
#define ACCEKERATION 3.0f							// ドリフト加速度初期値
#define ACCEKERATION_ADDITION 0.5f					// ドリフト加速度加算量
#define DRIFT_DECREACE 0.6f							// ドリフト時速度減少
#define DRIFT_DEST 0.25f							// ドリフト時タイヤの向き
#define	INIT_ROT 3.14f
#define PLAYER_SPEED 5.0f							// プレイヤー速度

//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CSceneX(obj)
{
	// 優先度の設定
	SetObjType(CScene::PRIORITY_PLAYER);				// オブジェクトタイプ
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	m_vecAxis = ZeroVector3;							// 回転軸の初期化
	m_fValueRot = 0.0f;									// 回転角の初期化
	m_nLife = 100;										// 体力の初期化
	m_rot = D3DXVECTOR3(0.0f, INIT_ROT, 0.0f);			// 回転の初期化
	m_fSpeed = 0;										// スピードの初期化
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動量の初期化
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 移動先の初期化
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 差の初期化
	m_vectorOld = ZeroVector3;							// 前回のベクトル向き
	m_cameraRot = D3DXVECTOR3(0, D3DX_PI, 0);			// カメラの回転情報初期化
	m_pColPlayerSphere = NULL;							// プレイヤー当たり判定ポインタの初期化
	m_pColPlayerBox = NULL;
	m_pDistanceNext = NULL;								// 次のプレイヤーとの距離のUI
	m_bHit = false;										// 当たり判定フラグの初期亜化
	m_bJump = false;									// ジャンプフラグの初期化
	m_nActionCount = 0;									// アクションカウンタの初期化
	m_nParticleCount = 0;								// パーティクルカウンタの初期化
	m_nPointNum = 0;									// ポイント番号初期化
	m_fDeathblow = 0.0f;								// 必殺技ポイントの初期化
	m_fAcceleration = ACCEKERATION;						// 加速度
	m_bEvent = true;									// イベント発生フラグの初期化
	m_bMove = false;									// 現在動いているかのフラグ
	m_bAccel = false;									// アクセルを押しているかのフラグ
	m_bColliderWithWall = true;							// 壁の当たり判定
	m_bGoal = false;									// ゴールフラグ
	m_nPoint = 0;
	m_pUi = NULL;
	m_nCntSound = 0;
	m_bFever = false;

	m_pRank = NULL;

	m_nRound = 0;			// 現在の周回回数

	for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
	{
		m_bDrift[nCnt] = false;							// ドリフトフラグ判定
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPlayer::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();
	CNetwork *pNetwork = CManager::GetNetwork();

	//デバイスを取得する
	pDevice = pRenderer->GetDevice();
	m_fPuzzleMaxSPeed = 0;
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = GetPosition();							// プレイヤーの位置取得

	pos = D3DXVECTOR3(7879.83f, 350.0f, -741.0f);			// プレイヤーの位置設定

	m_pUi = CUi::Create();

	if (m_pUi != NULL)
	{
		m_pUi->LoadScript("data/text/ui/score.txt");
		m_pUi->SetPosition(D3DXVECTOR3(874.00, 51.00, 0.00));

		CCounter *pCounter = m_pUi->GetCounter("time");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(0);
		}
	}
	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	// キャラクターの初期化処理
	CSceneX::Init();

	// プレイヤーの当たり判定を生成
	m_pColPlayerSphere = CColliderSphere::Create(false, 500.0f);

	if (m_pColPlayerSphere != NULL)
	{ //球体のポインタがNULLではないとき
		m_pColPlayerSphere->SetScene(this);
		m_pColPlayerSphere->SetTag("player");										// タグ の設定
		m_pColPlayerSphere->SetPosition(pos);										// 位置 の設定
		m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// プレイヤーの当たり判定を生成
	m_pColPlayerBox = CColliderBox::Create(true, D3DXVECTOR3(500.0f, 500.0f, 500.0f));

	if (m_pColPlayerBox != NULL)
	{ //球体のポインタがNULLではないとき
		m_pColPlayerBox->SetScene(this);
		m_pColPlayerBox->SetTag("player");										// タグ の設定
		m_pColPlayerBox->SetPosition(pos);										// 位置 の設定
		m_pColPlayerBox->SetOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	}

	// 位置の設定
	SetPosition(pos);

	// 影の生成
	//m_pShadow = CShadow::Create();
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pColPlayerSphere != NULL)
	{// 武器の当たり判定が存在していたとき
		m_pColPlayerSphere->Release();
	}

	if (m_pDistanceNext != NULL)
	{
		m_pDistanceNext->Uninit();
		delete m_pDistanceNext;
		m_pDistanceNext = NULL;
	}

	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos;
	CSound *pSound = CManager::GetSound();
	CNetwork *pNetwork = CManager::GetNetwork();
	float fHeight = 0.0f;
	//CModel *pModel = GetModel();

	pos = GetPosition();				// 位置の取得

	//// アニメーション情報の取得
	//ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	//int currentKey = GetCurrentKey();
	//int currentFrame = GetCurrentFrame();
	//bool bAnimPlayState = GetAnimPlayState();

	// 入力処理
	if (!m_bHit)
	{// 当たっていなかったとき
		CCamera *pCamera = CManager::GetCamera();
		if (pCamera->GetStalker())
		{
			// 入力処理
			Input(pos);
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	VERTEX_PLANE plane = {};

	//CCollider::RayBlockCollision(pos, &pModel[0].GetMtxWorld(), 110, 250.0f, plane);

	D3DXVECTOR3 AB = plane.a - plane.b;
	D3DXVECTOR3 BC = plane.b - plane.c;

	D3DXVECTOR3 norwork;

	D3DXVec3Cross(&norwork, &BC, &AB);
	D3DXVec3Normalize(&norwork, &norwork);

	//CDebugProc::Log("a地点 : %f, %f, %f\n", plane.a.x, plane.a.y, plane.a.z);
	//CDebugProc::Log("b地点 : %f, %f, %f\n", plane.b.x, plane.b.y, plane.b.z);
	//CDebugProc::Log("c地点 : %f, %f, %f\n", plane.c.x, plane.c.y, plane.c.z);

	//床の高さを取得する
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// シーンの先頭アドレスを取得

	CScene *pScene = NowFloor(pos);													// 現在いるフィールドを取得

	if (pScene != NULL)
	{// 今立っている床が存在したとき
		m_size = GetSize();
		CMeshField *pFloor = (CMeshField*)pScene;									// キャスト
		fHeight = pFloor->GetHeight(pos);
		fHeight += 350.0f;
		pos.y = fHeight + (m_size.y * 5);

		RANDTYPE Type = pFloor->GetRandType();

		if (fabs(m_move.x) > 1.0f || fabs(m_move.y) > 1.0f || fabs(m_move.z) > 1.0f)
		{
			if (m_nCntSound > 50)
			{
				CSound *pSound = CManager::GetSound();				// サウンドの取得
				pSound->PlaySoundA(SOUND_LABEL_SE_Roll);			// ダメージ音の再生
				m_nCntSound = 0;
			}
			else
			{
				m_nCntSound++;
			}

			if (Type == RANDTYPE_GRASS)
			{
				if (m_size.x < 100000.0f)
				{

				}
			}
			else if (Type == RANDTYPE_SAND)
			{
				//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
			}
		}
		//if (animType == ANIMATIONTYPE_RUN)
		//{
		//	if (currentKey == 5 || currentKey == 0)
		//	{
		//		if (currentFrame == 0)
		//		{
		//			if (Type == RANDTYPE_GRASS)
		//			{
		//				//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_GRASS_1));
		//			}
		//			else if (Type == RANDTYPE_SAND)
		//			{
		//				//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
		//			}
		//		}
		//	}
		//}
	}

	// 位置更新
	pos += m_move;

	// ジャンプしていないとき
	if (!m_bJump)
	{
		//// 減速
		//m_move.x += (0 - m_move.x) * CManager::GetSpeedDampingRate();
		//m_move.z += (0 - m_move.z) * CManager::GetSpeedDampingRate();
		// 減速
		m_move.x += (0 - m_move.x) * 0.001f;
		m_move.z += (0 - m_move.z) * 0.001f;
	}

	////重力処理
	//if (m_bJump)
	//{// ジャンプしていたとき
	//	if (animType != ANIMATIONTYPE_JUMP_1)
	//	{
	//		if (pos.y > fHeight)
	//		{// 現在の高さが床より高かったとき
	//			m_move.y += -0.7f;
	//		}
	//		else
	//		{// 現在の高さが床より低かった時
	//			pos.y = fHeight;											// 床の高さを求める
	//			m_move.y = 0.0f;
	//			if (m_bJump)
	//			{
	//				// 砂煙のエフェクト表現
	//				CEffect::SandSmokeEffect(pos);
	//				m_bJump = false;										// ジャンプ判定を変える
	//				SetAnimPlayState(true);									// アニメーションの再開
	//				AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// アニメーションの変更
	//				pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// 着地音の再生
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	pos.y = fHeight;											// 床の高さを求める
	//}


	if (m_bColliderWithWall)
	{
		// 当たり判定管理処理
		Collision();
	}

	if (m_pColPlayerSphere != NULL)
	{// 武器の当たり判定が存在していたとき
		m_pColPlayerSphere->SetPosition(pos);
	}

	if (m_pColPlayerBox != NULL)
	{// 武器の当たり判定が存在していたとき
		m_pColPlayerBox->SetPosition(pos);
	}

	//	D3DXVECTOR3 move = CManager::Slip(playerPos + m_move, vNormal);// 滑りベクトルを計算

	//// 坂でのプレイヤー処理
	//SlopeMove();

	// キャラクターの更新処理
	CSceneX::Update();

	if (m_pRank != NULL)
	{
		m_pRank->SetNumber(pNetwork->GetRank(pNetwork->GetId()));
	}

	if (m_pDistanceNext != NULL)
	{
		m_pDistanceNext->Update();
	}

	// ドリフトしていたとき
	if (m_bDrift[DRIFT_LEFT] || m_bDrift[DRIFT_RIGHT])
	{
		// パーティクル全体の位置計算
		D3DXVECTOR3 vecPos = D3DXVECTOR3(-25.0f, 0.0f, -5.0f);
		D3DXMATRIX mtxMeshRot, mtxMeshTrans;				// 計算用マトリックス
		D3DXMATRIX mtx;										// 武器のマトリックス
		D3DXMATRIX mtxPlayer;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtx);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxPlayer);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		for (int nCount = 0; nCount < 2; nCount++)
		{
			CEffect::CreateEffect("spark", D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		// パーティクル全体の位置計算
		vecPos = D3DXVECTOR3(25.0f, 0.0f, -5.0f);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtx);

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtxPlayer);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		for (int nCount = 0; nCount < 2; nCount++)
		{
			CEffect::CreateEffect("spark", D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	//CDebugProc::Log("速度 : %.2f", CSpeed::GetSpeed());

	if (CSpeed::GetSpeed() > 10)
	{
		CEffect::SandSmoke(pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}


	if (pos.x >= 12000)
	{
		pos.x = 12000;
	}
	else if (pos.x <= -12000)
	{
		pos.x = -12000;
	}
	if (pos.z >= 12000)
	{
		pos.z = 12000;
	}
	else if (pos.z <= -12000)
	{
		pos.z = -12000;
	}
	// 位置設定
	SetPosition(pos);


#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CSceneX::DrawSnow();
}

//=============================================================================
// クリエイト処理
//=============================================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);
	pPlayer->Init();
	pPlayer->BindModel(m_pMeshModel, m_nNumMatModel, m_pBuffMatModel);
	return pPlayer;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// テクスチャの読み込み
	//D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTexture);
	// Xファイルの読み込み
	D3DXLoadMeshFromX("data/model/SnowBall.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

	return S_OK;
}

//=============================================================================
// 必殺技ポイント数の設定
//=============================================================================
void CPlayer::SetDeathblow(float nValue)
{
	m_fDeathblow = nValue;
}

//=============================================================================
// イベントフラグの設定
//=============================================================================
void CPlayer::SetEvent(bool bValue)
{
	m_bEvent = bValue;
}

//=============================================================================
// ゴールフラグの設定
//=============================================================================
void CPlayer::SetGoalState(bool bValue)
{
	m_bGoal = bValue;
}

//=============================================================================
// feverモード切替
//=============================================================================
void CPlayer::SetFeverMode(bool bValue)
{
	m_bFever = bValue;
}

//=============================================================================
// ポイント加算
//=============================================================================
void CPlayer::AddPoint(int nValue)
{
	m_nPoint += nValue;

	CCounter *pCounter = m_pUi->GetCounter("time");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_nPoint);
	}
}

//=============================================================================
// 当たり判定(trigger)
//=============================================================================
void CPlayer::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
	//CModel *pModel = GetModel();
	std::vector<CObject*> pointObj = CObject::GetPointObj();

	if(col->GetScene() == NULL) return;
	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{
		if (sTag == "weapon")
		{
			CSound *pSound = CManager::GetSound();				// サウンドの取得
		//	pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// ダメージ音の再生
			m_nLife -= 5;										// 体力を削る
			//AnimationSwitch(ANIMATIONTYPE_DAMAGE);				// アニメーションを変更

			D3DXVECTOR3 vec;

			vec = GetPosition() - col->GetPosition();			//差分を求める(方向を求めるため)
			D3DXVec3Normalize(&vec, &vec);						//正規化する

			m_move.x = vec.x * 25;
			m_move.z = vec.z * 25;

			if (m_nLife < 0)
			{
				CResult::SetIdxKill(CEnemy::GetEnemyKill());			// Kill数をリザルトに渡す
				CFade::SetFade(CManager::MODE_RESULT, CFade::FADETYPE_SLIDE);					// リザルトに遷移
			}
		}
	}
	//if (sTag == "wood")
	//{
	//	col->Release();
	//}
	//else if (sTag == "Building")
	//{
	//	col->Release();
	//}
	//else if (sTag == "Car")
	//{
	//	col->Release();
	//}
	//else if (sTag == "House")
	//{
	//	col->Release();
	//}
	if (sTag == "goal")
	{
		CNetwork *pNetwork = CManager::GetNetwork();
		pNetwork->SendTCP("GOAL", sizeof("GOAL"));
		m_bEvent = true;

		//if (CFade::GetFade() == CFade::FADE_NONE)
		//{//フェードが処理をしていないとき
		//	CFade::SetFade(CManager::MODE_RESULT, CFade::FADETYPE_NORMAL);					//フェードを入れる
		//}
	}
}

//=============================================================================
// 当たり判定(collider)
//=============================================================================
void CPlayer::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (sTag == "wood")
	{
		col->Release();
	}
	else if(sTag == "Building")
	{
		col->Release();
	}
	else if (sTag == "Car")
	{
		col->Release();
	}
	else if (sTag == "House")
	{
		col->Release();
	}
}

//========================================================================================
// アニメーションフレームの最大数に到達したときの処理
//========================================================================================
void CPlayer::BehaviorForMaxFrame(void)
{

}

//========================================================================================
// アニメーションキーの最大数に到達したときの処理
//========================================================================================
void CPlayer::BehaviorForMaxKey(void)
{
	//CModel *pModel = GetModel();
	//ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	//D3DXVECTOR3 rot = pModel[0].GetRotation();		// 回転量取得
}

//=============================================================================
// 近くにいる敵の近くまで移動する処理
//=============================================================================
void CPlayer::MoveNearEnemy(void)
{
	bool bTarget = false;				// 範囲内に敵が存在していたとき
	float fMinDistance = 750000.0f;		// 現在の最短距離
	float fDistance = 0.0f;				// 現在の距離
	D3DXVECTOR3 enemyPos;				// 最短距離にいる敵の位置
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_ENEMY);					// 先頭アドレスの取得

	// 次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_ENEMY);		//次回アップデート対象を控える
		fDistance = CManager::GetDistance(GetPosition(), pSceneNow->GetPosition());		// 距離を求める

		if (fDistance < fMinDistance)
		{
			if (!bTarget)
			{// ターゲットの存在が確認されていなかったとき
				bTarget = true;				// 位置の移動を許可する
			}

			enemyPos = pSceneNow->GetPosition();			// 敵の位置を記録する
		}

		pSceneNow = pSceneNext;								//次回アップデート対象を格納
	}

	if (bTarget)
	{// 移動の許可が出ていたとき
		SetPosition(enemyPos);			// 位置の移動
	}
}

//=============================================================================
// 当たり判定処理
//=============================================================================
void CPlayer::Collision(void)
{
	// 壁の当たり判定
	//m_bHit = CollisionWall();
}

//=============================================================================
// キー入力情報処理
//=============================================================================
void CPlayer::Input(D3DXVECTOR3 &pos)
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// ゲームパッドの取得
	CInputController *pGamepad = CManager::GetInputController();

	// カメラの取得
	CCamera *pCamera = CManager::GetCamera();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 Diff;	// 計算用格納変数
	float nValueH = 0;									//コントローラー
	float nValueV = 0;									//コントローラー

	D3DXVECTOR3 cameraVec = D3DXVECTOR3(0, 0, 0);				// カメラの方向ベクトル
	m_fSpeed = 0;

	// ====================== コントローラー ====================== //
	if (!m_bEvent)
	{// イベントが発生していなかったとき
		if (pGamepad != NULL)
		{// ゲームパッドが存在したとき
			if (pGamepad->GetJoypadUse(0))
			{// 使用可能だったとき
				pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

				float		fSpeed = 0.0f;			// プレイヤーの速度
				float		fAngle = 0.0f;			// スティック角度の計算用変数
				float fMomentX = sinf(m_dest.y);			//X角度
				float fMomentZ = cosf(m_dest.y);			//Z角度

				// 角度の計算して補正
				fAngle = atan2f(nValueH, nValueV);
				CTakaseiLibrary::RotRevision(&D3DXVECTOR3(0.0f, fAngle, 0.0f));

				// スティックの倒れ具合でスピードを決定
				if (abs(nValueH) > abs(nValueV))
					fSpeed = (abs(nValueH)) * PLAYER_SPEED;	// 横の倒れ具合
				else
					fSpeed = (abs(nValueV)) * PLAYER_SPEED;	// 縦の倒れ具合

				// スティックの角度によってプレイヤー移動
				m_move.x += sinf(fAngle + rot.y) * fSpeed;
				m_move.z += -cosf(fAngle + rot.y) * fSpeed;

				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

				// 目的の向きを決定
				if (nValueH != 0 || nValueV != 0)
				{
					m_dest.y = D3DX_PI + fAngle + rot.y;
				}

				// 回転の補正
				CTakaseiLibrary::RotRevision(&m_dest);

				// 目的の回転の設定
				SetRotation(m_dest);

#ifdef _DEBUG
				CDebugProc::Log("移動量 : %.2f %.2f %.2f", m_move.x, m_move.y, m_move.z);

				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					CDebugProc::Log("コントローラー : 0番\n");
				}
				if (pGamepad->GetControllerPress(1, JOYPADKEY_A))
				{
					CDebugProc::Log("コントローラー : 1番\n");
				}

				CDebugProc::Log("コントローラーH : %f\n", nValueH);
				CDebugProc::Log("コントローラーV : %f\n", nValueV);
#endif
			}
		}

		// ====================== キーボード ====================== //

		if (pKeyboard->GetTriggerKeyboard(DIK_1))
		{
			for (int nCount = 0; nCount < 20; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
				particlePos.y = sinf(D3DX_PI + fAngle_x);
				particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

				fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
				D3DXVECTOR3 rot;
				rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				CEffect::SetEffect("data/tex/effect/rose_01.png",			// パーティクルのタイプ
					GetPosition(),											// 発生位置
					D3DXVECTOR3(8.0f, 8.0f, 0.0f),							// サイズ
					particlePos * 5.0f,										// 方向ベクトル
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// 回転の変化量
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// 色の変化量
					EASINGTYPE_NONE,
					rot,													// テクスチャの回転
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// カラー
					200,													// パーティクルの生存カウント数
					-0.98f,													// 重力
					0,														// 抵抗
					true,													// ビルボード
					0,														// 表示する箇所(横)
					0,														// 表示する箇所(縦)
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0,
					0,
					0,
					D3DXVECTOR2(1.0f, 1.0f),								// 画像の分割数
					false,													// 加算合成の有無
					false,													// Zバッファの比較有無
					false													// フェード
				);
			}

		}

		if (pKeyboard->GetTriggerKeyboard(DIK_2))
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(D3DX_PI / 2, m_rot.y, m_rot.z));
		}
	}

	if (!m_bEvent)
	{// イベントが発生していなかったとき

		float fMomentX = sinf(m_dest.y);			//X角度
		float fMomentZ = cosf(m_dest.y);			//Z角度

		if (pKeyboard->GetPressKeyboard(DIK_A))
		{// 方向キー左ボタンを押されたとき
			if (pKeyboard->GetPressKeyboard(DIK_W))
			{// 左上
				m_move.x += sinf(D3DX_PI * -0.25f)*1.0f;
				m_move.z += cosf(D3DX_PI * -0.25f)*1.0f;
				//プレイヤー旋回
				m_dest.y = (-D3DX_PI * -0.75f);
				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else if (pKeyboard->GetPressKeyboard(DIK_S))
			{// 左下
				m_move.x += sinf(D3DX_PI * -0.75f)*1.0f;
				m_move.z += cosf(D3DX_PI * -0.75f)*1.0f;
				//プレイヤー旋回
				m_dest.y = (-D3DX_PI * -0.25f);
				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else
			{// 左
				m_move.x += sinf(D3DX_PI * -0.5f)*1.0f;
				m_move.z += cosf(D3DX_PI * -0.5f)*1.0f;
				//プレイヤー旋回
				m_dest.y = (-D3DX_PI * -0.5f);
				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
		}
		else if (pKeyboard->GetPressKeyboard(DIK_D))
		{// 方向キー右ボタンを押されたとき
			if (pKeyboard->GetPressKeyboard(DIK_W))
			{// 右上
				m_move.x += sinf(D3DX_PI * 0.25f)*1.0f;
				m_move.z += cosf(D3DX_PI * 0.25f)*1.0f;
				//プレイヤー旋回
				m_dest.y = (-D3DX_PI * 0.75f);
				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else if (pKeyboard->GetPressKeyboard(DIK_S))
			{// 右下
				m_move.x += sinf(D3DX_PI * 0.75f)*1.0f;
				m_move.z += cosf(D3DX_PI * 0.75f)*1.0f;
				//プレイヤー旋回
				m_dest.y = (-D3DX_PI * 0.25f);
				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else
			{// 右
				m_move.x += sinf(D3DX_PI * 0.5f)*1.0f;
				m_move.z += cosf(D3DX_PI * 0.5f)*1.0f;
				//プレイヤー旋回
				m_dest.y = (D3DX_PI * 0.5f);
				//クォータニオン回転処理
				m_vecAxis.x = fMomentX * cosf(-D3DX_PI / 2.0f) - fMomentZ * sinf(-D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(-D3DX_PI / 2.0f) + fMomentZ * cosf(-D3DX_PI / 2.0f);
			}
		}
		else if (pKeyboard->GetPressKeyboard(DIK_W))
		{// 上
			m_move.x += sinf(D3DX_PI * 0.0f)*1.0f;
			m_move.z += cosf(D3DX_PI * 0.0f)*1.0f;
			//プレイヤー旋回
			m_dest.y = (D3DX_PI * 1.0f);
			//クォータニオン回転処理
			m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
			m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

		}
		else if (pKeyboard->GetPressKeyboard(DIK_S))
		{// 下
		 // 移動量(回転なし)
			m_move.x += sinf(D3DX_PI * 1.0f)*1.0f;
			m_move.z += cosf(D3DX_PI * 1.0f)*1.0f;

			//プレイヤー旋回
			m_dest.y = (D3DX_PI * 0.0f);

			//クォータニオン回転処理
			m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
			m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

		}

		//クォータニオン(回転量)
		m_fValueRot = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z) * 0.01f;

		// -3.14より小さかったら
		if (m_rot.y < -D3DX_PI)
		{
			// 修正
			m_rot.y += D3DX_PI * 2;
		}
		// 3.14より大きかったら
		else if (m_rot.y > D3DX_PI)
		{
			// 修正
			m_rot.y -= D3DX_PI * 2;
		}

		pos += m_move;

		m_move.x += (0 - m_move.x) * 0.1f;
		m_move.z += (0 - m_move.z) * 0.1f;


		SetRotation(m_rot);
		SetvecAxis(D3DXVECTOR3(m_vecAxis.x, m_vecAxis.y, m_vecAxis.z));
		SetfValueRot(m_fValueRot);
		CSceneX::Update();
	}

	//{// カメラ設定
	//	// タイヤの回転の半分を差と格納
	//	cameraVec.y = m_cameraRot.y - (m_rot.y + (m_dest.y * 0.85f));

	//	// 回転の補正
	//	CTakaseiLibrary::RotRevision(&cameraVec);

	//	// カメラを徐々に回転させていく
	//	m_cameraRot.y -= cameraVec.y * CAMERA_ROT_SPEED;

	//	// 回転の補正
	//	CTakaseiLibrary::RotRevision(&m_cameraRot);

	//	// カメラ回転設定
	//	SetCameraRot(m_cameraRot);
	//}

	// 移動していいとき
	if (m_bMove)
	{
		// 回転の補正
		CTakaseiLibrary::RotRevision(&m_dest);

		// プレイヤーを徐々に回転させていく
		m_rot.y += m_dest.y * ROT_AMOUNT;
	}

	// アクセルボタンとブレーキボタンを離したとき
	if (!pGamepad->GetControllerPress(0, JOYPADKEY_A) && !pGamepad->GetControllerPress(0, JOYPADKEY_B) &&
		!pKeyboard->GetPressKeyboard(MOVE_ACCEL) && !pKeyboard->GetPressKeyboard(MOVE_BRAKE))
	{
		// 移動不可
		m_bMove = false;
	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// Kが押されたとき
		m_fDeathblow = 50.0f;				// 必殺技ポイントを最大値まで上げる
	}
	{// Kが押されたとき
		m_fDeathblow = 50.0f;				// 必殺技ポイントを最大値まで上げる
	}

	if (pKeyboard->GetTriggerKeyboard(DIK_5))
	{
		CEffect::SandSmoke(pos);
		//CEffect::PetalCluster(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//CEffect::Star(pos);
	}

#endif
}

//=============================================================================
// キーボード入力処理
//=============================================================================
void CPlayer::InputKeyboard(float fTireRotSpeed, D3DXVECTOR3 aVec)
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	//上下操作
	if (pKeyboard->GetPressKeyboard(MOVE_ACCEL))
	{
		// 前輪モデルの最終目的座標
		m_dest.y = 0.0f;

		// 速度設定
		m_fSpeed = -m_fPuzzleMaxSPeed;

		// タイヤ回転方向設定
		fTireRotSpeed = TIRE_ROT_SPEED;

		// 動いていい
		m_bMove = true;

		// アクセルボタンを押した
		m_bAccel = true;
	}
	else if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
	{
		// 前輪モデルの最終目的座標
		m_dest.y = 0.0f;

		// 速度設定
		m_fSpeed = m_fPuzzleMaxSPeed;

		// タイヤ回転方向設定
		fTireRotSpeed = -TIRE_ROT_SPEED;

		// 動いていい
		m_bMove = true;
	}

	// アクセルボタンを離したとき
	if (!pKeyboard->GetPressKeyboard(MOVE_ACCEL))
	{
		// アクセルボタンを離した
		m_bAccel = false;
	}

	// ドリフトボタンを押していないとき
	if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
	{
		//左右操作
		if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
		{
			// 前輪モデルの最終目的座標
			m_dest.y = -CManager::GetTurnVelocity();
		}
		else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
		{
			// 前輪モデルの最終目的座標
			m_dest.y = CManager::GetTurnVelocity();
		}

		// ブレーキボタンが押されたとき
		if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
		{
			//左右操作
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// 前輪モデルの最終目的座標
				m_dest.y = CManager::GetTurnVelocity();
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// 前輪モデルの最終目的座標
				m_dest.y = -CManager::GetTurnVelocity();
			}
		}
	}

	// アクセル状態のとき
	if (m_bAccel)
	{
		// ドリフトしていないとき
		if (!m_bDrift[DRIFT_RIGHT] && !m_bDrift[DRIFT_LEFT])
		{
			// ドリフトボタンを押したとき
			if (pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				//左右操作
				if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
				{
					// ドリフトしている状態にする
					m_bDrift[DRIFT_LEFT] = true;
				}
				else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
				{
					// ドリフトしている状態にする
					m_bDrift[DRIFT_RIGHT] = true;
				}
			}
		}

		// 右ドリフトしているとき
		if (m_bDrift[DRIFT_RIGHT])
		{
			// 前輪モデルの最終目的地座標
			m_dest.y = DRIFT_DEST;

			//左右操作
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// 前輪モデルの最終目的地座標
				m_dest.y = 0.0f;

				// 加速度
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// 前輪モデルの最終目的地座標
				m_dest.y = ROT_SPEED_DRIFT;

				// 加速度
				m_fAcceleration += ACCEKERATION_ADDITION;
			}

			// 加速度ベクトル設定
			aVec.x = sinf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;

			// ドリフトボタンを離したとき
			if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				// ドリフト最大までカウント
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// ドリフトしていない状態にする
					m_bDrift[nCnt] = false;

					// 加速度初期化
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
		else if (m_bDrift[DRIFT_LEFT])
		{// 左ドリフトのとき
			// 前輪モデルの最終目的地座標
			m_dest.y = -DRIFT_DEST;

			//左右操作
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// 前輪モデルの最終目的地座標
				m_dest.y = -ROT_SPEED_DRIFT;

				// 加速度
				m_fAcceleration += ACCEKERATION_ADDITION;
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// 前輪モデルの最終目的地座標
				m_dest.y = 0.0f;

				// 加速度
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}

			// 加速度ベクトル設定
			aVec.x = sinf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;

			// ドリフトボタンを離したとき
			if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				// ドリフト最大までカウント
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// ドリフトしていない状態にする
					m_bDrift[nCnt] = false;

					// 加速度初期化
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
	}
}

//=============================================================================
// ゲームパッド入力処理
//=============================================================================
void CPlayer::InputGemepad(float nValueH, float nValueV, float fTireRotSpeed, D3DXVECTOR3 aVec)
{
	// ゲームパッドの取得
	CInputController *pGamepad = CManager::GetInputController();

	//上下操作
	if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
	{
		// 前輪モデルの最終目的座標
		m_dest.y = 0.0f;

		// 速度設定
		m_fSpeed = -m_fPuzzleMaxSPeed;

		// タイヤ回転方向設定
		fTireRotSpeed = TIRE_ROT_SPEED;

		// 動いていい
		m_bMove = true;

		// アクセルボタンを押した
		m_bAccel = true;
	}
	else if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
	{
		// 前輪モデルの最終目的座標
		m_dest.y = 0.0f;

		// 速度設定
		m_fSpeed = m_fPuzzleMaxSPeed;

		// タイヤ回転方向設定
		fTireRotSpeed = -TIRE_ROT_SPEED;

		// 動いていい
		m_bMove = true;
	}

	// アクセルボタンを離したとき
	if (!pGamepad->GetControllerPress(0, JOYPADKEY_A))
	{
		// アクセルボタンを離した
		m_bAccel = false;
	}

	// ドリフトボタンを押していないとき
	if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
	{
		// 左にスティックが倒れたとき
		if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
		{
			// 前輪モデルの最終目的座標
			m_dest.y = -CManager::GetTurnVelocity();
		}
		else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
		{// 右にスティックが倒れたとき
			// 前輪モデルの最終目的座標
			m_dest.y = CManager::GetTurnVelocity();
		}

		// ブレーキボタンが押されたとき
		if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
		{
			// 左にスティックが倒れたとき
			if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
			{
				// 前輪モデルの最終目的座標
				m_dest.y = CManager::GetTurnVelocity();
			}
			else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
			{// 右にスティックが倒れたとき
			 // 前輪モデルの最終目的座標
				m_dest.y = -CManager::GetTurnVelocity();
			}
		}
	}

	// アクセル状態のとき
	if (m_bAccel)
	{
		// ドリフトしていないとき
		if (!m_bDrift[DRIFT_RIGHT] && !m_bDrift[DRIFT_LEFT])
		{
			// ドリフトボタンを押したとき
			if (pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
			{
				// 左にスティックが倒れたとき
				if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
				{
					// ドリフトしている状態にする
					m_bDrift[DRIFT_LEFT] = true;
				}
				else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
				{// 右にスティックが倒れたとき
					// ドリフトしている状態にする
					m_bDrift[DRIFT_RIGHT] = true;
				}
			}
		}

		// 右ドリフトしているとき
		if (m_bDrift[DRIFT_RIGHT])
		{
			// 前輪モデルの最終目的地座標
			m_dest.y = DRIFT_DEST;

			// 左にスティックが倒れたとき
			if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
			{
				// 前輪モデルの最終目的地座標
				m_dest.y = 0.0f;

				// 加速度
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}
			else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
			{// 右にスティックが倒れたとき
				// 前輪モデルの最終目的地座標
				m_dest.y = ROT_SPEED_DRIFT;

				// 加速度
				m_fAcceleration += ACCEKERATION_ADDITION;
			}

			// 加速度ベクトル設定
			aVec.x = sinf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;

			// ドリフトボタンを離したとき
			if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
			{
				// ドリフト最大までカウント
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// ドリフトしていない状態にする
					m_bDrift[nCnt] = false;

					// 加速度初期化
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
		else if (m_bDrift[DRIFT_LEFT])
		{// 左ドリフトのとき
			// 前輪モデルの最終目的地座標
			m_dest.y = -DRIFT_DEST;

			// 左にスティックが倒れたとき
			if (nValueV <= JOY_MAX_RANGE && nValueV > 0)
			{
				// 前輪モデルの最終目的地座標
				m_dest.y = -ROT_SPEED_DRIFT;

				// 加速度
				m_fAcceleration += ACCEKERATION_ADDITION;
			}
			else if (nValueV >= -JOY_MAX_RANGE && nValueV < 0)
			{// 右にスティックが倒れたとき
				// 前輪モデルの最終目的地座標
				m_dest.y = 0.0f;

				// 加速度
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}

			// 加速度ベクトル設定
			aVec.x = sinf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;

			// ドリフトボタンを離したとき
			if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
			{
				// ドリフト最大までカウント
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// ドリフトしていない状態にする
					m_bDrift[nCnt] = false;

					// 加速度初期化
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void CPlayer::Debug(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();

	ImGui::Begin("System");													// 「System」ウィンドウに追加します。なければ作成します。

	if (ImGui::CollapsingHeader("player"))
	{
		ImGui::Text("pos = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);								// プレイヤーの現在位置を表示
		ImGui::Text("posOld = %.2f, %.2f, %.2f", posOld.x, posOld.y, posOld.z);								// プレイヤーの現在位置を表示
		ImGui::Text("rot = %.2f, %.2f, %.2f", m_rot.x, m_rot.y, m_rot.z);								// プレイヤーの回転を表示
		ImGui::Text("move = %.2f, %.2f, %.2f", m_move.x, m_move.y, m_move.z);								// プレイヤーの現在位置を表示
		ImGui::Text("HP = %d", m_nLife);				// プレイヤーの体力を表示

		D3DXVECTOR3 size = GetSize();
		ImGui::DragFloat3(u8"大きさ", (float*)size);
		SetSize(size);

		ImGui::Checkbox("ColliderWithWall", &m_bColliderWithWall);

		if (ImGui::Button("BOSS"))
		{
			pos = D3DXVECTOR3(5972.14f, 100.0f, 8000.62f);
			SetPosition(pos);
		}
	}

	//デバッグ処理を終了
	ImGui::End();
}
#endif
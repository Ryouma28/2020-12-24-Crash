//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "player.h"
#include "scene3D.h"
#include "meshSphere.h"
#include "meshCube.h"
#include "meshCapsule.h"
#include "enemy.h"
#include "sky.h"
#include "tree.h"
#include "object.h"
#include "circle.h"
#include "stage.h"
#include "effect.h"
#include "tree.h"
#include "number.h"
#include "time.h"
#include "result.h"
#include "ui.h"
#include "network.h"
#include "wall.h"
#include "speed.h"
#include "GuideSign.h"
#include "network.h"
#include "startSignal.h"
#include "shadow.h"
#include "finishUi.h"
#include "counter.h"
#include "camera.h"
#include "ranking.h"
#include "inputKeyboard.h"
#include "time.h"
#include "endSignal.h"
#include "fever.h"

//=============================================================================
// 静的メンバ変数
//=============================================================================
CMeshField	*CGame::m_pMeshField = NULL;		// メッシュフィールドのポインタ
CPlayer		*CGame::m_pPlayer = NULL;			// プレイヤーのポインタ
CMeshSphere *CGame::m_pMeshSphere = NULL;		// メッシュスフィアのポインタ
CEnemy		*CGame::m_pEnemy = NULL;			// 敵のポインタ
CSky		*CGame::m_pSky = NULL;				// 空のポインタ
CHouse		*CGame::m_pHouse = NULL;			// 家のポインタ
CSpeed		*CGame::m_pSpeed = NULL;			// 時速のポインタ
CEndSignal	*CGame::m_pEndSignal = NULL;

CUi			*CGame::m_pUi = NULL;				// UIのポインタ
CTime		*CGame::m_pTime = NULL;				// カウンタのポインタ
CUi			*CGame::m_pTimeUi = NULL;				// UIのポインタ

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//Init処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// エフェクトの生成
	CEffect::Create();
	m_bRate = false;

	m_pTime = CTime::Create();
	m_pTime->SetPosition(D3DXVECTOR3(500.0f, 100.0f, 0.0f));
	m_pTime->SetSeconds(60);
	m_pTime->SetUpdateTimer(true);

	m_pTimeUi = CUi::Create();

	if (m_pTimeUi != NULL)
	{
		m_pTimeUi->LoadScript("data/text/ui/time.txt");
		CCounter *pCounter = m_pTimeUi->GetCounter("min");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(m_pTime->GetMinutes());
		}

		pCounter = m_pTimeUi->GetCounter("sec");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(m_pTime->GetSeconds());
		}
	}

	// プレイヤーの生成
	m_pPlayer = NULL;
	m_pPlayer = CPlayer::Create();

	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{
		pCamera->SetDistance(2500);
		pCamera->SetRotation(D3DXVECTOR3(0.46f, 3.09f, 0.0f));
	}

	// 空の作成
	m_pSky = CSky::Create();

	CObject::LoadScript();

	// 各種アセットの生成＆設置
	CMeshField::LoadRand("data/text/stage/rand.txt", false);			// 床情報の読込
	//CObject::LoadModel("data/stage/object.csv");					// モデル情報の読込
	//CEnemy::LoadEnemy("data/stage/enemy.csv");					// 敵情報の読込

	// 時間のクリエイト処理
	CTime::Create();

	//// モデル情報の読み込み
	//CObject::LoadModelTest("data/text/stage/stage_1.txt");

	// モデル情報の読み込み
	CObject::LoadModelTest("data/text/stage/model.txt");

	//// ネットワークでのゲーム時初期化処理
	//CManager::GetNetwork()->InitGame();

	CStartSignal::Create();

	return S_OK;
}

//=============================================================================
// Update関数
//=============================================================================
void CGame::Update(void)
{
	CNetwork *pNetwork = CManager::GetNetwork();

	if (pNetwork != NULL)
	{
		pNetwork->Create();
	}

	bool bTime = m_pTime->GetUpdateTimer();

	if (m_pTimeUi != NULL)
	{
		CCounter *pCounter;

		pCounter = m_pTimeUi->GetCounter("sec");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(m_pTime->GetSeconds());
		}

	}

	if (bTime == false)
	{
		if (m_pEndSignal == NULL)
		{
			m_pEndSignal = CEndSignal::Create();
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();
	if (pKeyboard->GetTriggerKeyboard(DIK_F))
	{
		CFever::Create();
	}

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTriggerKeyboard(DIK_0))
	{
		CStartSignal::Create();
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//フェードが処理をしていないとき
		if (pKeyboard != NULL)
		{// キーボードが存在していたとき
			if (pKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// 指定のキーが押されたとき
				m_pEndSignal = CEndSignal::Create();
			}
		}
	}

#endif
}

//=============================================================================
// Draw関数
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// Uninit関数
//=============================================================================
void CGame::Uninit(void)
{
	CNetwork *pNetwork = CManager::GetNetwork();

	if (pNetwork != NULL)
	{// ネットワークが存在していたとき
		pNetwork->StopUpdate();				// 更新停止予約
		pNetwork->CloseTCP();				// サーバーとの窓口を閉める
	}

	CObject::Unload();

	// ポリゴンの開放
	CScene::ReleaseAll();
}

//=============================================================================
// アセットの読み込み
//=============================================================================
void CGame::LoadAsset(void)
{
	// =============== アセットの読み込み ===============//
	CPlayer::Load();
	CSky::Load();
	CMeshField::Load();
	CEnemy::Load();
	CObject::Load();
	CMeshSphere::Load();
	CEffect::Load();
	CNumber::Load();
	CGuideSign::Load();
	CShadow::Load();
}
//=============================================================================
//
// オブジェクト処理 [object.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "meshField.h"
#include "camera.h"
#include "house.h"
#include "colliderBox.h"
#include "colliderSphere.h"
#include "game.h"
#include "player.h"
#include "distanceNext.h"
#include "finishUi.h"
#include "effect.h"
#include "sound.h"
#include "fade.h"
#include "fever.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define OBJECT_SCRIPT "data/text/manager/manager_model.txt"
#define POINT_WOOD (10)
#define POINT_BUILDING (100)
#define POINT_CAR (50)
#define POINT_HOUSE (500)
#define	POINT_SNOWMAN (1000)

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
std::vector<CObject*> CObject::m_vPointObj = {};
unsigned int CObject::m_pointNum = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(CScene::PRIORITY obj = CScene::PRIORITY_MODEL) : CSceneX(obj)
{
	SetObjType(CScene::PRIORITY_MODEL);

	m_pBox = NULL;
	m_pSphere = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得

	// 初期化処理
	CSceneX::Init();

	D3DXVECTOR3 pos = D3DXVECTOR3(500.0f, 0.0f, 0.0f);

	// ポイントオブジェクトのとき
	if (m_Add == "data/model/point.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 3000.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("checkpoint");
				m_pSphere->SetPosition(pos);
			}
		}
	}

	// ポイントオブジェクトのとき
	if (m_Add == "data/model/wood3.x" || m_Add == "data/model/wood2.x" || m_Add == "data/model/wood1.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("wood");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			}
		}
	}
	// ポイントオブジェクトのとき
	if (m_Add == "data/model/object/Building.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("Building");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(-25.0f, 250.0f, 0.0f));
			}
		}
	}
	// ポイントオブジェクトのとき
	if (m_Add == "data/model/Snowman.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("Snowman");
				pos;
				pos.x = CManager::GetRand(2500) - 1250.0f;
				pos.z = CManager::GetRand(2500) - 1250.0f;

				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			}
		}
	}
	// ポイントオブジェクトのとき
	if (m_Add == "data/model/object/Car_Rad.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("Car");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			}
		}
	}

	// ポイントオブジェクトのとき
	if (m_Add == "data/model/object/Car_Blue.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("Car");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			}
		}
	}

	// ポイントオブジェクトのとき
	if (m_Add == "data/model/object/Car_Green.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("Car");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			}
		}
	}

	// ポイントオブジェクトのとき
	if (m_Add == "data/model/object/House00.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("House");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 50.0f, -100.0f));
			}
		}
	}

	// ポイントオブジェクトのとき
	if (m_Add == "data/model/object/House01.x")
	{
		// スフィアがNULLのとき
		if (m_pSphere == NULL)
		{
			// スフィアの生成処理
			m_pSphere = CColliderSphere::Create(true, 100.0f);

			// スフィアがあるとき
			if (m_pSphere != NULL)
			{
				m_pSphere->SetScene(this);
				m_pSphere->SetTag("House");
				m_pSphere->SetPosition(pos);
				m_pSphere->SetOffset(D3DXVECTOR3(0.0f, 50.0f, 0.0f));
			}
		}
	}

	SetPosition(pos);			// 位置の設定
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CObject::Uninit(void)
{
	// 開放処理
	CSceneX::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void CObject::Update(void)
{
	D3DXVECTOR3 pos = GetPosition();				// 位置の取得

	if (m_Add == "data/model/GoalCircle35.x")
	{
		if (m_pBox == NULL)
		{
			m_pBox = CColliderBox::Create(true, D3DXVECTOR3(400.0f, 800.0f, 1400.0f));

			if (m_pBox != NULL)
			{
				m_pBox->SetScene(this);
				m_pBox->SetTag("goal");
				m_pBox->SetPosition(pos);
				m_pBox->SetOffset(D3DXVECTOR3(0.0f, 500.0f, 0.0f));
			}
		}
	}

	if (m_pBox != NULL)
	{
		m_pBox->SetPosition(pos);
	}

	SetPosition(pos);		// 位置の設定

#ifdef _DEBUG
	Debug();				// デバッグ処理
#endif
}

//=============================================================================
// 描画処理
//=============================================================================
void CObject::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// クリエイト関数
//=============================================================================
CObject *CObject::Create(void)
{
	CObject *pObject = NULL;
	pObject = new CObject(CScene::PRIORITY_MODEL);

	if (pObject != NULL)
	{
		pObject->Init();
	}
	return pObject;
}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CObject::Load(void)
{
	return S_OK;
}

//=============================================================================
// テクスチャの解放関数
//=============================================================================
void CObject::Unload(void)
{
}

//=============================================================================
// オブジェクトアセットのロード処理
//=============================================================================
void CObject::LoadScript(void)
{
	FILE *pFile;
	char cReadText[128];		//文字
	char cHeadText[128];		//比較
	char cDie[128];
	int nCntPointer = 0;		//ポインターの数値

	char sAdd[64];				//モデルのアドレス
	std::string Add;

	int nCntMotion = 0;			//参照するポインタの値を初期化
	int nCntKey = 0;

	int nMaxModel = 0;

	//テキストデータロード
	pFile = fopen(OBJECT_SCRIPT, "r");

	if (pFile != NULL)
	{
		//ポインターのリセット
		nCntPointer = 0;

		//スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//エンドスクリプトが来るまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//改行
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{//パーツモデルのアドレス情報のとき
						sscanf(cReadText, "%s %s %s", &cDie, &cDie, &sAdd[0]);						//アドレスの取得
						Add = sAdd;
						CManager::LoadModel(Add);
					}
				}
			}
		}

		//ファイル閉
		fclose(pFile);
	}
	else
	{

	}
}

//=============================================================================
// モデルの読込
//=============================================================================
void CObject::LoadModel(char *add)
{
	FILE *pFile = NULL;											// ファイル
	char cReadText[128] = {};									// 文字
	char cHeadText[128] = {};									// 比較
	CObject *pObject = NULL;

	pFile = fopen(add, "r");									// ファイルを開くまたは作る

	if (pFile != NULL)											// ファイルが読み込めた場合
	{
		fgets(cReadText, sizeof(cReadText), pFile);				// 行を飛ばす
		fgets(cReadText, sizeof(cReadText), pFile);				// 行を飛ばす
		fgets(cReadText, sizeof(cReadText), pFile);				// 行を飛ばす

		while (strcmp(cHeadText, "End") != 0)
		{
			std::string Data = cReadText;
			std::vector<std::string> splitData = CManager::split(Data, ',');

			pObject = CObject::Create();
			pObject->BindModel(splitData[LOADTYPE_ADD]);
			pObject->SetPosition(D3DXVECTOR3((float)atof(splitData[LOADTYPE_POS_X].c_str()), (float)atof(splitData[LOADTYPE_POS_Y].c_str()), (float)atof(splitData[LOADTYPE_POS_Z].c_str())));			// 位置の設定
			pObject->SetRotation(D3DXVECTOR3((float)atof(splitData[LOADTYPE_ROT_X].c_str()), (float)atof(splitData[LOADTYPE_ROT_Y].c_str()), (float)atof(splitData[LOADTYPE_ROT_Z].c_str())));			// 回転値を設定

			fgets(cReadText, sizeof(cReadText), pFile);									// 行を飛ばす
			sscanf(cReadText, "%s", &cHeadText);
		}

		fclose(pFile);					// ファイルを閉じる
	}
	else
	{
		MessageBox(NULL, "モデル情報のアクセス失敗！", "WARNING", MB_ICONWARNING);	// メッセージボックスの生成
	}
}

//=============================================================================
// モデル読み込み(テスト)
//=============================================================================
void CObject::LoadModelTest(char *add)
{
	FILE *pFile = NULL;																	// ファイル
	char cReadText[128];															// 文字
	char cHeadText[128];															// 比較
	char cDie[128];

	pFile = fopen(add, "r");				// ファイルを開くまたは作る

	if (pFile != NULL)						//ファイルが読み込めた場合
	{
		//スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		//スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			//エンドスクリプトが来るまで
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cReadText, sizeof(cReadText), pFile);
				sscanf(cReadText, "%s", &cHeadText);

				//改行
				if (strcmp(cReadText, "\n") != 0)
				{
					if (strcmp(cHeadText, "MODELSET") == 0)
					{//キャラクターの初期情報のとき
						CObject *pObj = NULL;
						pObj = CObject::Create();										// 床の作成

						if (pObj != NULL)
						{
							//エンドキャラクターセットが来るまでループ
							while (strcmp(cHeadText, "END_MODELSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
								{//パーツ総数のとき
									char aModelAdd[64];
									memset(&aModelAdd, 0, sizeof(aModelAdd));

									sscanf(cReadText, "%s %s %s", &cDie, &cDie,
										&aModelAdd);

									// 位置の設定
									pObj->BindModel(aModelAdd);
									pObj->Init();

									// ポイントモデルのとき
									if (strcmp(aModelAdd, "data/model/point.x") == 0)
									{
										m_vPointObj.push_back(pObj);
									}
								}
								else if (strcmp(cHeadText, "POS") == 0)
								{//パーツ総数のとき
									D3DXVECTOR3 pos;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&pos.x,
										&pos.y,
										&pos.z);

									// 位置の設定
									pObj->SetPosition(pos);
								}
								else if (strcmp(cHeadText, "ROT") == 0)
								{//パーツ総数のとき
									D3DXVECTOR3 rot;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&rot.x,
										&rot.y,
										&rot.z);

									// 位置の設定
									pObj->SetRotation(rot);
								}
								else if (strcmp(cHeadText, "SIZE") == 0)
								{//パーツ総数のとき
									D3DXVECTOR3 size;
									sscanf(cReadText, "%s %s %f %f %f", &cDie, &cDie,
										&size.x,
										&size.y,
										&size.z);

									// 位置の設定
									pObj->SetSize(size);
								}
							}
						}
					}
				}
			}
		}
		fclose(pFile);				// ファイルを閉じる

		CObject *pObj = CObject::Create();										// 床の作成
		if (pObj != NULL)
		{
			pObj->BindModel("data/model/Snowman.x");
			pObj->Init();
		}

	}
	else
	{
		MessageBox(NULL, "地面情報のアクセス失敗！", "WARNING", MB_ICONWARNING);	// メッセージボックスの生成
	}
}

//=============================================================================
// モデル情報の読込
//=============================================================================
void CObject::BindModel(std::string add)
{
	m_Add = add;

	// モデルの取得
	CManager::GetModelResource(add, m_pBuffMat, m_nNumMat, m_pMesh);

	// モデルの情報をセット
	CSceneX::BindModel(m_pMesh, m_nNumMat, m_pBuffMat);
}

//=============================================================================
// オブジェクトの当たり判定
//=============================================================================
void CObject::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	CSound *pSound = CManager::GetSound();				// サウンドの取得
	if (pSound != NULL)
	{
		pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// ダメージ音の再生
	}

	if (sTag == "player")
	{
		CPlayer *pPlayer = CGame::GetPlayer();

		// ポイントオブジェクトのとき
		if (m_Add == "data/model/wood3.x" || m_Add == "data/model/wood2.x" || m_Add == "data/model/wood1.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_WOOD * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_WOOD);
			}
		}
		// ポイントオブジェクトのとき
		else if (m_Add == "data/model/object/Building.x")
		{
			CEffect::SandSmokeEffect(GetPosition());
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_BUILDING * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_BUILDING);
			}
		}
		// ポイントオブジェクトのとき
		else if (m_Add == "data/model/object/Car_Rad.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_CAR * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_CAR);
			}
		}

		// ポイントオブジェクトのとき
		else if (m_Add == "data/model/object/Car_Blue.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_CAR * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_CAR);
			}
		}

		// ポイントオブジェクトのとき
		else if (m_Add == "data/model/object/Car_Green.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_CAR * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_CAR);
			}
		}

		// ポイントオブジェクトのとき
		else if (m_Add == "data/model/object/House00.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_HOUSE * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_HOUSE);
			}
		}

		// ポイントオブジェクトのとき
		else if (m_Add == "data/model/object/House01.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pPlayer->GetFeverMode())
			{
				pPlayer->AddPoint(POINT_HOUSE * 2);
			}
			else
			{
				pPlayer->AddPoint(POINT_HOUSE);
			}
		}
		else if (m_Add == "data/model/Snowman.x")
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			pPlayer->AddPoint(POINT_SNOWMAN);

			D3DXVECTOR3 pos;
			pos.x = CManager::GetRand(5000) - 2500.0f;
			pos.z = CManager::GetRand(5000) - 2500.0f;

			SetPosition(pos);

			if (!pPlayer->GetFeverMode())
			{
				CFever::Create();
			}
		}

		SetActive(false);

		if (m_pSphere != NULL)
		{
			m_pSphere->SetActive(false);
		}
		if (m_pBox != NULL)
		{
			m_pSphere->SetActive(false);
		}
		//Release();
	}
}

//=============================================================================
// インスペクターを表示
//=============================================================================
void CObject::ShowInspector(void)
{
	CSceneX::ShowInspector();
}

//=============================================================================
// 当たり判定(レイ)
//=============================================================================
bool CObject::Collide(D3DXVECTOR3 vStart, D3DXVECTOR3 vDir, FLOAT* pfDistance, D3DXVECTOR3* pvNormal, D3DXMATRIX mtx)
{
	BOOL boHit = false;
	D3DXMATRIX mWorld;
	D3DXVec3Normalize(&vDir, &vDir);

	CScene *pSceneNext = NULL;														//次回アップデート対象
	CScene *pSceneNow = NULL;

	D3DXMATRIX			invmat;							// 逆行列を格納する変数
	D3DXVECTOR3			m_posAfter;						// 逆行列で出した終点情報を格納する
	D3DXVECTOR3			m_posBefore;					// 終点情報を格納する
	D3DXVECTOR3			direction;						// 変換後の位置、方向を格納する変数

	CDebugProc::Log("始点 : %.2f %.2f %.2f\n", vStart.x, vStart.y, vStart.z);
	CDebugProc::Log("終点 : %.2f %.2f %.2f\n", vDir.x, vDir.y, vDir.z);

	pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);

	//次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, (CScene::PRIORITY_MODEL));							//次回アップデート対象を控える

		CObject *pObj = (CObject*)pSceneNow;								// クラスチェンジ(床)

		//// レイを当てるメッシュが動いていたり回転している場合でも対象のワールド行列の逆行列を用いれば正しくレイが当たる
		//D3DXMatrixInverse(&mWorld, NULL, &pObj->GetMtxWorld());
		//D3DXVec3TransformCoord(&vStart, &vStart, &mWorld);

		//	逆行列の取得
		D3DXMatrixInverse(&invmat, NULL, &pObj->GetMtxWorld());

		//	逆行列を使用し、レイ始点情報を変換　位置と向きで変換する関数が異なるので要注意
		D3DXVec3TransformCoord(&m_posBefore, &D3DXVECTOR3(vStart.x, mtx._42, vStart.z), &invmat);

		//	レイ終点情報を変換
		D3DXVec3TransformCoord(&m_posAfter, &D3DXVECTOR3(vDir.z, vStart.y, vDir.z), &invmat);

		//	レイ方向情報を変換
		D3DXVec3Normalize(&direction, &(m_posAfter - m_posBefore));
		//Rayを飛ばす


		DWORD dwPolyIndex;
		D3DXIntersect(pObj->GetMesh(), &m_posBefore, &direction, &boHit, &dwPolyIndex, NULL, NULL, pfDistance, NULL, NULL);
		//D3DXIntersect(pObj->m_pMesh, &vStart, &vDir, &boHit, &dwPolyIndex, NULL, NULL, pfDistance, NULL, NULL);
		if (boHit)
		{
			//交差しているポリゴンの頂点を見つける
			D3DXVECTOR3 vVertex[3];
			FindVerticesOnPoly(pObj->m_pMesh, dwPolyIndex, vVertex);
			D3DXPLANE p;
			//その頂点から平面方程式を得る
			D3DXPlaneFromPoints(&p, &vVertex[0], &vVertex[1], &vVertex[2]);
			//平面方程式の係数が法線の成分
			pvNormal->x = p.a;
			pvNormal->y = p.b;
			pvNormal->z = p.c;

			CDebugProc::Log("モデルのアドレス : %s\n", pObj->m_Add.c_str());

			return true;
		}

		pSceneNow = pSceneNext;														//次回アップデート対象を格納
	}

	return false;
}

//
//HRESULT FindVerticesOnPoly(LPD3DXMESH pMesh,DWORD dwPolyIndex,D3DXVECTOR3* pvVertices )
//そのポリゴンの頂点を見つける
HRESULT CObject::FindVerticesOnPoly(LPD3DXMESH pMesh, DWORD dwPolyIndex, D3DXVECTOR3* pvVertices)
{
	DWORD dwStride = pMesh->GetNumBytesPerVertex();
	DWORD dwVertexNum = pMesh->GetNumVertices();
	DWORD dwPolyNum = pMesh->GetNumFaces();
	WORD* pwPoly = NULL;
	pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pwPoly);

	BYTE *pbVertices = NULL;
	FLOAT* pfVetices = NULL;
	LPDIRECT3DVERTEXBUFFER9 VB = NULL;
	pMesh->GetVertexBuffer(&VB);
	if (SUCCEEDED(VB->Lock(0, 0, (VOID**)&pbVertices, 0)))
	{
		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3]];
		pvVertices[0].x = pfVetices[0];
		pvVertices[0].y = pfVetices[1];
		pvVertices[0].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 1]];
		pvVertices[1].x = pfVetices[0];
		pvVertices[1].y = pfVetices[1];
		pvVertices[1].z = pfVetices[2];

		pfVetices = (FLOAT*)&pbVertices[dwStride*pwPoly[dwPolyIndex * 3 + 2]];
		pvVertices[2].x = pfVetices[0];
		pvVertices[2].y = pfVetices[1];
		pvVertices[2].z = pfVetices[2];

		pMesh->UnlockIndexBuffer();
		VB->Unlock();
		VB->Release();
	}
	return S_OK;
}

//=============================================================================
// 消されたオブジェクトの復活
//=============================================================================
void CObject::RecoveryActive(void)
{
	CScene *pSceneNext = NULL;														// 初期化
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_MODEL);					// 先頭アドレスの取得

	// 次がなくなるまで繰り返す
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_MODEL);		//次回アップデート対象を控える
		CObject *pObj = (CObject*)pSceneNow;								// クラスチェンジ(床)
		pObj->SetActive(true);

		if (pObj->m_pSphere != NULL)
		{
			pObj->m_pSphere->SetActive(true);
		}
		if (pObj->m_pBox != NULL)
		{
			pObj->m_pBox->SetActive(true);
		}

		pSceneNow = pSceneNext;								//次回アップデート対象を格納
	}
}

#ifdef _DEBUG
//=============================================================================
// デバッグ処理
//=============================================================================
void CObject::Debug(void)
{

}
#endif
//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "ranking.h"
#include "fade.h"
#include "manager.h"
#include "scene.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "bg.h"
#include "number.h"
#include "sound.h"
#include "ui.h"
#include "counter.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXTFILE_RANKING	"data/stage/RANKING.txt"		//ランキングのデータ読み込み

//=============================================================================
// 静的メンバ変数
//=============================================================================
int CRanking::m_nResult = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CRanking::CRanking()
{
	m_pUi = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	CBg::Load();				// 背景の読み込み
	CBg::Create();				// 背景の作成

	m_pUi = CUi::Create();

	if (m_pUi != NULL)
	{
		m_pUi->LoadScript("data/text/ui/ranking.txt");

		CCounter *pCounter = m_pUi->GetCounter("1");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(100000);
		}

		pCounter = m_pUi->GetCounter("2");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(200000);
		}

		pCounter = m_pUi->GetCounter("3");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(300000);
		}

		pCounter = m_pUi->GetCounter("4");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(400000);
		}

		pCounter = m_pUi->GetCounter("5");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(500000);
		}
	}


	//// =============== タイトル ================ //
	//CScene2D *pScene2D = CScene2D::Create(CScene::PRIORITY_UI);

	//if (pScene2D != NULL)
	//{// pScene2Dが存在していたとき
	//	pScene2D->BindTexture("data/tex/ranking.png");		// 画像の読み込み
	//	pScene2D->SetPosition(D3DXVECTOR3(300.0f, 100.0f, 0.0f));					// 位置設定
	//	pScene2D->SetSize(D3DXVECTOR3(500.0f, 100.0f, 0.0f));						// 大きさ設定
	//	pScene2D->SetTransform();													// 頂点情報の更新
	//}

	// ランキングを読み込む
	LoadRanking();

	// ランキングを整理する
	ConvertRanking(m_nResult);

	CCounter *pCounter = m_pUi->GetCounter("1");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_aScore[0]);
	}

	pCounter = m_pUi->GetCounter("2");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_aScore[1]);
	}

	pCounter = m_pUi->GetCounter("3");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_aScore[2]);
	}

	pCounter = m_pUi->GetCounter("4");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_aScore[3]);
	}

	pCounter = m_pUi->GetCounter("5");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_aScore[4]);
	}

	// ランキングを保存する
	SaveRanking();

	m_nCount = 0;

	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CRanking::Uninit(void)
{
	//ポリゴンの開放
	CScene::ReleaseAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CRanking::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (m_nCount > 15)
	{
		for (int nCount = 0; nCount < 5; nCount++)
		{
			if (m_aScore[nCount] == m_nResult)
			{
				char aIndex[8];
				memset(&aIndex, 0, sizeof(aIndex));

				sprintf(aIndex, "%d", nCount + 1);

				CCounter *pCounter;
				pCounter = m_pUi->GetCounter(aIndex);

				if (pCounter != NULL)
				{
					if (pCounter->GetActive())
					{
						pCounter->SetActive(false);
						pCounter->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
					}
					else
					{
						pCounter->SetActive(true);
						pCounter->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
					}
				}
			}
		}

		m_nCount = 0;
	}
	else
	{
		m_nCount++;
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//フェードが処理をしていないとき
		if (pInputKeyboard != NULL)
		{// キーボードが存在していたとき
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// 指定のキーが押されたとき
				CSound *pSound = CManager::GetSound();				// サウンドの取得

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// ダメージ音の再生

				CFade::SetFade(CManager::MODE_TITLE, CFade::FADETYPE_SLIDE);					//フェードを入れる
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// コントローラーが生成されているとき
		 //ゲームの遷移
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// ゲームパッドのAボダンが押されたとき
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// ゲームパッドのSTARTボタンが押されたとき
			{
				CSound *pSound = CManager::GetSound();				// サウンドの取得

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// ダメージ音の再生

				CFade::SetFade(CManager::MODE_TITLE, CFade::FADETYPE_SLIDE);					//フェードを入れる
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
// 結果の設定
//=============================================================================
void CRanking::SetResultIndex(int nScore)
{
	m_nResult = nScore;
}

//=============================================================================
// 結果の整理処理
//============================================================================
void CRanking::ConvertRanking(int nNumber)
{
	int nCount, nCountA;
	int nNum01;
	m_aScore[MAX_RANK - 1] = nNumber;

	//データのソートを実行
	for (nCount = 0; nCount < MAX_RANK; nCount++)
	{
		for (nCountA = 0; nCountA < MAX_RANK - nCount; nCountA++)
		{
			if (m_aScore[nCountA] < m_aScore[nCountA + 1])
			{
				nNum01 = m_aScore[nCountA];					//小さかった数字を入れ替える
				m_aScore[nCountA] = m_aScore[nCountA + 1];	//大きかった数字を格納
				m_aScore[nCountA + 1] = nNum01;				//小さい数字を入れる
			}
		}
	}
}

//=============================================================================
// 結果の保存処理
//=============================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile;
	int nCount;

	pFile = fopen(TEXTFILE_RANKING, "w");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < 5; nCount++)
		{
			//データの保存
			fprintf(pFile, "第%d位\n", nCount + 1);
			fprintf(pFile, "%d\n", m_aScore[nCount]);
		}
		fclose(pFile);
	}
}

//=============================================================================
// 結果の読み込み処理
//=============================================================================
void CRanking::LoadRanking(void)
{
	FILE *pFile;
	int nCount;
	char x[128];

	pFile = fopen(TEXTFILE_RANKING, "r");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < 5; nCount++)
		{
			//データのロード
			fscanf(pFile, "%s", &x[0]);
			fscanf(pFile, "%d", &m_aScore[nCount]);
		}
		fclose(pFile);
	}
}
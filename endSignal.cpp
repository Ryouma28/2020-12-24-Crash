//=============================================================================
//
// 終了宣言処理 [endSignal.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "endSignal.h"
#include "manager.h"
#include "renderer.h"
#include "time.h"
#include "ui.h"
#include "counter.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "fade.h"
#include "ranking.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CEndSignal::CEndSignal(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene(obj)
{
	m_pUi = NULL;
	m_bEnd = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEndSignal::~CEndSignal()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CEndSignal::Init(void)
{
	m_pUi = CUi::Create();

	if (m_pUi != NULL)
	{
		m_pUi->LoadScript("data/text/ui/EndSignal.txt");
		m_pUi->SetPosition(D3DXVECTOR3(640.0f, -50.0f, 0.0f));
	}

	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != NULL)
	{
		CRanking::SetResultIndex(pPlayer->GetPoint());	// ランキングに今回の得点を送る
		pPlayer->SetEvent(true);
	}
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CEndSignal::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CEndSignal::Update(void)
{
	if (m_pUi == NULL) return;

	if (m_pUi->GetOpType() == CUi::OPERATIONTYPE_NONE)
	{
		m_pUi->SetNextOperation();
		if (CFade::GetFade() == CFade::FADE_NONE)
		{//フェードが処理をしていないとき
			CFade::SetFade(CManager::MODE_RANKING, CFade::FADETYPE_SLIDE);					//フェードを入れる
		}

		Release();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEndSignal::Draw(void)
{
}

//=============================================================================
// クリエイト処理
//=============================================================================
CEndSignal *CEndSignal::Create(void)
{
	CEndSignal *pEndSignal;
	pEndSignal = new CEndSignal(PRIORITY_UI);

	if (pEndSignal != NULL)
	{// 数字が存在していたとき
		pEndSignal->Init();					// 初期化処理
	}
	return pEndSignal;
}
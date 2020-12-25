//=============================================================================
//
// fever処理 [fever.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "fever.h"
#include "manager.h"
#include "renderer.h"
#include "time.h"
#include "ui.h"
#include "counter.h"
#include "game.h"
#include "player.h"
#include "sound.h"
#include "scene2D.h"
#include "object.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFever::CFever(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene(obj)
{
	m_pTime = NULL;
	m_pUi = NULL;
	m_bEnd = false;
	m_pScene2D = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CFever::~CFever()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFever::Init(void)
{
	m_pTime = CTime::Create();

	if (m_pTime != NULL)
	{
		m_pTime->SetSeconds(5);
		m_pTime->SetUpdateTimer(false);
	}

	m_pUi = CUi::Create();

	if (m_pUi != NULL)
	{
		m_pUi->LoadScript("data/text/ui/fever.txt");
		m_pUi->SetPosition(D3DXVECTOR3(640.0f, -50.0f, 0.0f));
	}

	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != NULL)
	{
		pPlayer->SetFeverMode(true);
	}

	m_pScene2D = CScene2D::Create(PRIORITY_UI);

	if(m_pScene2D != NULL)
	{
		m_pScene2D->BindTexture("data/tex/Gradation.png");
		m_pScene2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		m_pScene2D->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		m_pScene2D->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		m_pScene2D->SetTransform();
	}
	return S_OK;
}

//=============================================================================
// 開放処理
//=============================================================================
void CFever::Uninit(void)
{
	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		m_pTime->Release();
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CFever::Update(void)
{
	if (m_pUi == NULL) return;

	if (m_pTime == NULL) return;

	if (m_pUi->GetOpType() == CUi::OPERATIONTYPE_NONE)
	{
		if (!m_bEnd)
		{
			m_pTime->SetUpdateTimer(true);
			m_bEnd = true;
		}

		if (m_pTime->GetSeconds() <= 0)
		{
			CPlayer *pPlayer = CGame::GetPlayer();

			if (pPlayer != NULL)
			{
				pPlayer->SetFeverMode(false);
				CObject::RecoveryActive();
			}

			if (m_pScene2D != NULL)
			{
				m_pScene2D->Release();
				m_pScene2D = NULL;
			}

			m_pUi->SetNextOperation();
			Release();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFever::Draw(void)
{
}

//=============================================================================
// クリエイト処理
//=============================================================================
CFever *CFever::Create(void)
{
	CFever *pFever;
	pFever = new CFever(PRIORITY_UI);

	if (pFever != NULL)
	{// 数字が存在していたとき
		pFever->Init();					// 初期化処理
	}
	return pFever;
}
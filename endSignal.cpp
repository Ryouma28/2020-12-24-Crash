//=============================================================================
//
// �I���錾���� [endSignal.cpp]
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
// �R���X�g���N�^
//=============================================================================
CEndSignal::CEndSignal(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene(obj)
{
	m_pUi = NULL;
	m_bEnd = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEndSignal::~CEndSignal()
{

}

//=============================================================================
// ����������
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
		CRanking::SetResultIndex(pPlayer->GetPoint());	// �����L���O�ɍ���̓��_�𑗂�
		pPlayer->SetEvent(true);
	}
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CEndSignal::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CEndSignal::Update(void)
{
	if (m_pUi == NULL) return;

	if (m_pUi->GetOpType() == CUi::OPERATIONTYPE_NONE)
	{
		m_pUi->SetNextOperation();
		if (CFade::GetFade() == CFade::FADE_NONE)
		{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
			CFade::SetFade(CManager::MODE_RANKING, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
		}

		Release();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEndSignal::Draw(void)
{
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CEndSignal *CEndSignal::Create(void)
{
	CEndSignal *pEndSignal;
	pEndSignal = new CEndSignal(PRIORITY_UI);

	if (pEndSignal != NULL)
	{// ���������݂��Ă����Ƃ�
		pEndSignal->Init();					// ����������
	}
	return pEndSignal;
}
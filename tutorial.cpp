//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "tutorial.h"
#include "fade.h"
#include "manager.h"
#include "scene.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "bg.h"
#include "number.h"
#include "ranking.h"
#include "sound.h"
#include "ui.h"
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTutorial::Init(void)
{

	CScene2D *pBack = CScene2D::Create(CScene::PRIORITY_UI);

	pBack->BindTexture("data/tex/tutorial.jpg");
	pBack->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
	pBack->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	pBack->SetTransform();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_GAME, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
		 //�Q�[���̑J��
			if (pInputController->GetControllerTrigger(0, JOYPADKEY_A) ||			// �Q�[���p�b�h��A�{�_���������ꂽ�Ƃ�
				pInputController->GetControllerTrigger(0, JOYPADKEY_START))			// �Q�[���p�b�h��START�{�^���������ꂽ�Ƃ�
			{
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_GAME, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}
//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "fade.h"
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "title.h"
#include "bg.h"
#include "inputController.h"
#include "meshField.h"
#include "object.h"
#include "camera.h"
#include "sky.h"
#include "sound.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	m_fMove = 0.0f;
	m_fAlpha = 1.0f;
	m_bAlpha = false;

	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		pBack[nCnt] = CScene2D::Create(CScene::PRIORITY_UI);
	}

	if (pBack[0] != NULL)
	{
		pBack[0]->BindTexture("data/tex/titleBG.png");
		pBack[0]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		pBack[0]->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		pBack[0]->SetTransform();
	}
	if (pBack[1] != NULL)
	{
		pBack[1]->BindTexture("data/tex/snow01.png");
		pBack[1]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		pBack[1]->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		pBack[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		pBack[1]->SetTransform();
	}
	if (pBack[2] != NULL)
	{
		pBack[2]->BindTexture("data/tex/title.png");
		pBack[2]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		pBack[2]->SetSize(D3DXVECTOR3(500, 700, 0.0f));
		pBack[2]->SetTransform();
	}
	if (pBack[3] != NULL)
	{
		pBack[3]->BindTexture("data/tex/PRESS START.png");
		pBack[3]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 520, 0.0f));
		pBack[3]->SetSize(D3DXVECTOR3(300, 120, 0.0f));
		pBack[3]->SetTransform();
	}

	// ��̍쐬
	CSky::Create();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CTitle::Uninit(void)
{
	//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();
	CInputController *pInputController = CManager::GetInputController();

	if (pBack[1] != NULL)
	{
		m_fMove -= 0.001f;
		pBack[1]->BindTexture("data/tex/snow01.png");
		pBack[1]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f));
		pBack[1]->SetSize(D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
		pBack[1]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		pBack[1]->SetDrawPos(D3DXVECTOR2(0.0f, m_fMove));
		pBack[1]->SetTransform();
	}
	if (pBack[3] != NULL)
	{
		pBack[3]->BindTexture("data/tex/PRESS START.png");
		pBack[3]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH / 2, 520, 0.0f));
		pBack[3]->SetSize(D3DXVECTOR3(300, 120, 0.0f));

		if (m_bAlpha)
		{
			m_fAlpha += 0.01f;

			if (m_fAlpha > 1.0f)
			{
				m_bAlpha = false;
			}
		}
		else
		{
			m_fAlpha -= 0.01f;

			if (m_fAlpha < 0)
			{
				m_bAlpha = true;
			}
		}
		pBack[3]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
		pBack[3]->SetTransform();
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pInputKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pInputKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_TUTORIAL, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
		if (pInputController->GetJoypadUse(0))
		{// �R���g���[���[����������Ă���Ƃ�
			//�Q�[���̑J��
			if (pInputController->GetControllerPress(0, JOYPADKEY_START))		// �X�^�[�g�{�^�����������Ƃ�
			{
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾

				pSound->PlaySoundA(SOUND_LABEL_SE_Decision);			// �_���[�W���̍Đ�

				CFade::SetFade(CManager::MODE_TUTORIAL, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CTitle::LoadAsset(void)
{

}
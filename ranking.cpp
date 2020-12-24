//=============================================================================
//
// �����L���O���� [ranking.cpp]
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

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXTFILE_RANKING	"data/stage/RANKING.txt"		//�����L���O�̃f�[�^�ǂݍ���

//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
int CRanking::m_nResult = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	CBg::Load();				// �w�i�̓ǂݍ���
	CBg::Create();				// �w�i�̍쐬

	// =============== �^�C�g�� ================ //
	CScene2D *pScene2D = CScene2D::Create(CScene::PRIORITY_UI);

	if (pScene2D != NULL)
	{// pScene2D�����݂��Ă����Ƃ�
		pScene2D->BindTexture("data/tex/ranking.png");		// �摜�̓ǂݍ���
		pScene2D->SetPosition(D3DXVECTOR3(300.0f, 100.0f, 0.0f));					// �ʒu�ݒ�
		pScene2D->SetSize(D3DXVECTOR3(500.0f, 100.0f, 0.0f));						// �傫���ݒ�
		pScene2D->SetTransform();													// ���_���̍X�V
	}

	// �����L���O��ǂݍ���
	LoadRanking();

	// �����L���O�𐮗�����
	ConvertRanking(m_nResult);

	// �����L���O��ۑ�����
	SaveRanking();

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CRanking::Uninit(void)
{
	//�|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
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

				CFade::SetFade(CManager::MODE_TITLE, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
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

				CFade::SetFade(CManager::MODE_TITLE, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{

}

//=============================================================================
// ���ʂ̐ݒ�
//=============================================================================
void CRanking::SetResultIndex(int nScore)
{
	m_nResult = nScore;
}

//=============================================================================
// ���ʂ̐�������
//============================================================================
void CRanking::ConvertRanking(int nNumber)
{
	int nCount, nCountA;
	int nNum01;
	m_aScore[MAX_RANK - 1] = nNumber;

	//�f�[�^�̃\�[�g�����s
	for (nCount = 0; nCount < MAX_RANK; nCount++)
	{
		for (nCountA = 0; nCountA < MAX_RANK - nCount; nCountA++)
		{
			if (m_aScore[nCountA] < m_aScore[nCountA + 1])
			{
				nNum01 = m_aScore[nCountA];					//�������������������ւ���
				m_aScore[nCountA] = m_aScore[nCountA + 1];	//�傫�������������i�[
				m_aScore[nCountA + 1] = nNum01;				//����������������
			}
		}
	}
}

//=============================================================================
// ���ʂ̕ۑ�����
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
			//�f�[�^�̕ۑ�
			fprintf(pFile, "��%d��\n", nCount + 1);
			fprintf(pFile, "%d\n", m_aScore[nCount]);
		}
		fclose(pFile);
	}
}

//=============================================================================
// ���ʂ̓ǂݍ��ݏ���
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
			//�f�[�^�̃��[�h
			fscanf(pFile, "%s", &x[0]);
			fscanf(pFile, "%d", &m_aScore[nCount]);
		}
		fclose(pFile);
	}
}
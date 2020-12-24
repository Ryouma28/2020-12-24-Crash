//=============================================================================
//
// �Q�[������ [game.cpp]
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
#include "ranking.h"
#include "inputKeyboard.h"


//=============================================================================
// �ÓI�����o�ϐ�
//=============================================================================
CMeshField	*CGame::m_pMeshField = NULL;		// ���b�V���t�B�[���h�̃|�C���^
CPlayer		*CGame::m_pPlayer = NULL;			// �v���C���[�̃|�C���^
CMeshSphere *CGame::m_pMeshSphere = NULL;		// ���b�V���X�t�B�A�̃|�C���^
CEnemy		*CGame::m_pEnemy = NULL;			// �G�̃|�C���^
CSky		*CGame::m_pSky = NULL;				// ��̃|�C���^
CHouse		*CGame::m_pHouse = NULL;			// �Ƃ̃|�C���^
CSpeed		*CGame::m_pSpeed = NULL;			// �����̃|�C���^

CUi			*CGame::m_pUi = NULL;				// UI�̃|�C���^
CTime		*CGame::m_pTime = NULL;				// �J�E���^�̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//Init����
//=============================================================================
HRESULT CGame::Init(void)
{
	// �G�t�F�N�g�̐���
	CEffect::Create();
	m_bRate = false;
	// �v���C���[�̐���
	m_pPlayer = NULL;
	m_pPlayer = CPlayer::Create();

	// ��̍쐬
	m_pSky = CSky::Create();

	CObject::LoadScript();

	// �e��A�Z�b�g�̐������ݒu
	CMeshField::LoadRand("data/text/stage/rand.txt", false);			// �����̓Ǎ�
	//CObject::LoadModel("data/stage/object.csv");					// ���f�����̓Ǎ�
	//CEnemy::LoadEnemy("data/stage/enemy.csv");					// �G���̓Ǎ�

	// ���Ԃ̃N���G�C�g����
	CTime::Create();

	//// ���f�����̓ǂݍ���
	//CObject::LoadModelTest("data/text/stage/stage_1.txt");

	// ���f�����̓ǂݍ���
	//CObject::LoadModelTest("data/text/model.txt");

	//// �l�b�g���[�N�ł̃Q�[��������������
	//CManager::GetNetwork()->InitGame();

	CStartSignal::Create();

	return S_OK;
}

//=============================================================================
// Update�֐�
//=============================================================================
void CGame::Update(void)
{
	CNetwork *pNetwork = CManager::GetNetwork();

	if (pNetwork != NULL)
	{
		pNetwork->Create();
	}
#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	if (pKeyboard->GetTriggerKeyboard(DIK_0))
	{
		CStartSignal::Create();
	}

	if (CFade::GetFade() == CFade::FADE_NONE)
	{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		if (pKeyboard != NULL)
		{// �L�[�{�[�h�����݂��Ă����Ƃ�
			if (pKeyboard->GetTriggerKeyboard(DIK_RETURN))
			{// �w��̃L�[�������ꂽ�Ƃ�
				//CRanking::SetResultIndex(nScore);	// �����L���O�ɍ���̓��_�𑗂�
				//CFade::SetFade(CManager::MODE_RANKING, CFade::FADETYPE_SLIDE);					//�t�F�[�h������
			}
		}
	}

#endif
}

//=============================================================================
// Draw�֐�
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// Uninit�֐�
//=============================================================================
void CGame::Uninit(void)
{
	CNetwork *pNetwork = CManager::GetNetwork();

	if (pNetwork != NULL)
	{// �l�b�g���[�N�����݂��Ă����Ƃ�
		pNetwork->StopUpdate();				// �X�V��~�\��
		pNetwork->CloseTCP();				// �T�[�o�[�Ƃ̑�����߂�
	}

	CObject::Unload();

	// �|���S���̊J��
	CScene::ReleaseAll();
}

//=============================================================================
// �A�Z�b�g�̓ǂݍ���
//=============================================================================
void CGame::LoadAsset(void)
{
	// =============== �A�Z�b�g�̓ǂݍ��� ===============//
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
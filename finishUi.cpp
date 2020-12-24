//=============================================================================
//
// �S�[���pUI���� [finishUi.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "finishUi.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFinishUi::CFinishUi(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene2D(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_UI);				// �I�u�W�F�N�g�^�C�v
	m_move = D3DXVECTOR3_ZERO;			// �ړ���
	m_bGravity = true;					// �d��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFinishUi::~CFinishUi()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFinishUi::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();
	D3DXVECTOR3 pos = GetPosition();							// �v���C���[�̈ʒu�擾

	pos = D3DXVECTOR3(SCREEN_WIDTH / 2, -150.0f, 0.0f);			// �v���C���[�̈ʒu�ݒ�

	// �L�����N�^�[�̏���������
	CScene2D::Init();

	// �ʒu�̐ݒ�
	SetPosition(pos);

	SetSize(D3DXVECTOR3(700.0f, 500.0f, 0.0f));

	SetTransform();

	// �e�N�X�`���̐ݒ�
	BindTexture("data/tex/Finish.png");

	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CFinishUi::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFinishUi::Update(void)
{
	if (m_bGravity)
	{
		D3DXVECTOR3 pos;
		pos = GetPosition();				// �ʒu�̎擾

		// �ʒu�X�V
		pos += m_move;

		// ����
		m_move.y += 0.98f;

		if (pos.y > SCREEN_HEIGHT / 2)
		{// �ʒu�����ʈȉ��������Ƃ�
			pos.y = SCREEN_HEIGHT / 2;
			m_move *= -0.6f;			// �d�͂̒l�����������ړ��ʂ𔽓]������

			if (fabs(m_move.y) < 5.0f)
			{// �㉺�̈ړ��ʂ����l�ȉ��������Ƃ�
				m_bGravity = false;			// �d�͂�؂�
			}
		}

		// �ʒu�ݒ�
		SetPosition(pos);
		SetTransform();

	}
	// �L�����N�^�[�̍X�V����
	CScene2D::Update();

#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFinishUi::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CFinishUi *CFinishUi::Create(void)
{
	CFinishUi *pFinishUi;
	pFinishUi = new CFinishUi(CScene::PRIORITY_UI);
	pFinishUi->Init();
	return pFinishUi;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CFinishUi::Load(void)
{
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CFinishUi::Debug(void)
{

}
#endif
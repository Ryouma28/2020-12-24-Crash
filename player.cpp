//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "player.h"
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputController.h"
#include "scene3D.h"
#include "meshField.h"
#include "model.h"
#include "meshCapsule.h"
#include "colliderSphere.h"
#include "meshCube.h"
#include "enemy.h"
#include "camera.h"
#include "colliderBox.h"
#include "stage.h"
#include "sound.h"
#include "scene2D.h"
#include "effect.h"
#include "result.h"
#include "fade.h"
#include "takaseiLibrary.h"
#include "sceneX.h"
#include "speed.h"
#include "wall.h"
#include "object.h"
#include "number.h"
#include "network.h"
#include "distanceNext.h"
#include "ui.h"
#include "shadow.h"
#include "meshSphere.h"
#include "counter.h"

//=============================================================================
// �O���錾
//=============================================================================
LPD3DXMESH CPlayer::m_pMeshModel = NULL;
LPD3DXBUFFER CPlayer::m_pBuffMatModel = NULL;
DWORD CPlayer::m_nNumMatModel = NULL;

//=============================================================================
// �}�N����`
//=============================================================================
#define	SCRIPT_CAR01 "data/animation/car01.txt"		// ��01�̃��f�����A�h���X
#define	SCRIPT_CAR02 "data/animation/car02.txt"		// ��01�̃��f�����A�h���X
#define	SCRIPT_CAR03 "data/animation/car03.txt"		// ��01�̃��f�����A�h���X
#define	SCRIPT_CAR04 "data/animation/car04.txt"		// ��01�̃��f�����A�h���X

#define ROT_AMOUNT 0.1f								// ��]�̍������炵�Ă�����
#define ROT_SPEED_DRIFT 0.5f						// �h���t�g����]���x
#define MODEL_FRONT 2								// ���f���O�֔ԍ�
#define MODEL_REAR 1								// ���f����֔ԍ�
#define MODEL_TIRE 2								// �^�C�����f���̐�
#define CAMERA_ROT_SPEED 0.4f						// �J�����̉�]���x
#define TIRE_ROT_SPEED 0.1f							// �^�C���̉�]���x
#define ACCEKERATION 3.0f							// �h���t�g�����x�����l
#define ACCEKERATION_ADDITION 0.5f					// �h���t�g�����x���Z��
#define DRIFT_DECREACE 0.6f							// �h���t�g�����x����
#define DRIFT_DEST 0.25f							// �h���t�g���^�C���̌���
#define	INIT_ROT 3.14f
#define PLAYER_SPEED 5.0f							// �v���C���[���x

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(CScene::PRIORITY obj = CScene::PRIORITY_PLAYER) : CSceneX(obj)
{
	// �D��x�̐ݒ�
	SetObjType(CScene::PRIORITY_PLAYER);				// �I�u�W�F�N�g�^�C�v
	SetSize(D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	m_vecAxis = ZeroVector3;							// ��]���̏�����
	m_fValueRot = 0.0f;									// ��]�p�̏�����
	m_nLife = 100;										// �̗͂̏�����
	m_rot = D3DXVECTOR3(0.0f, INIT_ROT, 0.0f);			// ��]�̏�����
	m_fSpeed = 0;										// �X�s�[�h�̏�����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ��ʂ̏�����
	m_dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ړ���̏�����
	m_difference = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ���̏�����
	m_vectorOld = ZeroVector3;							// �O��̃x�N�g������
	m_cameraRot = D3DXVECTOR3(0, D3DX_PI, 0);			// �J�����̉�]��񏉊���
	m_pColPlayerSphere = NULL;							// �v���C���[�����蔻��|�C���^�̏�����
	m_pColPlayerBox = NULL;
	m_pDistanceNext = NULL;								// ���̃v���C���[�Ƃ̋�����UI
	m_bHit = false;										// �����蔻��t���O�̏�������
	m_bJump = false;									// �W�����v�t���O�̏�����
	m_nActionCount = 0;									// �A�N�V�����J�E���^�̏�����
	m_nParticleCount = 0;								// �p�[�e�B�N���J�E���^�̏�����
	m_nPointNum = 0;									// �|�C���g�ԍ�������
	m_fDeathblow = 0.0f;								// �K�E�Z�|�C���g�̏�����
	m_fAcceleration = ACCEKERATION;						// �����x
	m_bEvent = true;									// �C�x���g�����t���O�̏�����
	m_bMove = false;									// ���ݓ����Ă��邩�̃t���O
	m_bAccel = false;									// �A�N�Z���������Ă��邩�̃t���O
	m_bColliderWithWall = true;							// �ǂ̓����蔻��
	m_bGoal = false;									// �S�[���t���O
	m_nPoint = 0;
	m_pUi = NULL;
	m_nCntSound = 0;
	m_bFever = false;

	m_pRank = NULL;

	m_nRound = 0;			// ���݂̎����

	for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
	{
		m_bDrift[nCnt] = false;							// �h���t�g�t���O����
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	CRenderer *pRenderer = CManager::GetRenderer();
	CNetwork *pNetwork = CManager::GetNetwork();

	//�f�o�C�X���擾����
	pDevice = pRenderer->GetDevice();
	m_fPuzzleMaxSPeed = 0;
	CCamera *pCamera = CManager::GetCamera();
	D3DXVECTOR3 pos = GetPosition();							// �v���C���[�̈ʒu�擾

	pos = D3DXVECTOR3(7879.83f, 350.0f, -741.0f);			// �v���C���[�̈ʒu�ݒ�

	m_pUi = CUi::Create();

	if (m_pUi != NULL)
	{
		m_pUi->LoadScript("data/text/ui/score.txt");
		m_pUi->SetPosition(D3DXVECTOR3(874.00, 51.00, 0.00));

		CCounter *pCounter = m_pUi->GetCounter("time");

		if (pCounter != NULL)
		{
			pCounter->SetNumber(0);
		}
	}
	if (pCamera != NULL)
	{
		pCamera->SetPosCamera(pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	}

	// �L�����N�^�[�̏���������
	CSceneX::Init();

	// �v���C���[�̓����蔻��𐶐�
	m_pColPlayerSphere = CColliderSphere::Create(false, 500.0f);

	if (m_pColPlayerSphere != NULL)
	{ //���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColPlayerSphere->SetScene(this);
		m_pColPlayerSphere->SetTag("player");										// �^�O �̐ݒ�
		m_pColPlayerSphere->SetPosition(pos);										// �ʒu �̐ݒ�
		m_pColPlayerSphere->SetOffset(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// �v���C���[�̓����蔻��𐶐�
	m_pColPlayerBox = CColliderBox::Create(true, D3DXVECTOR3(500.0f, 500.0f, 500.0f));

	if (m_pColPlayerBox != NULL)
	{ //���̂̃|�C���^��NULL�ł͂Ȃ��Ƃ�
		m_pColPlayerBox->SetScene(this);
		m_pColPlayerBox->SetTag("player");										// �^�O �̐ݒ�
		m_pColPlayerBox->SetPosition(pos);										// �ʒu �̐ݒ�
		m_pColPlayerBox->SetOffset(D3DXVECTOR3(0.0f, 20.0f, 0.0f));
	}

	// �ʒu�̐ݒ�
	SetPosition(pos);

	// �e�̐���
	//m_pShadow = CShadow::Create();
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CPlayer::Uninit(void)
{
	if (m_pColPlayerSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerSphere->Release();
	}

	if (m_pDistanceNext != NULL)
	{
		m_pDistanceNext->Uninit();
		delete m_pDistanceNext;
		m_pDistanceNext = NULL;
	}

	CSceneX::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	D3DXVECTOR3 pos;
	CSound *pSound = CManager::GetSound();
	CNetwork *pNetwork = CManager::GetNetwork();
	float fHeight = 0.0f;
	//CModel *pModel = GetModel();

	pos = GetPosition();				// �ʒu�̎擾

	//// �A�j���[�V�������̎擾
	//ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	//int currentKey = GetCurrentKey();
	//int currentFrame = GetCurrentFrame();
	//bool bAnimPlayState = GetAnimPlayState();

	// ���͏���
	if (!m_bHit)
	{// �������Ă��Ȃ������Ƃ�
		CCamera *pCamera = CManager::GetCamera();
		if (pCamera->GetStalker())
		{
			// ���͏���
			Input(pos);
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	VERTEX_PLANE plane = {};

	//CCollider::RayBlockCollision(pos, &pModel[0].GetMtxWorld(), 110, 250.0f, plane);

	D3DXVECTOR3 AB = plane.a - plane.b;
	D3DXVECTOR3 BC = plane.b - plane.c;

	D3DXVECTOR3 norwork;

	D3DXVec3Cross(&norwork, &BC, &AB);
	D3DXVec3Normalize(&norwork, &norwork);

	//CDebugProc::Log("a�n�_ : %f, %f, %f\n", plane.a.x, plane.a.y, plane.a.z);
	//CDebugProc::Log("b�n�_ : %f, %f, %f\n", plane.b.x, plane.b.y, plane.b.z);
	//CDebugProc::Log("c�n�_ : %f, %f, %f\n", plane.c.x, plane.c.y, plane.c.z);

	//���̍������擾����
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = GetScene(PRIORITY_FLOOR);									// �V�[���̐擪�A�h���X���擾

	CScene *pScene = NowFloor(pos);													// ���݂���t�B�[���h���擾

	if (pScene != NULL)
	{// �������Ă��鏰�����݂����Ƃ�
		m_size = GetSize();
		CMeshField *pFloor = (CMeshField*)pScene;									// �L���X�g
		fHeight = pFloor->GetHeight(pos);
		fHeight += 350.0f;
		pos.y = fHeight + (m_size.y * 5);

		RANDTYPE Type = pFloor->GetRandType();

		if (fabs(m_move.x) > 1.0f || fabs(m_move.y) > 1.0f || fabs(m_move.z) > 1.0f)
		{
			if (m_nCntSound > 50)
			{
				CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
				pSound->PlaySoundA(SOUND_LABEL_SE_Roll);			// �_���[�W���̍Đ�
				m_nCntSound = 0;
			}
			else
			{
				m_nCntSound++;
			}

			if (Type == RANDTYPE_GRASS)
			{
				if (m_size.x < 100000.0f)
				{

				}
			}
			else if (Type == RANDTYPE_SAND)
			{
				//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
			}
		}
		//if (animType == ANIMATIONTYPE_RUN)
		//{
		//	if (currentKey == 5 || currentKey == 0)
		//	{
		//		if (currentFrame == 0)
		//		{
		//			if (Type == RANDTYPE_GRASS)
		//			{
		//				//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_GRASS_1));
		//			}
		//			else if (Type == RANDTYPE_SAND)
		//			{
		//				//	pSound->PlaySoundA((SOUND_LABEL)(CManager::GetRand(3) + (int)SOUND_LABEL_SE_SAND_1));
		//			}
		//		}
		//	}
		//}
	}

	// �ʒu�X�V
	pos += m_move;

	// �W�����v���Ă��Ȃ��Ƃ�
	if (!m_bJump)
	{
		//// ����
		//m_move.x += (0 - m_move.x) * CManager::GetSpeedDampingRate();
		//m_move.z += (0 - m_move.z) * CManager::GetSpeedDampingRate();
		// ����
		m_move.x += (0 - m_move.x) * 0.001f;
		m_move.z += (0 - m_move.z) * 0.001f;
	}

	////�d�͏���
	//if (m_bJump)
	//{// �W�����v���Ă����Ƃ�
	//	if (animType != ANIMATIONTYPE_JUMP_1)
	//	{
	//		if (pos.y > fHeight)
	//		{// ���݂̍���������荂�������Ƃ�
	//			m_move.y += -0.7f;
	//		}
	//		else
	//		{// ���݂̍����������Ⴉ������
	//			pos.y = fHeight;											// ���̍��������߂�
	//			m_move.y = 0.0f;
	//			if (m_bJump)
	//			{
	//				// �����̃G�t�F�N�g�\��
	//				CEffect::SandSmokeEffect(pos);
	//				m_bJump = false;										// �W�����v�����ς���
	//				SetAnimPlayState(true);									// �A�j���[�V�����̍ĊJ
	//				AnimationSwitch(ANIMATIONTYPE_JUMP_5);					// �A�j���[�V�����̕ύX
	//				pSound->PlaySoundA(SOUND_LABEL_SE_LANDING);				// ���n���̍Đ�
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	pos.y = fHeight;											// ���̍��������߂�
	//}


	if (m_bColliderWithWall)
	{
		// �����蔻��Ǘ�����
		Collision();
	}

	if (m_pColPlayerSphere != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerSphere->SetPosition(pos);
	}

	if (m_pColPlayerBox != NULL)
	{// ����̓����蔻�肪���݂��Ă����Ƃ�
		m_pColPlayerBox->SetPosition(pos);
	}

	//	D3DXVECTOR3 move = CManager::Slip(playerPos + m_move, vNormal);// ����x�N�g�����v�Z

	//// ��ł̃v���C���[����
	//SlopeMove();

	// �L�����N�^�[�̍X�V����
	CSceneX::Update();

	if (m_pRank != NULL)
	{
		m_pRank->SetNumber(pNetwork->GetRank(pNetwork->GetId()));
	}

	if (m_pDistanceNext != NULL)
	{
		m_pDistanceNext->Update();
	}

	// �h���t�g���Ă����Ƃ�
	if (m_bDrift[DRIFT_LEFT] || m_bDrift[DRIFT_RIGHT])
	{
		// �p�[�e�B�N���S�̂̈ʒu�v�Z
		D3DXVECTOR3 vecPos = D3DXVECTOR3(-25.0f, 0.0f, -5.0f);
		D3DXMATRIX mtxMeshRot, mtxMeshTrans;				// �v�Z�p�}�g���b�N�X
		D3DXMATRIX mtx;										// ����̃}�g���b�N�X
		D3DXMATRIX mtxPlayer;

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		for (int nCount = 0; nCount < 2; nCount++)
		{
			CEffect::CreateEffect("spark", D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}

		// �p�[�e�B�N���S�̂̈ʒu�v�Z
		vecPos = D3DXVECTOR3(25.0f, 0.0f, -5.0f);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtx);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxPlayer);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, vecPos.x, vecPos.y, vecPos.z);
		D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxMeshTrans);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxMeshRot, 0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxMeshTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxMeshTrans);

		D3DXMatrixMultiply(&mtx, &mtx, &mtxPlayer);

		for (int nCount = 0; nCount < 2; nCount++)
		{
			CEffect::CreateEffect("spark", D3DXVECTOR3(mtx._41, mtx._42, mtx._43), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}

	//CDebugProc::Log("���x : %.2f", CSpeed::GetSpeed());

	if (CSpeed::GetSpeed() > 10)
	{
		CEffect::SandSmoke(pos + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	}


	if (pos.x >= 12000)
	{
		pos.x = 12000;
	}
	else if (pos.x <= -12000)
	{
		pos.x = -12000;
	}
	if (pos.z >= 12000)
	{
		pos.z = 12000;
	}
	else if (pos.z <= -12000)
	{
		pos.z = -12000;
	}
	// �ʒu�ݒ�
	SetPosition(pos);


#ifdef _DEBUG
	Debug();
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	CSceneX::DrawSnow();
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);
	pPlayer->Init();
	pPlayer->BindModel(m_pMeshModel, m_nNumMatModel, m_pBuffMatModel);
	return pPlayer;
}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CPlayer::Load(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	// �e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile(pDevice, PLAYER_TEX, &m_pTexture);
	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data/model/SnowBall.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMatModel, NULL, &m_nNumMatModel, &m_pMeshModel);

	return S_OK;
}

//=============================================================================
// �K�E�Z�|�C���g���̐ݒ�
//=============================================================================
void CPlayer::SetDeathblow(float nValue)
{
	m_fDeathblow = nValue;
}

//=============================================================================
// �C�x���g�t���O�̐ݒ�
//=============================================================================
void CPlayer::SetEvent(bool bValue)
{
	m_bEvent = bValue;
}

//=============================================================================
// �S�[���t���O�̐ݒ�
//=============================================================================
void CPlayer::SetGoalState(bool bValue)
{
	m_bGoal = bValue;
}

//=============================================================================
// fever���[�h�ؑ�
//=============================================================================
void CPlayer::SetFeverMode(bool bValue)
{
	m_bFever = bValue;
}

//=============================================================================
// �|�C���g���Z
//=============================================================================
void CPlayer::AddPoint(int nValue)
{
	m_nPoint += nValue;

	CCounter *pCounter = m_pUi->GetCounter("time");

	if (pCounter != NULL)
	{
		pCounter->SetNumber(m_nPoint);
	}
}

//=============================================================================
// �����蔻��(trigger)
//=============================================================================
void CPlayer::OnTriggerEnter(CCollider *col)
{
	std::string sTag = col->GetTag();
	//CModel *pModel = GetModel();
	std::vector<CObject*> pointObj = CObject::GetPointObj();

	if(col->GetScene() == NULL) return;
	if (col->GetScene()->GetObjType() == PRIORITY_ENEMY)
	{
		if (sTag == "weapon")
		{
			CSound *pSound = CManager::GetSound();				// �T�E���h�̎擾
		//	pSound->PlaySoundA(SOUND_LABEL_SE_PUNCH);			// �_���[�W���̍Đ�
			m_nLife -= 5;										// �̗͂����
			//AnimationSwitch(ANIMATIONTYPE_DAMAGE);				// �A�j���[�V������ύX

			D3DXVECTOR3 vec;

			vec = GetPosition() - col->GetPosition();			//���������߂�(���������߂邽��)
			D3DXVec3Normalize(&vec, &vec);						//���K������

			m_move.x = vec.x * 25;
			m_move.z = vec.z * 25;

			if (m_nLife < 0)
			{
				CResult::SetIdxKill(CEnemy::GetEnemyKill());			// Kill�������U���g�ɓn��
				CFade::SetFade(CManager::MODE_RESULT, CFade::FADETYPE_SLIDE);					// ���U���g�ɑJ��
			}
		}
	}
	//if (sTag == "wood")
	//{
	//	col->Release();
	//}
	//else if (sTag == "Building")
	//{
	//	col->Release();
	//}
	//else if (sTag == "Car")
	//{
	//	col->Release();
	//}
	//else if (sTag == "House")
	//{
	//	col->Release();
	//}
	if (sTag == "goal")
	{
		CNetwork *pNetwork = CManager::GetNetwork();
		pNetwork->SendTCP("GOAL", sizeof("GOAL"));
		m_bEvent = true;

		//if (CFade::GetFade() == CFade::FADE_NONE)
		//{//�t�F�[�h�����������Ă��Ȃ��Ƃ�
		//	CFade::SetFade(CManager::MODE_RESULT, CFade::FADETYPE_NORMAL);					//�t�F�[�h������
		//}
	}
}

//=============================================================================
// �����蔻��(collider)
//=============================================================================
void CPlayer::OnCollisionEnter(CCollider *col)
{
	std::string sTag = col->GetTag();

	if (sTag == "wood")
	{
		col->Release();
	}
	else if(sTag == "Building")
	{
		col->Release();
	}
	else if (sTag == "Car")
	{
		col->Release();
	}
	else if (sTag == "House")
	{
		col->Release();
	}
}

//========================================================================================
// �A�j���[�V�����t���[���̍ő吔�ɓ��B�����Ƃ��̏���
//========================================================================================
void CPlayer::BehaviorForMaxFrame(void)
{

}

//========================================================================================
// �A�j���[�V�����L�[�̍ő吔�ɓ��B�����Ƃ��̏���
//========================================================================================
void CPlayer::BehaviorForMaxKey(void)
{
	//CModel *pModel = GetModel();
	//ANIMATIONTYPE animType = (ANIMATIONTYPE)GetAnimType();
	//D3DXVECTOR3 rot = pModel[0].GetRotation();		// ��]�ʎ擾
}

//=============================================================================
// �߂��ɂ���G�̋߂��܂ňړ����鏈��
//=============================================================================
void CPlayer::MoveNearEnemy(void)
{
	bool bTarget = false;				// �͈͓��ɓG�����݂��Ă����Ƃ�
	float fMinDistance = 750000.0f;		// ���݂̍ŒZ����
	float fDistance = 0.0f;				// ���݂̋���
	D3DXVECTOR3 enemyPos;				// �ŒZ�����ɂ���G�̈ʒu
	CScene *pSceneNext = NULL;														// ������
	CScene *pSceneNow = CScene::GetScene(CScene::PRIORITY_ENEMY);					// �擪�A�h���X�̎擾

	// �����Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (pSceneNow != NULL)
	{
		pSceneNext = CScene::GetSceneNext(pSceneNow, CScene::PRIORITY_ENEMY);		//����A�b�v�f�[�g�Ώۂ��T����
		fDistance = CManager::GetDistance(GetPosition(), pSceneNow->GetPosition());		// ���������߂�

		if (fDistance < fMinDistance)
		{
			if (!bTarget)
			{// �^�[�Q�b�g�̑��݂��m�F����Ă��Ȃ������Ƃ�
				bTarget = true;				// �ʒu�̈ړ���������
			}

			enemyPos = pSceneNow->GetPosition();			// �G�̈ʒu���L�^����
		}

		pSceneNow = pSceneNext;								//����A�b�v�f�[�g�Ώۂ��i�[
	}

	if (bTarget)
	{// �ړ��̋����o�Ă����Ƃ�
		SetPosition(enemyPos);			// �ʒu�̈ړ�
	}
}

//=============================================================================
// �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	// �ǂ̓����蔻��
	//m_bHit = CollisionWall();
}

//=============================================================================
// �L�[���͏�񏈗�
//=============================================================================
void CPlayer::Input(D3DXVECTOR3 &pos)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̎擾
	CInputController *pGamepad = CManager::GetInputController();

	// �J�����̎擾
	CCamera *pCamera = CManager::GetCamera();

	D3DXVECTOR3 rot = pCamera->GetRotation();
	D3DXVECTOR3 Diff;	// �v�Z�p�i�[�ϐ�
	float nValueH = 0;									//�R���g���[���[
	float nValueV = 0;									//�R���g���[���[

	D3DXVECTOR3 cameraVec = D3DXVECTOR3(0, 0, 0);				// �J�����̕����x�N�g��
	m_fSpeed = 0;

	// ====================== �R���g���[���[ ====================== //
	if (!m_bEvent)
	{// �C�x���g���������Ă��Ȃ������Ƃ�
		if (pGamepad != NULL)
		{// �Q�[���p�b�h�����݂����Ƃ�
			if (pGamepad->GetJoypadUse(0))
			{// �g�p�\�������Ƃ�
				pGamepad->GetJoypadStickLeft(0, &nValueH, &nValueV);

				float		fSpeed = 0.0f;			// �v���C���[�̑��x
				float		fAngle = 0.0f;			// �X�e�B�b�N�p�x�̌v�Z�p�ϐ�
				float fMomentX = sinf(m_dest.y);			//X�p�x
				float fMomentZ = cosf(m_dest.y);			//Z�p�x

				// �p�x�̌v�Z���ĕ␳
				fAngle = atan2f(nValueH, nValueV);
				CTakaseiLibrary::RotRevision(&D3DXVECTOR3(0.0f, fAngle, 0.0f));

				// �X�e�B�b�N�̓|���ŃX�s�[�h������
				if (abs(nValueH) > abs(nValueV))
					fSpeed = (abs(nValueH)) * PLAYER_SPEED;	// ���̓|��
				else
					fSpeed = (abs(nValueV)) * PLAYER_SPEED;	// �c�̓|��

				// �X�e�B�b�N�̊p�x�ɂ���ăv���C���[�ړ�
				m_move.x += sinf(fAngle + rot.y) * fSpeed;
				m_move.z += -cosf(fAngle + rot.y) * fSpeed;

				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

				// �ړI�̌���������
				if (nValueH != 0 || nValueV != 0)
				{
					m_dest.y = D3DX_PI + fAngle + rot.y;
				}

				// ��]�̕␳
				CTakaseiLibrary::RotRevision(&m_dest);

				// �ړI�̉�]�̐ݒ�
				SetRotation(m_dest);

#ifdef _DEBUG
				CDebugProc::Log("�ړ��� : %.2f %.2f %.2f", m_move.x, m_move.y, m_move.z);

				if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 0��\n");
				}
				if (pGamepad->GetControllerPress(1, JOYPADKEY_A))
				{
					CDebugProc::Log("�R���g���[���[ : 1��\n");
				}

				CDebugProc::Log("�R���g���[���[H : %f\n", nValueH);
				CDebugProc::Log("�R���g���[���[V : %f\n", nValueV);
#endif
			}
		}

		// ====================== �L�[�{�[�h ====================== //

		if (pKeyboard->GetTriggerKeyboard(DIK_1))
		{
			for (int nCount = 0; nCount < 20; nCount++)
			{
				float fAngle = float(CManager::GetRand(314)) - float(CManager::GetRand(314));
				float fAngle_x = float(CManager::GetRand(314)) - float(CManager::GetRand(314));

				D3DXVECTOR3 particlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				particlePos.x = cosf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);
				particlePos.y = sinf(D3DX_PI + fAngle_x);
				particlePos.z = sinf(D3DX_PI + fAngle) * cosf(D3DX_PI + fAngle_x);

				fAngle = float(CManager::GetRand(314)) / 100.0f - float(CManager::GetRand(314)) / 100.0f;
				D3DXVECTOR3 rot;
				rot = D3DXVECTOR3(sinf(fAngle) * 10, cosf(fAngle) * 10, 0.0f);

				CEffect::SetEffect("data/tex/effect/rose_01.png",			// �p�[�e�B�N���̃^�C�v
					GetPosition(),											// �����ʒu
					D3DXVECTOR3(8.0f, 8.0f, 0.0f),							// �T�C�Y
					particlePos * 5.0f,										// �����x�N�g��
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					D3DXVECTOR3(0.02f, 0.02f, 0.0f),						// ��]�̕ω���
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f),						// �F�̕ω���
					EASINGTYPE_NONE,
					rot,													// �e�N�X�`���̉�]
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),						// �J���[
					200,													// �p�[�e�B�N���̐����J�E���g��
					-0.98f,													// �d��
					0,														// ��R
					true,													// �r���{�[�h
					0,														// �\������ӏ�(��)
					0,														// �\������ӏ�(�c)
					D3DXVECTOR3(0.0f, 0.0f, 0.0f),
					0,
					0,
					0,
					D3DXVECTOR2(1.0f, 1.0f),								// �摜�̕�����
					false,													// ���Z�����̗L��
					false,													// Z�o�b�t�@�̔�r�L��
					false													// �t�F�[�h
				);
			}

		}

		if (pKeyboard->GetTriggerKeyboard(DIK_2))
		{
			CEffect::PetalCluster(GetPosition(), D3DXVECTOR3(D3DX_PI / 2, m_rot.y, m_rot.z));
		}
	}

	if (!m_bEvent)
	{// �C�x���g���������Ă��Ȃ������Ƃ�

		float fMomentX = sinf(m_dest.y);			//X�p�x
		float fMomentZ = cosf(m_dest.y);			//Z�p�x

		if (pKeyboard->GetPressKeyboard(DIK_A))
		{// �����L�[���{�^���������ꂽ�Ƃ�
			if (pKeyboard->GetPressKeyboard(DIK_W))
			{// ����
				m_move.x += sinf(D3DX_PI * -0.25f)*1.0f;
				m_move.z += cosf(D3DX_PI * -0.25f)*1.0f;
				//�v���C���[����
				m_dest.y = (-D3DX_PI * -0.75f);
				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else if (pKeyboard->GetPressKeyboard(DIK_S))
			{// ����
				m_move.x += sinf(D3DX_PI * -0.75f)*1.0f;
				m_move.z += cosf(D3DX_PI * -0.75f)*1.0f;
				//�v���C���[����
				m_dest.y = (-D3DX_PI * -0.25f);
				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else
			{// ��
				m_move.x += sinf(D3DX_PI * -0.5f)*1.0f;
				m_move.z += cosf(D3DX_PI * -0.5f)*1.0f;
				//�v���C���[����
				m_dest.y = (-D3DX_PI * -0.5f);
				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
		}
		else if (pKeyboard->GetPressKeyboard(DIK_D))
		{// �����L�[�E�{�^���������ꂽ�Ƃ�
			if (pKeyboard->GetPressKeyboard(DIK_W))
			{// �E��
				m_move.x += sinf(D3DX_PI * 0.25f)*1.0f;
				m_move.z += cosf(D3DX_PI * 0.25f)*1.0f;
				//�v���C���[����
				m_dest.y = (-D3DX_PI * 0.75f);
				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else if (pKeyboard->GetPressKeyboard(DIK_S))
			{// �E��
				m_move.x += sinf(D3DX_PI * 0.75f)*1.0f;
				m_move.z += cosf(D3DX_PI * 0.75f)*1.0f;
				//�v���C���[����
				m_dest.y = (-D3DX_PI * 0.25f);
				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

			}
			else
			{// �E
				m_move.x += sinf(D3DX_PI * 0.5f)*1.0f;
				m_move.z += cosf(D3DX_PI * 0.5f)*1.0f;
				//�v���C���[����
				m_dest.y = (D3DX_PI * 0.5f);
				//�N�H�[�^�j�I����]����
				m_vecAxis.x = fMomentX * cosf(-D3DX_PI / 2.0f) - fMomentZ * sinf(-D3DX_PI / 2.0f);
				m_vecAxis.z = fMomentX * sinf(-D3DX_PI / 2.0f) + fMomentZ * cosf(-D3DX_PI / 2.0f);
			}
		}
		else if (pKeyboard->GetPressKeyboard(DIK_W))
		{// ��
			m_move.x += sinf(D3DX_PI * 0.0f)*1.0f;
			m_move.z += cosf(D3DX_PI * 0.0f)*1.0f;
			//�v���C���[����
			m_dest.y = (D3DX_PI * 1.0f);
			//�N�H�[�^�j�I����]����
			m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
			m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

		}
		else if (pKeyboard->GetPressKeyboard(DIK_S))
		{// ��
		 // �ړ���(��]�Ȃ�)
			m_move.x += sinf(D3DX_PI * 1.0f)*1.0f;
			m_move.z += cosf(D3DX_PI * 1.0f)*1.0f;

			//�v���C���[����
			m_dest.y = (D3DX_PI * 0.0f);

			//�N�H�[�^�j�I����]����
			m_vecAxis.x = fMomentX * cosf(D3DX_PI / 2.0f) - fMomentZ * sinf(D3DX_PI / 2.0f);
			m_vecAxis.z = fMomentX * sinf(D3DX_PI / 2.0f) + fMomentZ * cosf(D3DX_PI / 2.0f);

		}

		//�N�H�[�^�j�I��(��]��)
		m_fValueRot = sqrtf(m_move.x * m_move.x + m_move.z * m_move.z) * 0.01f;

		// -3.14��菬����������
		if (m_rot.y < -D3DX_PI)
		{
			// �C��
			m_rot.y += D3DX_PI * 2;
		}
		// 3.14���傫��������
		else if (m_rot.y > D3DX_PI)
		{
			// �C��
			m_rot.y -= D3DX_PI * 2;
		}

		pos += m_move;

		m_move.x += (0 - m_move.x) * 0.1f;
		m_move.z += (0 - m_move.z) * 0.1f;


		SetRotation(m_rot);
		SetvecAxis(D3DXVECTOR3(m_vecAxis.x, m_vecAxis.y, m_vecAxis.z));
		SetfValueRot(m_fValueRot);
		CSceneX::Update();
	}

	//{// �J�����ݒ�
	//	// �^�C���̉�]�̔��������Ɗi�[
	//	cameraVec.y = m_cameraRot.y - (m_rot.y + (m_dest.y * 0.85f));

	//	// ��]�̕␳
	//	CTakaseiLibrary::RotRevision(&cameraVec);

	//	// �J���������X�ɉ�]�����Ă���
	//	m_cameraRot.y -= cameraVec.y * CAMERA_ROT_SPEED;

	//	// ��]�̕␳
	//	CTakaseiLibrary::RotRevision(&m_cameraRot);

	//	// �J������]�ݒ�
	//	SetCameraRot(m_cameraRot);
	//}

	// �ړ����Ă����Ƃ�
	if (m_bMove)
	{
		// ��]�̕␳
		CTakaseiLibrary::RotRevision(&m_dest);

		// �v���C���[�����X�ɉ�]�����Ă���
		m_rot.y += m_dest.y * ROT_AMOUNT;
	}

	// �A�N�Z���{�^���ƃu���[�L�{�^���𗣂����Ƃ�
	if (!pGamepad->GetControllerPress(0, JOYPADKEY_A) && !pGamepad->GetControllerPress(0, JOYPADKEY_B) &&
		!pKeyboard->GetPressKeyboard(MOVE_ACCEL) && !pKeyboard->GetPressKeyboard(MOVE_BRAKE))
	{
		// �ړ��s��
		m_bMove = false;
	}

#ifdef _DEBUG
	if (pKeyboard->GetTriggerKeyboard(DIK_K))
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}
	{// K�������ꂽ�Ƃ�
		m_fDeathblow = 50.0f;				// �K�E�Z�|�C���g���ő�l�܂ŏグ��
	}

	if (pKeyboard->GetTriggerKeyboard(DIK_5))
	{
		CEffect::SandSmoke(pos);
		//CEffect::PetalCluster(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		//CEffect::Star(pos);
	}

#endif
}

//=============================================================================
// �L�[�{�[�h���͏���
//=============================================================================
void CPlayer::InputKeyboard(float fTireRotSpeed, D3DXVECTOR3 aVec)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

	//�㉺����
	if (pKeyboard->GetPressKeyboard(MOVE_ACCEL))
	{
		// �O�փ��f���̍ŏI�ړI���W
		m_dest.y = 0.0f;

		// ���x�ݒ�
		m_fSpeed = -m_fPuzzleMaxSPeed;

		// �^�C����]�����ݒ�
		fTireRotSpeed = TIRE_ROT_SPEED;

		// �����Ă���
		m_bMove = true;

		// �A�N�Z���{�^����������
		m_bAccel = true;
	}
	else if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
	{
		// �O�փ��f���̍ŏI�ړI���W
		m_dest.y = 0.0f;

		// ���x�ݒ�
		m_fSpeed = m_fPuzzleMaxSPeed;

		// �^�C����]�����ݒ�
		fTireRotSpeed = -TIRE_ROT_SPEED;

		// �����Ă���
		m_bMove = true;
	}

	// �A�N�Z���{�^���𗣂����Ƃ�
	if (!pKeyboard->GetPressKeyboard(MOVE_ACCEL))
	{
		// �A�N�Z���{�^���𗣂���
		m_bAccel = false;
	}

	// �h���t�g�{�^���������Ă��Ȃ��Ƃ�
	if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
	{
		//���E����
		if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = -CManager::GetTurnVelocity();
		}
		else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = CManager::GetTurnVelocity();
		}

		// �u���[�L�{�^���������ꂽ�Ƃ�
		if (pKeyboard->GetPressKeyboard(MOVE_BRAKE))
		{
			//���E����
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// �O�փ��f���̍ŏI�ړI���W
				m_dest.y = CManager::GetTurnVelocity();
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// �O�փ��f���̍ŏI�ړI���W
				m_dest.y = -CManager::GetTurnVelocity();
			}
		}
	}

	// �A�N�Z����Ԃ̂Ƃ�
	if (m_bAccel)
	{
		// �h���t�g���Ă��Ȃ��Ƃ�
		if (!m_bDrift[DRIFT_RIGHT] && !m_bDrift[DRIFT_LEFT])
		{
			// �h���t�g�{�^�����������Ƃ�
			if (pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				//���E����
				if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
				{
					// �h���t�g���Ă����Ԃɂ���
					m_bDrift[DRIFT_LEFT] = true;
				}
				else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
				{
					// �h���t�g���Ă����Ԃɂ���
					m_bDrift[DRIFT_RIGHT] = true;
				}
			}
		}

		// �E�h���t�g���Ă���Ƃ�
		if (m_bDrift[DRIFT_RIGHT])
		{
			// �O�փ��f���̍ŏI�ړI�n���W
			m_dest.y = DRIFT_DEST;

			//���E����
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = 0.0f;

				// �����x
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = ROT_SPEED_DRIFT;

				// �����x
				m_fAcceleration += ACCEKERATION_ADDITION;
			}

			// �����x�x�N�g���ݒ�
			aVec.x = sinf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;

			// �h���t�g�{�^���𗣂����Ƃ�
			if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				// �h���t�g�ő�܂ŃJ�E���g
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// �h���t�g���Ă��Ȃ���Ԃɂ���
					m_bDrift[nCnt] = false;

					// �����x������
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
		else if (m_bDrift[DRIFT_LEFT])
		{// ���h���t�g�̂Ƃ�
			// �O�փ��f���̍ŏI�ړI�n���W
			m_dest.y = -DRIFT_DEST;

			//���E����
			if (pKeyboard->GetPressKeyboard(MOVE_LEFT))
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = -ROT_SPEED_DRIFT;

				// �����x
				m_fAcceleration += ACCEKERATION_ADDITION;
			}
			else if (pKeyboard->GetPressKeyboard(MOVE_RIGHT))
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = 0.0f;

				// �����x
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}

			// �����x�x�N�g���ݒ�
			aVec.x = sinf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;

			// �h���t�g�{�^���𗣂����Ƃ�
			if (!pKeyboard->GetPressKeyboard(MOVE_DRIFT))
			{
				// �h���t�g�ő�܂ŃJ�E���g
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// �h���t�g���Ă��Ȃ���Ԃɂ���
					m_bDrift[nCnt] = false;

					// �����x������
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
	}
}

//=============================================================================
// �Q�[���p�b�h���͏���
//=============================================================================
void CPlayer::InputGemepad(float nValueH, float nValueV, float fTireRotSpeed, D3DXVECTOR3 aVec)
{
	// �Q�[���p�b�h�̎擾
	CInputController *pGamepad = CManager::GetInputController();

	//�㉺����
	if (pGamepad->GetControllerPress(0, JOYPADKEY_A))
	{
		// �O�փ��f���̍ŏI�ړI���W
		m_dest.y = 0.0f;

		// ���x�ݒ�
		m_fSpeed = -m_fPuzzleMaxSPeed;

		// �^�C����]�����ݒ�
		fTireRotSpeed = TIRE_ROT_SPEED;

		// �����Ă���
		m_bMove = true;

		// �A�N�Z���{�^����������
		m_bAccel = true;
	}
	else if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
	{
		// �O�փ��f���̍ŏI�ړI���W
		m_dest.y = 0.0f;

		// ���x�ݒ�
		m_fSpeed = m_fPuzzleMaxSPeed;

		// �^�C����]�����ݒ�
		fTireRotSpeed = -TIRE_ROT_SPEED;

		// �����Ă���
		m_bMove = true;
	}

	// �A�N�Z���{�^���𗣂����Ƃ�
	if (!pGamepad->GetControllerPress(0, JOYPADKEY_A))
	{
		// �A�N�Z���{�^���𗣂���
		m_bAccel = false;
	}

	// �h���t�g�{�^���������Ă��Ȃ��Ƃ�
	if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
	{
		// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
		if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
		{
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = -CManager::GetTurnVelocity();
		}
		else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
		{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
			// �O�փ��f���̍ŏI�ړI���W
			m_dest.y = CManager::GetTurnVelocity();
		}

		// �u���[�L�{�^���������ꂽ�Ƃ�
		if (pGamepad->GetControllerPress(0, JOYPADKEY_B))
		{
			// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
			if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
			{
				// �O�փ��f���̍ŏI�ړI���W
				m_dest.y = CManager::GetTurnVelocity();
			}
			else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
			{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
			 // �O�փ��f���̍ŏI�ړI���W
				m_dest.y = -CManager::GetTurnVelocity();
			}
		}
	}

	// �A�N�Z����Ԃ̂Ƃ�
	if (m_bAccel)
	{
		// �h���t�g���Ă��Ȃ��Ƃ�
		if (!m_bDrift[DRIFT_RIGHT] && !m_bDrift[DRIFT_LEFT])
		{
			// �h���t�g�{�^�����������Ƃ�
			if (pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
			{
				// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
				if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
				{
					// �h���t�g���Ă����Ԃɂ���
					m_bDrift[DRIFT_LEFT] = true;
				}
				else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
				{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
					// �h���t�g���Ă����Ԃɂ���
					m_bDrift[DRIFT_RIGHT] = true;
				}
			}
		}

		// �E�h���t�g���Ă���Ƃ�
		if (m_bDrift[DRIFT_RIGHT])
		{
			// �O�փ��f���̍ŏI�ړI�n���W
			m_dest.y = DRIFT_DEST;

			// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
			if (nValueH <= JOY_MAX_RANGE && nValueH > 0)
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = 0.0f;

				// �����x
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}
			else if (nValueH >= -JOY_MAX_RANGE && nValueH < 0)
			{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = ROT_SPEED_DRIFT;

				// �����x
				m_fAcceleration += ACCEKERATION_ADDITION;
			}

			// �����x�x�N�g���ݒ�
			aVec.x = sinf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y + D3DX_PI / 2) * m_fAcceleration;

			// �h���t�g�{�^���𗣂����Ƃ�
			if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
			{
				// �h���t�g�ő�܂ŃJ�E���g
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// �h���t�g���Ă��Ȃ���Ԃɂ���
					m_bDrift[nCnt] = false;

					// �����x������
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
		else if (m_bDrift[DRIFT_LEFT])
		{// ���h���t�g�̂Ƃ�
			// �O�փ��f���̍ŏI�ړI�n���W
			m_dest.y = -DRIFT_DEST;

			// ���ɃX�e�B�b�N���|�ꂽ�Ƃ�
			if (nValueV <= JOY_MAX_RANGE && nValueV > 0)
			{
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = -ROT_SPEED_DRIFT;

				// �����x
				m_fAcceleration += ACCEKERATION_ADDITION;
			}
			else if (nValueV >= -JOY_MAX_RANGE && nValueV < 0)
			{// �E�ɃX�e�B�b�N���|�ꂽ�Ƃ�
				// �O�փ��f���̍ŏI�ړI�n���W
				m_dest.y = 0.0f;

				// �����x
				m_fAcceleration -= ACCEKERATION_ADDITION;
			}

			// �����x�x�N�g���ݒ�
			aVec.x = sinf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;
			aVec.z = cosf(m_rot.y + m_dest.y - D3DX_PI / 2) * m_fAcceleration;

			// �h���t�g�{�^���𗣂����Ƃ�
			if (!pGamepad->GetControllerPress(0, JOYPADKEY_RIGHT_TRIGGER))
			{
				// �h���t�g�ő�܂ŃJ�E���g
				for (int nCnt = 0; nCnt < DRIFT_MAX; nCnt++)
				{
					// �h���t�g���Ă��Ȃ���Ԃɂ���
					m_bDrift[nCnt] = false;

					// �����x������
					m_fAcceleration = ACCEKERATION;
				}
			}
		}
	}
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void CPlayer::Debug(void)
{
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 posOld = GetPosOld();

	ImGui::Begin("System");													// �uSystem�v�E�B���h�E�ɒǉ����܂��B�Ȃ���΍쐬���܂��B

	if (ImGui::CollapsingHeader("player"))
	{
		ImGui::Text("pos = %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("posOld = %.2f, %.2f, %.2f", posOld.x, posOld.y, posOld.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("rot = %.2f, %.2f, %.2f", m_rot.x, m_rot.y, m_rot.z);								// �v���C���[�̉�]��\��
		ImGui::Text("move = %.2f, %.2f, %.2f", m_move.x, m_move.y, m_move.z);								// �v���C���[�̌��݈ʒu��\��
		ImGui::Text("HP = %d", m_nLife);				// �v���C���[�̗̑͂�\��

		D3DXVECTOR3 size = GetSize();
		ImGui::DragFloat3(u8"�傫��", (float*)size);
		SetSize(size);

		ImGui::Checkbox("ColliderWithWall", &m_bColliderWithWall);

		if (ImGui::Button("BOSS"))
		{
			pos = D3DXVECTOR3(5972.14f, 100.0f, 8000.62f);
			SetPosition(pos);
		}
	}

	//�f�o�b�O�������I��
	ImGui::End();
}
#endif
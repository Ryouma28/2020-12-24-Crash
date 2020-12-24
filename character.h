//=============================================================================
//
// �L�����N�^�[���� [character.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//=============================================================================
// �C���N���[�h
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_KEY 30

//=============================================================================
// �\���̒�`
//=============================================================================
// �L�[�v�f
typedef struct
{
	D3DXVECTOR3		pos;																			// �ʒu
	D3DXVECTOR3		rot;																			// ��]
} KEY;

// �L�[���
typedef struct
{
	KEY				aKey[MAX_KEY];																	// �p�[�c�̃L�[���
	int				nFrame;																			// �t���[����
} KEY_INFO;

// �A�j���[�V����
typedef struct
{
	int				nLoop;																			// ���[�v���邩�ǂ���
	int				nMaxKey;																		// �L�[��
	int				nSwitchFrame;																	// �؂�ւ���܂ł̃t���[����
	KEY_INFO		*apKeyInfo;																		// �L�[�̍\����
} ANIMATION;

//=============================================================================
// �O���錾
//=============================================================================
class CMeshCapsule;
class CMeshCube;
class CMeshOrbit;
class CColliderSphere;
class CColliderBox;
class CScene2D;
class CModel;
class CPlayerUi;

//=============================================================================
// �N���X��`
//=============================================================================
class CCharacter : public CScene
{
public:
	CCharacter(PRIORITY obj);						// �v���C���[�̃R���X�g���N�^(�I�u�W�F�N�g�^�C�v)
	~CCharacter();									// �v���C���[�̃f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �J������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static HRESULT Load(void);					// �f�ރf�[�^�̎擾

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col);
	void ShowInspector(void) {};

	virtual void BehaviorForMaxFrame(void) = 0;		// �ő�t���[�����ɓ��B�����Ƃ��̏���
	virtual void BehaviorForMaxKey(void) = 0;		// �ő�L�[���ɓ��B�����Ƃ��̏���

	D3DXVECTOR3 GetMove(void) { return m_move; }				// �ړ��ʂ̎擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }				// ��]�̎擾
	CModel *GetModel(void) { return m_pModel; }					// �p�[�c�̐擪�A�h���X�擾
	ANIMATION *GetAnimData(void) { return m_pAnimation; }		// �e�A�j���[�V�����f�[�^�̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }			// ���[���h�}�g���b�N�X�̎擾
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }		// ���[���h�}�g���b�N�X�̐ݒ�
	void LoadScript(std::string add, const int nMaxAnim);		// �X�N���v�g�ǂݍ���
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }			// ��]�̐ݒ�
	int GetnNumParts(void) { return m_nNumParts; }				// �p�[�c�̐��擾

	/*================= �A�j���[�V�����֘A =================*/
	// �ݒ�
	void AnimationSwitch(int nType);								// �A�j���[�V�����̐؂�ւ�����
	void SetAnimPlayState(bool bValue) { m_bAnimation = bValue; }	// �A�j���[�V�����Đ���~�̐ݒ�
	void SetCurrentKey(int nValue) { m_nCurrentKey = nValue; }		// ���݂̃L�[����ݒ�
	void SetCurrentFrame(int nValue) { m_nCurrentFrame = nValue; }	// ���݂̃t���[������ݒ�
	void ResetKeyAndFrame(void);									// �L�[���ƃt���[�����̃��Z�b�g

	// �擾
	int GetAnimType(void) { return m_nAnimationType; }			// �A�j���[�V�����^�C�v���擾
	int GetCurrentKey(void) { return m_nCurrentKey; }			// ���݂̃L�[�����擾
	int GetCurrentFrame(void) { return m_nCurrentFrame; }		// ���݂̃t���[�������擾
	bool GetAnimPlayState(void) { return m_bAnimation; }		// �A�j���[�V�����Đ���~�̎擾

private:
#ifdef _DEBUG
	void Debug(void);																// �f�o�b�O�����֐�
#endif

	void Animation(void);															// �A�j���[�V�����֘A�֐�

	/*================= �v���C���[�֘A =================*/
	D3DXVECTOR3						m_dest;											// ���f���̍ŏI���B�_
	D3DXVECTOR3						m_difference;									// ���f���̍ő��]
	D3DXVECTOR3						m_move;											// �ړ���
	D3DXVECTOR3						m_size;											// �傫��
	D3DXVECTOR3						m_rot;											// ��]��
	D3DXCOLOR						m_color;										// �F
	int								m_nActionCount;									// ���̃A�N�V�����܂ł̃J�E���^

	/*=============== 3D�����_�����O�֘A ===============*/
	LPDIRECT3DVERTEXBUFFER9			m_pVtxBuff;										// ���_�o�b�t�@�ւ̃|�C���^
	LPD3DXMESH						m_pMesh;										// ���b�V�����ւ̃|�C���^
	DWORD							m_nNumMat;										// �}�e���A�����̐�
	LPD3DXBUFFER					m_pBuffMat;										// �}�e���A�����ւ̃|�C���^
	CModel							*m_pModel;										// �p�[�c���f���̃|�C���^
	D3DXMATRIX						m_mtxWorld;										// ���[���h�}�g���b�N�X

	/*=============== �A�j���[�V�����֘A ===============*/
	int								m_nNumKey;										// �L�[�̑���
	int								m_nNumParts;									// �p�[�c�̑���
	int								m_nCurrentKey;									// ���݂̃L�[No.
	int								m_nCurrentFrame;								// ���݂̃t���[��No.
	int								m_nCntMotion;									// �J�E���^�[���[�V����
	int								m_nMaxAnimationType;							// �A�j���[�V�����^�C�v�̍ő吔
	ANIMATION						*m_pAnimation;									// �L�[���|�C���^
	int								m_nAnimationType;								// ���݂̃A�j���[�V�����^�C�v
	int								m_nAnimationTypeOld;							// �O�̃A�j���[�V�����^�C�v

	D3DXVECTOR3						*m_apCurrentPos;								// 1�t���[��������̈ړ���
	D3DXVECTOR3						*m_apCurrentRot;								// 1�t���[��������̉�]��

	bool							m_bAnimSwitch;									// �A�j���[�V�����̐؂�ւ��p�t���[�����g�p���邩
	bool							m_bAnimation;									// �A�j���[�V�����̍Đ�
};
#endif
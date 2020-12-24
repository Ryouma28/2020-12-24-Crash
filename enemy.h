//=============================================================================
//
// �G���� [enemy.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "character.h"

//=============================================================================
// �񋓑̒�`
//=============================================================================
// �^�[�Q�b�g�^�C�v
typedef enum
{
	TARGETTYPE_NONE = -1,
	TARGETTYPE_PLAYER = 0,
	TARGETTYPE_HOUSE,
	TARGETTYPE_MAX
} TARGETTYPE;

//=============================================================================
// �O���錾
//=============================================================================
class CColliderSphere;
class CColliderBox;
class CCollider;
class CLife;

//=============================================================================
// �N���X��`
//=============================================================================
class CEnemy : public CCharacter
{
public:
	//���[�V�����^�C�v
	typedef enum
	{
		ANIMATIONTYPE_NONE = -1,
		ANIMATIONTYPE_NEUTRAL = 0,
		ANIMATIONTYPE_RUN,
		ANIMATIONTYPE_ATTACK,
		ANIMATIONTYPE_DAMAGE,
		ANIMATIONTYPE_DIE,
		ANIMATIONTYPE_MAX
	} ANIMATIONTYPE;

	CEnemy(PRIORITY obj);			// �R���X�g���N�^
	~CEnemy();						// �f�X�g���N�^
	HRESULT Init(void);				// ����������
	void Uninit(void);				// �J������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CEnemy *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);		// ���[�h����
	static void LoadEnemy(char *add);		// �G�z�u���ǂݍ��ݏ���
	void SetTarget(TARGETTYPE target);		// �^�[�Q�b�g�̐ݒ�

	void OnTriggerEnter(CCollider *col);
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

	void BehaviorForMaxFrame(void);				// �ő�t���[�����ɓ��B�����Ƃ��̏���
	void BehaviorForMaxKey(void);				// �ő�L�[���ɓ��B�����Ƃ��̏���

	static int GetEnemyKill(void) { return m_nKill; }					// �|�������̎擾
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }					// �}�g���b�N�X�̎擾
	CColliderSphere *GetSphere(void) { return m_pSphere; }				// �X�t�B�A�R���C�_�[�̎擾
	TARGETTYPE GetTarget(void) { return m_target; }						// �^�[�Q�b�g�̎擾

private:
#ifdef _DEBUG
	void Debug(void);													// �f�o�b�O����
#endif
	void Move(D3DXVECTOR3 &pPos);										// �ړ�����
	void Collider(void);												// �����蔻��}�l�[�W��

	static LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`���ւ̃|�C���^
	D3DXMATRIX						m_mtxWorld;							// ���[���h�}�g���b�N�X

	/*=============== �����蔻�� ===============*/
	CColliderSphere					*m_pSphere;							// �����蔻��(��)�̃|�C���^

	/*=============== �X�e�[�^�X ===============*/
	static int						m_nKill;							// �|������
	D3DXVECTOR3						m_rot;								// ��]
	D3DXVECTOR3						m_move;								// �ړ���
	D3DXVECTOR3						m_dest;								// �ړ���
	D3DXVECTOR3						m_difference;						// ��
	int								m_nLife;							// �̗�
	int								m_nInterval;						// �x�e����
	bool							m_bHit;								// �����蔻��
	bool							m_bJump;							// �W�����v
	CLife							*m_pLife;							// �̗̓Q�[�W
	TARGETTYPE						m_target;							// �U���Ώ�
};
#endif
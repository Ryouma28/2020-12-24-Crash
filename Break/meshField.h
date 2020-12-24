//=============================================================================
//
// ���b�V���t�B�[���h���� [meshField.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEPTH_FIELD	50									// ���s��
#define WIDE_FIELD 50									// ��
#define SIZE		50.0f								// �T�C�Y

//=============================================================================
// �\���̒�`
//=============================================================================
//�n�ʂ̎��
typedef enum
{
	RANDTYPE_NONE = -1,
	RANDTYPE_GRASS = 0,
	RANDTYPE_SAND,
	RANDTYPE_MAX
} RANDTYPE;

//=============================================================================
// �O���錾
//=============================================================================
class CCircle;
class CWrite;

//=============================================================================
// �N���X��`
//=============================================================================
class CMeshField : public CScene
{
public:
	CMeshField(PRIORITY obj);														// �R���X�g���N�^
	~CMeshField();																	// �f�X�g���N�^
	HRESULT Init(void);																// ����������
	void Uninit(void);																// �J������
	void Update(void);																// �X�V����
	void Draw(void);																// �`�揈��

	static CMeshField *Create(void);												// �N���G�C�g����
	static HRESULT Load(void);														// ���[�h����

	static void LoadRand(char *add, bool bDebug);									// �n�ʂ̍����ǂݍ���
	void SetTexType(int nValue, RANDTYPE Type);										// �n�ʂ̎�ނ�ύX
	float GetHeight(D3DXVECTOR3 pos);												// �|���S���̍��������߂�

	static bool SphereModel(D3DXVECTOR3 C1, D3DXVECTOR3 C2, float R1);				// X����Z���̓����蔻��
	bool CollisionRange(D3DXVECTOR3 pos);											// �����͈͓̔��ɂ��邩�ǂ���
	bool CollisionRangeToCircle(D3DXVECTOR3 pos, float fRadius);					// �~�������͈͓̔��ɂ��邩�ǂ���
	bool CollisionCircle(D3DXVECTOR3 pos, float fRadius);							// �~�Ɠ_�̓����蔻��
	void Reset(void);																// �n�`�̍��჊�Z�b�g
	void CalculationNormalize(void);												// �@���̌v�Z

	RANDTYPE GetTexType(int nValue) { return m_randType[nValue]; }					// �n�ʂ̃^�C�v���擾
	RANDTYPE GetRandType(void) { return	 m_type; }									// �n�ʂ̃^�C�v���擾
	void SetColor(const D3DXCOLOR &col) { m_col = col; }							// �n�ʂ̐F��ݒ�

	void SetDebugRand(const bool &bValue) { m_bDebug = bValue; }					// �f�o�b�O�p�t���O�̐ݒ�
	bool GetDebugRand(void) { return m_bDebug; }									// �f�o�b�O�p�t���O�̎擾

	void SetCalculation(const bool &bValue) { m_bCalculation = bValue; }			// �N���v�Z�t���O�̐ݒ�
	const bool &GetCalculation(void) { return m_bCalculation; }						// �N���v�Z�t���O�̎擾

	static void CreateRand(const int &nWide, const int &nDepth, const D3DXVECTOR3 &createPos);

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void);

	void MakeVertex(FILE *pFile, bool bDebug);										// ���_���̍쐬

	void Debug(void);
	void SaveRand(CWrite *pWrite);														// �n�ʂ̍�����������
	void CreateRand(D3DXVECTOR3 &worldPos, float fRadius);							// �n�`�ҏW�֐�
	void Paint(D3DXVECTOR3 &worldPos, float fRadius);								// �y�C���g

private:
	void LoadTree(void);															// �ؔz�u�̏�������

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;												// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	void MakeIndex(void);															// ���_�C���f�b�N�X�̍쐬

	D3DXVECTOR3 apNor[DEPTH_FIELD * WIDE_FIELD * 2];								// 1�ʂ��Ƃ̖@���x�N�g���̔z��
	RANDTYPE	m_randType[DEPTH_FIELD * WIDE_FIELD * 2];							// ���̎��

	D3DXVECTOR3				m_rot;													// �|���S���̌���(��])
	D3DXCOLOR				m_col;													// ���_�J���[
	D3DXMATRIX				m_mtxWorld;												// ���[���h�}�g���b�N�X

	int m_nNumVertex;																// �����_��
	int m_nNumIndex;																// �C���f�b�N�X��
	int m_nNumPolygon;																// ���|���S����
	bool m_bDebug;																	// �f�o�b�O�Ƃ��Ẵt���O
	bool m_bCalculation;															// �N���v�Z�t���O

	RANDTYPE m_type;																// ���̃^�C�v
};
#endif
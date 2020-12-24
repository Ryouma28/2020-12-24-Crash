//=============================================================================
//
// �J�E���^���� [counter.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _COUNTER_H_
#define _COUNTER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;

//=============================================================================
// �N���X��`
//=============================================================================
class CCounter : public CScene
{
public:
	CCounter(PRIORITY obj);					// �R���X�g���N�^
	~CCounter();							// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static CCounter *Create(int nLength);			// �N���G�C�g����

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

	void SetTransform(void);
	void BindTexture(std::string Add);
	void SetNumber(int nValue);	// ���l�̐ݒ�
	void SetSize(D3DXVECTOR3 size);		// �T�C�Y�̐ݒ�

	void SetDistance(D3DXVECTOR3 &distance);			// �����̐ݒ�
	void SetIntervalNum(D3DXVECTOR3 &interval);			// �����ǂ����̋���

	D3DXVECTOR3 GetSize(void) { return m_size; }

private:
	D3DXVECTOR3 m_size;				// �T�C�Y
	D3DXVECTOR3 m_distance;			// �I�t�Z�b�g
	D3DXVECTOR3 m_IntervalNum;		// �����ǂ����̋���
	unsigned int m_nLength;

	std::vector<CNumber*> m_vNumber;		// ����^�C�v
};
#endif
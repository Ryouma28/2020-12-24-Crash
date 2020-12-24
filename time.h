//=============================================================================
//
// ���ԏ��� [time.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_POINTER 2								//���v�̐j�̐�

//=============================================================================
// �N���X��`
//=============================================================================
class CTime : public CScene
{
public:
	typedef enum
	{
		OPERATIONTYPE_TIMER = 0,
		OPERATIONTYPE_STOPWATCH,
		OPERATIONTYPE_MAX
	} OPERATIONTYPE;

	CTime(PRIORITY obj);							// �R���X�g���N�^
	~CTime();										// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	static CTime *Create(void);						// �N���G�C�g����

	void SetTime(int nTime);	// ���Ԑݒ�
	void SetSeconds(int nTime);	// ���Ԑݒ�
	void SetOpType(OPERATIONTYPE type);	// �^�C�}�[�̓�����@�ݒ�
	void SetUpdateTimer(bool bValue);		// �^�C�}�[�̓���t���O

	int GetMinutes(void) { return m_nMinutes; }		// �����̎擾
	int GetSeconds(void) { return m_nSeconds; }		// �b���̎擾
	bool GetUpdateTimer(void) { return m_bStart; }	// �^�C�}�[�̓���󋵎擾

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	OPERATIONTYPE m_type;

	int m_nMinutes;				// ��
	int m_nSeconds;				// �b
	int m_nMaxSeconds;			// �b���̍ő�l
	DWORD m_dTimeFrame;			// �t���[����
	bool m_bStart;				// �^�C�}�[�̓���
};
#endif
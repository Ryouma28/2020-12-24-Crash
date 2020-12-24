//=============================================================================
//
// �X�^�[�g�V�O�i������ [startSignal.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _STARTSIGNAL_H_
#define _STARTSIGNAL_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �O���錾
//=============================================================================
class CTime;

//=============================================================================
// �N���X��`
//=============================================================================
class CStartSignal : public CScene
{
public:
	CStartSignal(PRIORITY obj);					// �R���X�g���N�^
	~CStartSignal();							// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static CStartSignal *Create(void);			// �N���G�C�g����

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	CTime *m_pTime;
	CUi *m_pUi;

	bool m_bEnd;
};
#endif
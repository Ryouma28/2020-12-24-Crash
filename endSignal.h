//=============================================================================
//
// �G���h�V�O�i������ [endSignal.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _ENDSIGNAL_H_
#define _ENDSIGNAL_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene2D.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CEndSignal : public CScene
{
public:
	CEndSignal(PRIORITY obj);					// �R���X�g���N�^
	~CEndSignal();							// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static CEndSignal *Create(void);			// �N���G�C�g����

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	CUi *m_pUi;
	bool m_bEnd;
};
#endif
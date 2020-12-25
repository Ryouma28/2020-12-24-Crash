//=============================================================================
//
// fever���� [fever.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _FEVER_H_
#define _FEVER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CTime;
class CScene2D;

//=============================================================================
// �N���X��`
//=============================================================================
class CFever : public CScene
{
public:
	CFever(PRIORITY obj);					// �R���X�g���N�^
	~CFever();							// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �J������
	void Update(void);						// �X�V����
	void Draw(void);						// �`�揈��

	static CFever *Create(void);			// �N���G�C�g����

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	CTime *m_pTime;
	CUi *m_pUi;

	CScene2D *m_pScene2D;

	bool m_bEnd;
};
#endif
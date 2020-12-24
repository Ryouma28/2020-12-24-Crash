//=============================================================================
//
// �ē�?���� [GuideSign.h]
// Author : Seiya Takahashi
//
//=============================================================================
#ifndef _GUIDESIGN_H_
#define _GUIDESIGN_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "sceneX.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CGuideSign : public CSceneX
{
public:
	CGuideSign(PRIORITY obj);						// �R���X�g���N�^
	~CGuideSign();									// �f�X�g���N�^
	HRESULT Init(void);								// ����������
	void Uninit(void);								// �J������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	static CGuideSign *Create(void);				// �N���G�C�g����
	static HRESULT Load(void);						// ���[�h����

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
#ifdef _DEBUG
	void Debug(void);								// �f�o�b�O����
#endif
};
#endif
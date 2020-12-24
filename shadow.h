//==================================================================================================================
//
// �e�|���S��[shadow.h]
// Author:Seiya Takahashi
//
//==================================================================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define SHADOW_TEX "data/tex/effect/sphere.jpg"				// �ǂݍ��ރe�N�X�`���̃\�[�X��

//==================================================================================================================
//
// �e�N���X�̒�`
//
//==================================================================================================================
class CShadow : public CScene
{
public:

	CShadow(PRIORITY type);						// �R���X�g���N�^
	~CShadow();									// �f�X�g���N�^
	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	static CShadow *Create(void);				// ��������
	static HRESULT Load(void);					// �e�N�X�`����񃍁[�h

	inline D3DXVECTOR3 &GetPos(void) { return m_pos; }// �ʒu�擾
	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// �o�b�t�@
	VERTEX_3D *m_pVtx;							// ���_�i�[
	D3DXMATRIX m_mtxWorld;						// ���[���h�}�g���b�N�X

	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_size;							// �傫��
	D3DXVECTOR3 m_rot;							// ��]
};
#endif
//=============================================================================
//
// ���[�U�[�C���^�[�t�F�[�X���� [ui.h]
// Author : masayasu wakita
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"
#include "scene.h"

//=============================================================================
// �O���錾
//=============================================================================
class CScene2D;
class CCounter;

//=============================================================================
// �N���X��`
//=============================================================================
class CUi : public CScene
{
public:
	typedef enum
	{
		OPERATIONTYPE_NONE = -1,
		OPERATIONTYPE_MOVE,
		OPERATIONTYPE_DELETE,
		OPERATIONTYPE_MAX
	} OPERATIONTYPE;

	CUi(PRIORITY obj);			// �R���X�g���N�^
	~CUi();						// �f�X�g���N�^
	HRESULT Init(void);			// ����������
	void Uninit(void);			// �J������
	void Update(void);			// �X�V����
	void Draw(void);			// �`�揈��

	static CUi *Create(void);	// �N���G�C�g����
	static HRESULT Load(void);	// ���[�h����
	bool LoadScript(const std::string &add);		// �X�N���v�g�f�[�^���[�h����

	void CreateTexture(std::string Add);
	void DeleteTexture(int nIndex);
	void SceneDebug(void);
	void SaveScript(std::string Add);

	void SetPosition(D3DXVECTOR3 pos);
	void SetColor(D3DXCOLOR col);
	void SetNextOperation(void);

	CCounter *GetCounter(std::string Tag);
	OPERATIONTYPE GetOpType(void) { return m_vOpType[m_nOpTypeNow]; }

	void OnTriggerEnter(CCollider *col) {};
	void OnCollisionEnter(CCollider *col) {};
	void ShowInspector(void) {};

private:
	void PointMove(void);
	void Delete(void);

	typedef struct
	{
		D3DXVECTOR3 pos;			// �ʒu
		D3DXCOLOR col;				// �F
		int nFrame;					// ���t���[�������ď������邩
	} MOVEPOINT;

	D3DXCOLOR m_col;
	D3DXVECTOR3 m_currentPos;					// 1�t���[��������̈ړ���
	D3DXCOLOR m_currentCol;						// 1�t���[��������̐F
	int m_nCurrentFrame;						// ���݂̃t���[����
	int m_nCurrentPoint;						// ���݂̃|�C���g

	unsigned int m_nOpTypeNow;					// ���݂̓���^�C�v
	std::vector<OPERATIONTYPE> m_vOpType;		// ����^�C�v
	std::vector<MOVEPOINT> m_vPoint;			// �ړ���
	std::vector<CScene2D*> m_vAsset;			// �A�Z�b�g
	std::map<std::string, CCounter*> m_CntMap;	// �J�E���^�}�b�v
};
#endif
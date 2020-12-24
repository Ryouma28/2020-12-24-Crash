//=============================================================================
//
// �������� [number.cpp]
// Author : masayasu wakita
//
//=============================================================================
#include "counter.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"

// ==========================================================
// �ÓI�����o�[�ϐ��̏�����
// ==========================================================
int CCounter::m_nNum = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCounter::CCounter(CScene::PRIORITY obj = CScene::PRIORITY_UI) : CScene(obj)
{
	m_vNumber = {};

	m_distance = D3DXVECTOR3_ZERO;
	m_IntervalNum = D3DXVECTOR3_ZERO;
	m_bVariableNum = false;
	m_nLength = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCounter::~CCounter()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCounter::Init(void)
{
	return S_OK;
}

//=============================================================================
// �J������
//=============================================================================
void CCounter::Uninit(void)
{
	for (unsigned int nCount = 0; nCount < m_vNumber.size(); nCount++)
	{
		if (m_vNumber[nCount] != NULL)
		{
			m_vNumber[nCount]->Uninit();
			m_vNumber[nCount]->Release();
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CCounter::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCounter::Draw(void)
{
}

//=============================================================================
// �N���G�C�g����
//=============================================================================
CCounter *CCounter::Create(int nLength)
{
	CCounter *pCounter;
	pCounter = new CCounter(PRIORITY_UI);

	if (pCounter != NULL)
	{// ���������݂��Ă����Ƃ�
		pCounter->m_nLength = nLength;		// �ő包��

		// ��������������
		for (int nCount = 0; nCount < nLength; nCount++)
		{
			CNumber *pNumber = CNumber::Create();		// �����̍쐬

			if (pNumber != NULL)
			{// ���������݂��Ă����Ƃ�
				pNumber->SetNumber(0);
				pCounter->m_vNumber.push_back(pNumber);
			}
		}

		pCounter->Init();					// ����������
	}
	return pCounter;
}

//=============================================================================
// �����̐ݒ�
//=============================================================================
void CCounter::SetDistance(D3DXVECTOR3 &distance)
{
	m_distance = distance;
	SetTransform();
}

//=============================================================================
// �����ǂ����̋���
//=============================================================================
void CCounter::SetIntervalNum(D3DXVECTOR3 & interval)
{
	m_IntervalNum = interval;
	SetTransform();
}

//=============================================================================
// �ʒu�֌W�̍X�V
//=============================================================================
void CCounter::SetTransform(void)
{
	D3DXVECTOR3 pos = GetPosition();

	for (unsigned int nCount = 0; nCount < m_vNumber.size(); nCount++)
	{
		if (m_vNumber[nCount] != NULL)
		{
			m_vNumber[nCount]->SetPosition((m_distance + pos) + m_IntervalNum * (float)nCount);
			m_vNumber[nCount]->SetTransform();
		}
	}
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CCounter::BindTexture(std::string Add)
{
	for (unsigned int nCount = 0; nCount < m_vNumber.size(); nCount++)
	{
		if (m_vNumber[nCount] != NULL)
		{
			// �e�N�X�`���̐ݒ�
			m_vNumber[nCount]->BindTexture(Add);
		}
	}
}

//=============================================================================
// �^�C���̕ύX
//=============================================================================
void CCounter::SetNumber(int nValue)
{
	unsigned int nLength = CManager::LengthCalculation(nValue);

	// �ő包���܂ŃJ�E���g
	for (unsigned int nCount = 0; nCount < m_vNumber.size(); nCount++)
	{
		// �\�����鐔���v�Z
		m_nNum = nValue % (int)powf(10.0f, (float)m_vNumber.size() - nCount) / (int)powf(10.0f, m_vNumber.size() - 1.0f - nCount);

		// �����ݒ�
		m_vNumber[nCount]->SetNumber(m_nNum);

		if (m_bVariableNum)
		{
			if (m_vNumber.size() - nCount <= nLength)
			{// �ő包����菬�������������Ƃ�
				if (!m_vNumber[nCount]->GetActive())
				{
					m_vNumber[nCount]->SetActive(true);
				}
			}
			else
			{// �ő包�����傫�����������Ƃ�
				if (m_vNumber[nCount]->GetActive())
				{
					m_vNumber[nCount]->SetActive(false);
				}
			}
		}
	}
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CCounter::SetSize(D3DXVECTOR3 size)
{
	m_size = size;

	for (unsigned int nCount = 0; nCount < m_vNumber.size(); nCount++)
	{
		if (m_vNumber[nCount] != NULL)
		{
			m_vNumber[nCount]->SetSize(m_size);						// �傫���ݒ�
			m_vNumber[nCount]->SetTransform();
		}
	}
}
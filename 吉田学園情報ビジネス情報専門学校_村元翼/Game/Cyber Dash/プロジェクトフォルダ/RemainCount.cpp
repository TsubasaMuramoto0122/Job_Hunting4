//=============================================================================
//
// ���S�񐔃J�E���g�̃N���X [RemainCount.cpp]
// Author : ������
//
//============================================================================
#include "RemainCount.h"
#include "number.h"
#include "fade.h"
#include "Ui.h"
#include "StringManager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_DEATH (999)	// ���S�񐔌��E�l

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
int		CRemainCount::m_nRemain = 0;			// �c�@
bool	CRemainCount::m_bChange = false;		// �c�@��ω������邩

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRemainCount::CRemainCount(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(m_apNumber, 0, sizeof(m_apNumber));			// �����N���X
	m_pos = { 0.0f,0.0f,0.0f };							// �ʒu
	m_size = { 0.0f,0.0f,0.0f };						// �傫��
	m_col = { 0.0f,0.0f,0.0f,0.0f };					// �J���[
	m_bFade = false;									// �t�F�[�h���邩
	m_bChange = false;									// �J�E���g���鏈�����s����
	m_nFrame = 0;										// �t���[��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRemainCount::~CRemainCount()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRemainCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�E�傫���̐ݒ�E�擾
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// �c�@��100���͒����Ȃ�
	if (m_nRemain > MAX_DEATH)
	{
		m_nRemain = MAX_DEATH;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRemainCount::Uninit(void)
{
	// 2D�|���S���̔j��
	for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRemainCount::Update(void)
{
	// �c�@������
	if (m_bChange)
	{
		RemainChange();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRemainCount::Draw(void)
{
	// 2D�|���S���̕`��
	for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//=============================================================================
// ���Ԃ̐���
//=============================================================================
CRemainCount *CRemainCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// ���Ԃ̃C���X�^���X����
	CRemainCount *pRemainCount = new CRemainCount;

	// ���Ԃ̏���
	if (pRemainCount)
	{
		pRemainCount->Init(pos, size);			// ������
	}

	return pRemainCount;
}

//=============================================================================
// ���Ԃ̐���
//=============================================================================
void CRemainCount::RemainChange(void)
{
	// ���S�񐔕����\��
	if (m_nFrame == 0)
	{
		// �����񐶐�
		CStringManager::Create({ m_pos.x - (m_size.x * 1.5f), m_pos.y - (m_size.y / 2), 0.0f }, m_size.y, (int)m_size.y, "���S��", "HGP�n�p��ھ�ݽEB");

		// �ω��O�̎c�@
		for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
		{
			if (!m_apNumber[nCnt])
			{
				m_apNumber[nCnt] = CNumber::Create({ m_pos.x + (m_size.x * nCnt), m_pos.y, 0.0f }, { m_size.x, m_size.y, 0.0f });
				m_apNumber[nCnt]->SetNumber((m_nRemain) % (int)pow(10, MAX_REMAIN_DIGIT - nCnt) / (int)pow(10, (MAX_REMAIN_DIGIT - 1) - nCnt));
			}
		}
		m_nRemain++;	// ���S�񐔃J�E���g�𑝂₷
	}

	// ���S�񐔃J�E���g��\��
	else if (m_nFrame == COUNT_CHANGE_FRAME)
	{
		// �ω���̎c�@
		for (int nCnt = 0; nCnt < MAX_REMAIN_DIGIT; nCnt++)
		{
			if (m_apNumber[nCnt])
			{
				m_apNumber[nCnt]->SetNumber(m_nRemain % (int)pow(10, MAX_REMAIN_DIGIT - nCnt) / (int)pow(10, (MAX_REMAIN_DIGIT - 1) - nCnt));
			}
		}
	}

	// �Q�[�������Ȃ���
	else if (m_nFrame == COUNT_CHANGE_FRAME * 2)
	{
		// �t�F�[�h�̎擾
		CFade *pFade = CManager::GetInstance()->GetFade();

		CManager::GetInstance()->SetStop(true);
		m_bChange = false;

		pFade->SetFade(CManager::MODE::GAME);
	}

	m_nFrame++; // �t���[����i�߂�
}

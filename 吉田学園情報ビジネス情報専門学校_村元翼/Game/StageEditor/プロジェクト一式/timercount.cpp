//=============================================================================
// ���Ԍo�߂̏��� [timercount.cpp]
// Author : ������
//=============================================================================

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "timercount.h"
#include "number.h"
#include "fade.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTimerCount::CTimerCount(OBJTYPE nPriority) : CScene(nPriority)
{
	m_dwCurrentTime = 0;
	m_dwExecLastTime = timeGetTime();	// ���݂̎��Ԃ��擾
	m_bFade = false;					// �t�F�[�h����
	m_bLimit = false;					// ���Ԑ؂ꔻ��
	m_bStop = false;					// �X�g�b�v����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTimerCount::~CTimerCount()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTimerCount::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�E�T�C�Y�ݒ�
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// �����̐���
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		m_apNumber[nCntTime] = CNumber::Create(D3DXVECTOR3(m_pos.x + m_size.x + SPACE_TIME * nCntTime, m_pos.y, 0.0f), D3DXVECTOR3(m_size.x, m_size.y, 0.0f));
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTimerCount::Uninit(void)
{
	// 2D�|���S���̔j��
	for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
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
void CTimerCount::Update(void)
{
	// �X�g�b�v��Ԕ���
	if (!m_bStop)
	{
		// ���݂̎��Ԃ��擾
		m_dwCurrentTime = timeGetTime();

		// �t�F�[�h�̎擾
		CFade *pFade = CManager::GetInstance()->GetFade();

		// ���Ԍo�ߏ����i1�b�Ō��炷�j
		if ((m_dwCurrentTime - m_dwExecLastTime) >= REDUCE_TIME && m_nTimerCount != 0)
		{
			AddTimerCount(-1);
			m_dwCurrentTime = 0;
			m_dwExecLastTime = timeGetTime();
		}

		// �����̐ݒ�
		for (int nCnt = 0; nCnt < MAX_TIME; nCnt++)
		{
			m_apNumber[nCnt]->SetNumber(m_nTimerCount % (int)pow(10, MAX_TIME - nCnt) / (int)pow(10, (MAX_TIME - 1) - nCnt));
		}
		// ���Ԑ؂�ɂȂ�����
		if (m_nTimerCount == 0)
		{
			m_bLimit = true;

			if (!m_bFade)
			{
				// ��ʑJ�� / �t�F�[�h����
				m_bFade = true;
			}

		}

		if (m_bLimit)
		{

		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTimerCount::Draw(void)
{
	// 2D�|���S���̕`��
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		m_apNumber[nCntTime]->Draw();
	}
}

//=============================================================================
// ���Ԃ̉��Z
//=============================================================================
void CTimerCount::AddTimerCount(int nValue)
{
	m_nTimerCount += nValue;
}
//=============================================================================
// ���Ԃ̎擾
//=============================================================================
int CTimerCount::GetTimerCount(void)
{
	return m_nTimerCount;
}

//=============================================================================
// ���Ԃ̐���
//=============================================================================
CTimerCount *CTimerCount::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int time, bool bStop)
{
	// ���Ԃ̃C���X�^���X����
	CTimerCount *pTimerCount = nullptr;

	if (pTimerCount = new CTimerCount)
	{
		pTimerCount->m_bStop = bStop;		// ��~���邩
		pTimerCount->m_nTimerCount = time;	// ����
		pTimerCount->Init(pos, size);		// ������
	}
	return pTimerCount;
}

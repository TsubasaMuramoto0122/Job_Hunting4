//==============================================================
// �X�R�A�̍쐬
// Author:������
//==============================================================
#include "main.h"
#include "manager.h"
#include "keyboard.h"
#include "scene.h"
#include "scene2D.h"
#include "score.h"
#include "texture.h"
#include "number.h"
#include "playdata.h"

//==============================================================
// �}�N����`
//==============================================================
#define NUMBER_INTERVAL (50.0f)

//==============================================================
// �R���X�g���N�^
//==============================================================
CScore::CScore(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �T�C�Y
	m_nAddScore = ADD_SCORE;				// ���Z����X�R�A�̏�����
	m_nScore = 0;							// �X�R�A�̏�����
}

//==============================================================
// �f�X�g���N�^
//==============================================================
CScore::~CScore()
{

}

//==============================================================
// �X�R�A�̐���
//==============================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �C���X�^���X����
	CScore *pScore = nullptr;
	if (pScore = new CScore())
	{
		pScore->Init(pos, size);	// ������
	}

	return pScore;
}

//==============================================================
// ������
//==============================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �i���o�[����
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x + nCnt * NUMBER_INTERVAL, pos.y, pos.z), size);
		m_apNumber[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_NUMBER"));
		m_apNumber[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_nAll++;
	}

	// �ʒu�E�T�C�Y�̐ݒ�E�擾
	m_pos = pos;
	m_size = size;
	SetPos(m_pos);
	SetSize(m_size);

	return S_OK;
}

//==============================================================
// �I��
//==============================================================
void CScore::Uninit()
{
	// �i���o�[�j��
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Uninit();
			m_apNumber[nCnt] = nullptr;
		}
	}

	CManager::GetPlayData()->SetScore(m_nScore);	// �v���C�f�[�^�ɃX�R�A��ݒ�
	m_nAll = 0;										// �����̐������Z�b�g
	Release();										// ���̃I�u�W�F�N�g��j��
}

//==============================================================
// �X�V
//==============================================================
void CScore::Update()
{
#if _DEBUG
	// �f�o�b�O�p�X�R�A���Z
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	if (pKey->GetTrigger(DIK_0))
	{
		AddScore(m_nAddScore);	// �X�R�A�̉��Z
	}
#endif

	// �����̍X�V�֐�
	SetScore();
}

//==============================================================
// �`��
//==============================================================
void CScore::Draw()
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_apNumber[nCnt])
		{
			m_apNumber[nCnt]->Draw();
		}
	}
}

//==============================================================
// �X�R�A�̐ݒ�
//==============================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;
}

//==============================================================
// �X�R�A�̐ݒ�
//==============================================================
void CScore::SetScore()
{
	for (int nCnt = 0, nDecrement = 1; nCnt < m_nAll; nCnt++, nDecrement *= 10)
	{
		m_apNumber[nCnt]->SetNumber(m_nScore % ((int)pow(10, m_nAll) / nDecrement) / ((int)pow(10, m_nAll - 1) / nDecrement));
	}
}

//==============================================================
// �X�R�A�ʒu�ݒ菈��
//==============================================================
void CScore::SetScorePos(D3DXVECTOR3 pos)
{
	// �ʒu�ݒ�
	m_pos = pos;
	for (int nCntNumber = 0; nCntNumber < MAX_SCORE; nCntNumber++)
	{
		// �����_�����邽�߁A4�ڂ̐������炸�炷
		m_apNumber[nCntNumber]->SetPos(D3DXVECTOR3(pos.x - (m_size.x * MAX_SCORE / 2) + (m_size.x * nCntNumber) + (m_size.x / 2.0f), pos.y, 0.0f), m_size,false);
	}
	// �ʒu��ݒ�
	SetPos(m_pos);
}


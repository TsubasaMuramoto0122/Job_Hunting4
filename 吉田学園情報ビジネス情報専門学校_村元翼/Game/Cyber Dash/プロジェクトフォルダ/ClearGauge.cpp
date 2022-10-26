//============================================================
// �N���A�Q�[�W�̍쐬
// Author:������
//============================================================
#include "ClearGauge.h"
#include "number.h"
#include "stage.h"
#include "Goal.h"
#include "game.h"
#include "StringManager.h"
#include "Ui.h"
#include "meshfield.h"

//============================================================
// �}�N����`
//============================================================
#define GAUGE_FRAME_OFFSET		(D3DXVECTOR3(100.0f,0.0f,0.0f))
#define GAUGE_UI_SIZE			(D3DXVECTOR3(m_size.x * 8.0f,m_size.y * 2.0f,0.0f))
#define GAUGE_MAX_LENGTH		(m_GaugeUI.size.x - 30.0f)
#define GAUGE_OFFSET_X			(10.0f)
#define MESH_SPLIT_SIZE			(250.0f)
#define CLEAR_GAUGE				(100)

//============================================================
// �ÓI�����o�ϐ��̏�����
//============================================================
bool CClearGauge::m_bClear = false;		// �N���A����

//============================================================
// �R���X�g���N�^
//============================================================
CClearGauge::CClearGauge(OBJTYPE nPriority) :CScene(nPriority)
{
	memset(&m_GaugeUI, 0, sizeof(m_GaugeUI));		// �Q�[�WUI
	memset(m_apPercent, 0, sizeof(m_apPercent));	// �p�[�Z���g����
	m_pos = { 0.0f,0.0f,0.0f };						// �ʒu
	m_size = { 0.0f,0.0f,0.0f };					// �傫��
	m_ClearLength = { 0.0f,0.0f,0.0f };				// �N���A�Q�[�W�̒���
	m_nPercent = 0;									// �p�[�Z���g
}

//============================================================
// �f�X�g���N�^
//============================================================
CClearGauge::~CClearGauge()
{

}

//============================================================
// �I�u�W�F�N�g����
//============================================================
CClearGauge *CClearGauge::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �I�u�W�F�N�g����
	CClearGauge *pClearGauge = nullptr;
	if (pClearGauge = new CClearGauge)
	{
		pClearGauge->Init(pos, size);
	}

	return pClearGauge;
}

//============================================================
// ������
//============================================================
HRESULT CClearGauge::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�E�T�C�Y�ݒ�
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// UI�̊e�ϐ�������
	m_GaugeUI.posFrame	= m_pos - GAUGE_FRAME_OFFSET;
	m_GaugeUI.posGauge	= { m_GaugeUI.posFrame.x + GAUGE_OFFSET_X, m_GaugeUI.posFrame.y,0.0f };
	m_GaugeUI.size		= GAUGE_UI_SIZE;
	m_GaugeUI.MaxLength = GAUGE_MAX_LENGTH;

	// �Q�[�WUI�̐���
	if (!m_GaugeUI.pFrame && !m_GaugeUI.pGauge)
	{
		m_GaugeUI.pGauge = CUi::Create(m_pos, m_size, CUi::UI_TYPE_NONE);
		m_GaugeUI.pFrame = CUi::Create(m_pos, m_size, CUi::UI_TYPE_NONE);

		m_GaugeUI.pGauge->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_GAUGE01"));
		m_GaugeUI.pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_GAUGE_FRAME01"));

		m_GaugeUI.pGauge->SetScalePos(m_GaugeUI.posGauge, m_GaugeUI.size);
		m_GaugeUI.pFrame->SetScalePos(m_GaugeUI.posFrame, m_GaugeUI.size);
	}

	// �p�[�Z���g�����𐶐�
	CStringManager::Create({ m_pos.x + (m_size.x * (MAX_PERCENT_DIGIT + 1)), m_pos.y - 20.0f, 0.0f }, m_size.x, (int)m_size.x, "��", "�l�r�S�V�b�N");

	// �����𐶐�
	for (int nCnt = 0; nCnt < MAX_PERCENT_DIGIT; nCnt++)
	{
		m_apPercent[nCnt] = CNumber::Create({ m_pos.x + (m_size.x * nCnt), m_pos.y, 0.0f }, m_size);
	}

	// �@�X�^�[�g�n�_�ƃS�[���n�_�̊Ԃ̋����𑪂�
	CStage *pStage = CGame::GetStage();
	CGoal *pGoal = pStage->GetGoal();
	m_ClearLength = (pStage->GetGoalPos() - (pGoal->GetSize() * pGoal->GetScale().x)) - pStage->GetStartPos();

	// ���b�V��������
	int nLine = 0;
	nLine = (int)(m_ClearLength.x / MESH_SPLIT_SIZE);
	if (nLine <= 0)	// 0�ȉ��̓��b�V�����`�悳��Ȃ�
	{
		nLine = 1;
	}

	// �X�^�[�g����S�[���܂ł̃t�B�[���h�𐶐�����
	CMeshField::Create({ m_ClearLength.x / 2 + pStage->GetStartPos().x ,0.0f,0.0f }, { m_ClearLength.x - (pStage->GetStartPos().x/2),0.0f,1000.0f }, { 0.0f,0.0f,0.0f }, nLine, 3);
	CMeshField::Create({ m_ClearLength.x / 2 + pStage->GetStartPos().x ,1000.0f,0.0f }, { m_ClearLength.x - (pStage->GetStartPos().x / 2),0.0f,1000.0f }, { 180.0f,0.0f,0.0f }, nLine, 3);

	return S_OK;
}

//============================================================
// �I��
//============================================================
void CClearGauge::Uninit()
{
	// �����̔j��
	for (int nCnt = 0; nCnt < MAX_PERCENT_DIGIT; nCnt++)
	{
		if (m_apPercent[nCnt])
		{
			m_apPercent[nCnt]->Uninit();
			m_apPercent[nCnt] = nullptr;
		}
	}

	// �Q�[�WUI�̔j��
	if (m_GaugeUI.pGauge)
	{
		m_GaugeUI.pGauge->Uninit();
		m_GaugeUI.pGauge = nullptr;
	}

	// �Q�[�W�t���[��UI�̔j��
	if (m_GaugeUI.pFrame)
	{
		m_GaugeUI.pFrame->Uninit();
		m_GaugeUI.pFrame = nullptr;
	}

	// ���̃I�u�W�F�N�g��j��
	Release();
}

//============================================================
// �X�V
//============================================================
void CClearGauge::Update()
{
	// �p�[�Z���g�Q�[�W�̌v�Z�֐�
	SetPercent();
	
	if (m_nPercent >= CLEAR_GAUGE)
	{
		m_bClear = true;
	}
}

//============================================================
// �`��
//============================================================
void CClearGauge::Draw()
{

}

//============================================================
// �N���A�Q�[�W�̃p�[�Z���g�Ǘ�
//============================================================
void CClearGauge::SetPercent(void)
{
	if (CScene::GetScene(OBJTYPE_PLAYER) && m_nPercent < CLEAR_GAUGE)
	{
		// �v���C���[�̌��݈ʒu�����
		D3DXVECTOR3 CurrentPos = CScene::GetScene(OBJTYPE_PLAYER)->GetPos()- CStage::GetStartPos();

		// �����̓��ǂ̈ʒu�ɂ��邩�̊��������߂�(X�̋����̂�)
		float fRatio = CurrentPos.x / m_ClearLength.x;

		// �S�����ɒ���
		m_nPercent = (int)(fRatio * CLEAR_GAUGE);

		// �Q�[�W�̒������v�Z����
		float fGaugePer = (m_GaugeUI.MaxLength / CLEAR_GAUGE) * m_nPercent;

		// �Q�[�WUI�̒�����ݒ�
		if (fGaugePer <= m_GaugeUI.MaxLength)
		{
			m_GaugeUI.pGauge->SetScalePos(m_GaugeUI.posGauge, { fGaugePer ,m_GaugeUI.size.y,0.0f });
		}

		// �v�Z��̐�����ݒ�
		for (int nCnt = 0; nCnt < MAX_PERCENT_DIGIT; nCnt++)
		{
			if (m_apPercent[nCnt])
			{
				m_apPercent[nCnt]->SetNumber(m_nPercent % (int)pow(10, MAX_PERCENT_DIGIT - nCnt) / (int)pow(10, (MAX_PERCENT_DIGIT - 1) - nCnt));
			}
		}
	}
}
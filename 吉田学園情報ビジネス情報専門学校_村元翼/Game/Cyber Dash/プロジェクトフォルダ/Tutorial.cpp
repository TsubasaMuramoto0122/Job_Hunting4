//=====================================
// �`���[�g���A��
// Author:������
//=====================================
#include "Tutorial.h"
#include "bg.h"
#include "stage.h"
#include "camera.h"
#include "manager.h"
#include "InputManager.h"
#include "StringManager.h"

//=====================================
// �`���[�g���A����
// �y�[�W1�Ƀv���C���[�̐���
// �y�[�W2�ɃQ�[�g�ƃW�����v�T�[�N���̐���
// �㔼���Ƀ`���[�g���A���ő����������ꏊ�����A
// �������ɗ���郁�b�Z�[�W�����B
//=====================================

//=====================================
// �}�N����`
//=====================================
#define EXP_POS			(D3DXVECTOR3(SCREEN_WIDTH/2,SCREEN_HEIGHT/1.5f,0.0f))
#define EXP_FONTNAME	("HG�n�p�p�߯�ߑ�")

//=====================================
// �R���X�g���N�^
//=====================================
CTutorial::CTutorial()
{
	m_Exp = EXP_01;
	m_pPlayer[TYPE_MAX] = { nullptr };
	m_pExpStr = nullptr;
	m_pStage = nullptr;
}

//=====================================
// �f�X�g���N�^
//=====================================
CTutorial::~CTutorial()
{

}

//=====================================
// ������
//=====================================
HRESULT CTutorial::Init()
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
	pCamera->SetRot({ D3DX_PI/2,0.0f,0.0f });
	pCamera->NoFollowingPlayer();

	// �X�e�[�W����
	if (!m_pStage)
	{
		if (m_pStage = new CStage)
		{
			m_pStage->Init();
			m_pStage->SetStage("data/TEXT/TutorialSet000.txt");
		}
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (!m_pPlayer[nCnt])
		{
			bool bMove = false;
			CPlayer::PLAYER_STATE State;

			switch (nCnt)
			{
			case 0:
				State = CPlayer::PLAYER_STATE::CUBE;
				bMove = true;
				break;

			case 1:
				State = CPlayer::PLAYER_STATE::BALL;
				break;

			case 2:
				State = CPlayer::PLAYER_STATE::AIRSHIP;
				break;

			case 3:
				State = CPlayer::PLAYER_STATE::UFO;
				break;
			}

			// �v���C���[����
			m_pPlayer[nCnt] = CPlayer::Create({ -450.0f + (300.0f * nCnt),100.0f,0.0f }, { 0.0f,0.0f,0.0f }, State, bMove,false);
		}
	}

	return S_OK;
}

//=====================================
// �I��
//=====================================
void CTutorial::Uninit()
{
	// ������j��
	if (m_pExpStr)
	{
		m_pExpStr->Uninit();
		delete m_pExpStr;
		m_pExpStr = nullptr;
	}

	// �X�e�[�W�j��
	if (m_pStage)
	{
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	// �v���C���[�j��
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_pPlayer[nCnt])
		{
			m_pPlayer[nCnt]->Uninit();
			m_pPlayer[nCnt] = nullptr;
		}
	}
}

//=====================================
// �X�V
//=====================================
void CTutorial::Update()
{
	if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_RIGHT))
	{
		ExpChange(m_Exp + 1);
	}

	else if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_LEFT))
	{
		ExpChange(m_Exp - 1);
	}
}

//=====================================
// �`��
//=====================================
void CTutorial::Draw()
{

}

//=====================================
// �����ԍ��Ǘ�
//=====================================
void CTutorial::ExpManager(void)
{

}

//=====================================
// ������ԕω�
//=====================================
void CTutorial::ExpChange(int nExp)
{
	if (m_Exp != nExp)
	{
		m_pPlayer[m_Exp]->SetMoveBool(false);
		if (m_pPlayer[m_Exp]->GetReverse())	// �d�͔��]���Ă����猳�ɖ߂�
		{
			m_pPlayer[m_Exp]->SetReverse();
		}

		if (nExp < EXP_01)
		{
			nExp = EXP_04;
		}

		else if (nExp > EXP_04)
		{
			nExp = EXP_01;
		}

		m_Exp = static_cast<CHARA_EXP>(nExp);
		m_pPlayer[m_Exp]->SetMoveBool(true);

		switch (m_Exp)
		{
		case EXP_01:
			KeyExpCreate(&m_pExpStr, "�v���C���[����1");
			break;

		case EXP_02:
			KeyExpCreate(&m_pExpStr, "�v���C���[����2");
			break;

		case EXP_03:
			KeyExpCreate(&m_pExpStr, "�v���C���[����3");
			break;

		case EXP_04:
			KeyExpCreate(&m_pExpStr, "�v���C���[����4");
			break;
		}
	}
}

//=====================================
// �����񐶐�
//=====================================
void  CTutorial::KeyExpCreate(CStringManager **ppKeyExp, const char *cStr, const int& nFontSize)
{
	// �����񂪎c���Ă���Ȃ�j�����Ă��琶��
	if (ppKeyExp[0])
	{
		ppKeyExp[0]->Uninit();
		delete ppKeyExp[0];
		ppKeyExp[0] = nullptr;
	}

	// �����񐶐�
	if (!ppKeyExp[0])
	{
		ppKeyExp[0] = CStringManager::Create(EXP_POS, static_cast<float>(nFontSize), nFontSize, cStr, EXP_FONTNAME);
	}
}

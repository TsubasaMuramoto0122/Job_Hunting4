//=============================================
// ���U���g��ʂ̏���
// Author:������
//=============================================
#include "result.h"
#include "title.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "ranking.h"
#include "fade.h"
#include "bg.h"
#include "Ui.h"
#include "XInput.h"
#include "DirectInput.h"
#include "InputManager.h"
#include "sound.h"
#include "game.h"
#include "RemainCount.h"

//=============================================
// �}�N����`
//=============================================
#define RETURN_TITLE_TIME (120)

//===============================================
// �ÓI�����o�ϐ��̏�����
//===============================================
const CResult::SELECT_PARAM CResult::m_SelectParam[RESULT_SELECT_MAX] =
{
	{ { SCREEN_WIDTH -200.0f,450.0f,0.0f }	,{ 200.0f,100.0f,0.0f },"TEX_TYPE_RESULT_SELECT00",CUi::UI_TYPE_NONE },	// �Z���N�g01
	{ { SCREEN_WIDTH -200.0f,550.0f,0.0f }	,{ 200.0f,100.0f,0.0f },"TEX_TYPE_RESULT_SELECT01",CUi::UI_TYPE_NONE },	// �Z���N�g02
	{ { SCREEN_WIDTH -200.0f,650.0f,0.0f }	,{ 200.0f,100.0f,0.0f },"TEX_TYPE_RESULT_SELECT02",CUi::UI_TYPE_NONE }	// �Z���N�g03
};

//=============================================
// �R���X�g���N�^
//=============================================
CResult::CResult()
{
	memset(m_pSelectUI, 0, sizeof(m_pSelectUI));	// �I��UI
	m_pBgResult = nullptr;							// ���U���g�w�i
	m_pReturnTitle = nullptr;						// �^�C�g���ɖ߂�UI
	m_pUi = nullptr;								// UI
	m_nFrame = 0;									// �t���[��
	m_nSelectType = 0;								// �I���^�C�v
	m_nSelect = 0;									// �I����
	m_bOPRLock = false;								// ���샍�b�N����
	m_bUnlock = false;								// �A�����b�N����
}

//=============================================
// �f�X�g���N�^
//=============================================
CResult::~CResult()
{
}

//=============================================
// ������
//=============================================
HRESULT CResult::Init(void)
{
	// ���S�J�E���g�̃��Z�b�g
	CManager::GetInstance()->GetGame()->GetRemainCount()->ResetRemain();

	// ���U���g�̔w�i����
	m_pBgResult = CBg::Create();
	m_pBgResult->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	// UI�̐���
	m_pUi = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR3(800.0f, 200.0f, 0.0f), CUi::UI_TYPE_NONE);
	m_pUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_RESULT"));

	//�����L���O�̐���
	m_pRanking = CRanking::Create
	(
		D3DXVECTOR3(400.0f + RANKING_POLYGON_WIDTH * MAX_SCORE, RESULT_RANKING_POSY, 0.0f),
		D3DXVECTOR3(RANKING_POLYGON_WIDTH, RANKING_POLYGON_HEIGHT, 0.0f)
	);

	// �I�����̐���
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	for (int nCnt = 0; nCnt < RESULT_SELECT_MAX; nCnt++)
	{
		// �I����UI�̐���
		if (!m_pSelectUI[nCnt])
		{
			m_pSelectUI[nCnt] = CUi::Create(m_SelectParam[nCnt].pos, m_SelectParam[nCnt].size, m_SelectParam[nCnt].UiType);
			m_pSelectUI[nCnt]->BindTexture(pTex->GetTexture(m_SelectParam[nCnt].Tex));
			m_pSelectUI[nCnt]->SetCol(NOT_SELECT_COLOR);
		}
	}
	m_pSelectUI[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �J���[�̐ݒ�

	return S_OK;
}
//=============================================
// �I��
//=============================================
void CResult::Uninit(void)
{
	//------------------------//
	// �e��I�u�W�F�N�g�̔j�� //
	//------------------------//
	for (int nCnt = 0; nCnt < RESULT_SELECT_MAX; nCnt++)
	{
		if (m_pSelectUI[nCnt])
		{
			m_pSelectUI[nCnt]->Uninit();
			m_pSelectUI[nCnt] = nullptr;
		}
	}

	if (m_pUi)
	{
		m_pUi->Uninit();
		m_pUi = nullptr;
	}

	if (m_pRanking)
	{
		m_pRanking->Uninit();
		m_pRanking = nullptr;
	}

	if (m_pBgResult)
	{
		m_pBgResult->Uninit();
		m_pBgResult = nullptr;
	}
}

//=============================================
// �X�V
//=============================================
void CResult::Update(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

#ifdef _DEBUG
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_R))
	{
		m_pRanking->DeleteRanking();
	}
#endif

	// ���������
	if (!m_bOPRLock)
	{
		if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_ENTER) || CInputManager::TriggerAnyAction(CInputManager::TRIGGER_ATTACK))
		{
			// ���艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);

			switch (m_nSelect)
			{
			case RESULT_SELECT_01:
				// �Q�[�����[�h�ֈڍs
				if (CManager::SetModeBool(CManager::MODE::GAME))
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE_START);
					m_bOPRLock = true;	// ��������b�N
				}
				break;

			case RESULT_SELECT_02:
					CFade::SetFade(CManager::MODE::TITLE);
					m_bOPRLock = true;	// ��������b
				break;

			case RESULT_SELECT_03:
				// �Q�[�����I��
				PostQuitMessage(0);
				break;
			}
		}

		// �I�������ֈړ�������
		else if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_UP) || CInputManager::TriggerAnyAction(CInputManager::TRIGGER_DOWN))
		{
			// ���ݑI������Ă���I�����̖��邳��������
			m_pSelectUI[m_nSelect]->SetCol(NOT_SELECT_COLOR);

			// �J�[�\���ړ�����
			if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_UP))
			{
				CursorMove(m_nSelect, RESULT_SELECT_03, RESULT_SELECT_01, false);
			}

			// �I�����E�ֈړ�������
			else if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_DOWN))
			{
				CursorMove(m_nSelect, RESULT_SELECT_01, RESULT_SELECT_03, true);
			}

			// �ړ���̑I�����̃J���[�𖾂邳���ő�ɂ���
			m_pSelectUI[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//=============================================
// �`��
//=============================================
void CResult::Draw(void)
{

}

//============================================
// �J�[�\���ړ�����
//============================================
void CResult::CursorMove(int &nNowSel, const int &nFirst, const int &nLast, const bool &bMinus)
{
	// �J�[�\����
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSOR);

	if (!bMinus)
	{
		// �I���������炷
		if (nNowSel > nLast)
		{
			nNowSel--;
		}
		else
		{
			nNowSel = nFirst;
		}
	}

	else
	{
		// �I���������炷
		if (nNowSel < nLast)
		{
			nNowSel++;
		}
		else
		{
			nNowSel = nFirst;
		}
	}
}
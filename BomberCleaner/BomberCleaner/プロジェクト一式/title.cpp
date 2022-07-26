//============================================
// �^�C�g���̏���
// Author:������
//============================================
#include "title.h"
#include "texture.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "bg.h"
#include "Ui.h"
#include "sound.h"
#include "StringManager.h"

//===============================================
// �}�N����`
//===============================================
#define NOT_SELECT_COLOR	(D3DXCOLOR(0.3f,0.3f,0.3f,1.0f))													// �I������Ă��Ȃ����̐F

#define PRESS_ENTER_SIZE	(D3DXVECTOR3(1200.0f, 1000.0f, 0.0f))												// PressEnterUI�̃T�C�Y
#define PRESS_ENTER_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f))										// PressEnterUI�̈ʒu

#define TITLE_LOGO_SIZE		(D3DXVECTOR3(1200.0f, 500.0f, 0.0f))												// �^�C�g�����S�̃T�C�Y
#define TITLE_LOGO_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, 200.0f, 0.0f))										// �^�C�g�����S�̈ʒu

#define TUTORIAL_SIZE		(D3DXVECTOR3(1000.0f,600.0f,0.0f))													// �`���[�g���A���T�C�Y

#define SCROLL_UI_SIZE		(D3DXVECTOR3(TUTORIAL_SIZE.x,TUTORIAL_SIZE.y / 20.0f,0.0f))							// �X�N���[��UI�̃T�C�Y
#define SCROLL_SPEED		(0.005f)																			// �X�N���[���X�s�[�h
#define SCROLL_UI_INDENT	(100.0f)																			// �X�N���[��UI���m�̌���
#define SCROLL_UI_SPLIT		(D3DXVECTOR2(10.0f,1.0f))															// �X�N���[���e�N�X�`��������

#define KEY_EXP_POS			(D3DXVECTOR3(SCREEN_CENTER.x - (SCREEN_CENTER.x / 1.8f),SCREEN_CENTER.y + 130.0f,0.0f))// �L�[�����̈ʒu								// �L�[�����̃t�H���g�T�C�Y
#define KEY_EXP_FONTNAME	("HG�n�p�p�߯�ߑ�")

//===============================================
// �ÓI�����o�ϐ��̏�����
//===============================================
const CTitle::SELECT_PARAM CTitle::m_SelectParam[TITLE_SELECT_MAX] =
{
	{ { 300.0f,500.0f,0.0f }			,{ 300.0f,150.0f,0.0f },"TEX_TYPE_TITLE_SELECT00",CUi::UI_TYPE_NONE },
	{ { SCREEN_WIDTH / 2,600.0f,0.0f }	,{ 300.0f,150.0f,0.0f },"TEX_TYPE_TITLE_SELECT01",CUi::UI_TYPE_NONE },
	{ { 980.0f,500.0f,0.0f }			,{ 300.0f,150.0f,0.0f },"TEX_TYPE_TITLE_SELECT02",CUi::UI_TYPE_NONE }
};

//============================================
// �R���X�g���N�^
//============================================
CTitle::CTitle()
{
	// �ϐ�������
	memset(m_pSelectUI, 0, sizeof(m_pSelectUI));
	memset(m_pScrollUI, 0, sizeof(m_pScrollUI));
	m_pTutorial		= nullptr;
	m_pPressEnter	= nullptr;
	m_pTitleLogo	= nullptr;
	m_Page			= PAGE_01;
	m_TutoPage		= TUTORIAL_PAGE_01;
	m_nSelectKey	= 0;
	m_nSelect		= 0;
	m_bOPRLock		= false;
}

//============================================
// �f�X�g���N�^
//============================================
CTitle::~CTitle()
{

}

//============================================
// ������
//============================================
HRESULT CTitle::Init(void)
{
	// �^�C�g���w�i�̐���
	CBg::Create();

	// �e�N�X�`���N���X�̎擾
	CTexture *pTex = CManager::GetInstance()->GetTexture();

	// PressStartUI�̐���
	if (!m_pPressEnter)
	{
		m_pPressEnter = CUi::Create(PRESS_ENTER_POS, PRESS_ENTER_SIZE, CUi::UI_TYPE_BLINK);
		m_pPressEnter->BindTexture(pTex->GetTexture("TEX_TYPE_PRESS_START"));
	}
	
	// �^�C�g��UI�̐���
	if (!m_pTitleLogo)
	{
		m_pTitleLogo = CUi::Create(TITLE_LOGO_POS, TITLE_LOGO_SIZE);
		m_pTitleLogo->BindTexture(pTex->GetTexture("TEX_TYPE_TITLE"));
	}

	return S_OK;
}

//============================================
// �I��
//============================================
void CTitle::Uninit(void)
{
	//-------------------------------
	// �I�u�W�F�N�g�I������
	//-------------------------------
	// �I����UI�̍폜
	for (int nCnt = 0; nCnt < TITLE_SELECT_MAX; nCnt++)
	{
		if (m_pSelectUI[nCnt])
		{
			m_pSelectUI[nCnt]->Uninit();
			m_pSelectUI[nCnt] = nullptr;
		}
	}

	// �`���[�g���A��UI�̔j��
	if (m_pTutorial)
	{
		m_pTutorial->Uninit();
		m_pTutorial = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_SCROLL_UI; nCnt++)
	{
		if (m_pScrollUI[nCnt])
		{
			m_pScrollUI[nCnt]->Uninit();
			m_pScrollUI[nCnt] = nullptr;
		}
	}

	// PressEnter�̍폜
	if (m_pPressEnter)
	{
		m_pPressEnter->Uninit();
		m_pPressEnter = nullptr;
	}

	// �^�C�g�����S�̍폜
	if (m_pTitleLogo)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = nullptr;
	}
}
//--------------------------------------------
//�X�V
//--------------------------------------------
void CTitle::Update(void)
{
	// �N���X���擾
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	CSound *pSound = CManager::GetInstance()->GetSound();

	//-------------------------------------------------------
	// �y�[�W�ʏ���
	//-------------------------------------------------------
	switch (m_Page)
	{
		//---------------------------------------
		// �y�[�W1
		//---------------------------------------
	case PAGE_01:
		if (CInput::PressAnyAction(CInput::ACTION_ENTER))
		{
			// ���艹
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);

			// �y�[�W2�Ɉڍs
			m_Page = PAGE_02;

			// �v���X�X�^�[�g��j��
			if (m_pPressEnter)
			{
				m_pPressEnter->Uninit();
				m_pPressEnter = nullptr;
			}

			// �I�����̐���

			for (int nCnt = 0; nCnt < TITLE_SELECT_MAX; nCnt++)
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
		}
		break;

		//---------------------------------------
		// �y�[�W2
		//---------------------------------------
	case PAGE_02:
		// ���������
		if (!m_bOPRLock)
		{
			if (CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK))
			{
				// ���艹
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);

				switch (m_nSelect)
				{
				case TITLE_SELECT_01:
					// �Q�[�����[�h�ֈڍs
					if (CManager::SetModeBool(CManager::MODE::GAME))
					{
						pSound->PlaySound(CSound::SOUND_LABEL_SE_START);
						m_bOPRLock = true;	// ��������b�N
					}
					break;

				case TITLE_SELECT_02:
					// �y�[�W3�ֈڍs
					m_Page = PAGE_03;

					break;

				case TITLE_SELECT_03:
					// �Q�[�����I��
					PostQuitMessage(0);
					break;
				}
			}

			// �I�������ֈړ�������
			else if (CInput::PressAnyAction(CInput::ACTION_LEFT) || CInput::PressAnyAction(CInput::ACTION_RIGHT))
			{

				// ���ݑI������Ă���I�����̖��邳��������
				m_pSelectUI[m_nSelect]->SetCol(NOT_SELECT_COLOR);

				// �J�[�\���ړ�����
				if (CInput::PressAnyAction(CInput::ACTION_LEFT))
				{
					CursorMove(m_nSelect, TITLE_SELECT_03, TITLE_SELECT_01, false);
				}

				// �I�����E�ֈړ�������
				else if (CInput::PressAnyAction(CInput::ACTION_RIGHT))
				{
					CursorMove(m_nSelect, TITLE_SELECT_01, TITLE_SELECT_03, true);
				}

				// �ړ���̑I�����̃J���[�𖾂邳���ő�ɂ���
				m_pSelectUI[m_nSelect]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		break;

		//---------------------------------------
		// �y�[�W3
		//---------------------------------------
		case PAGE_03:
			// �`���[�g���A��UI�̐���
			if (!m_pTutorial)
			{
				m_pTutorial = CUi::Create(SCREEN_CENTER, TUTORIAL_SIZE);
				m_pTutorial->BindTexture(pTex->GetTexture("TEX_TYPE_TUTORIAL01"));
			}

			TutorialDisplay(&m_pTutorial,m_pScrollUI,pTex);	// �`���[�g���A���\���֐�
			break;
	}
}

//============================================
// �`��
//============================================
void CTitle::Draw(void)
{

}

//============================================
// �`���[�g���A����ʕ\������
//============================================
void CTitle::TutorialDisplay(CUi **pTutoUI, CUi **pScr, const CTexture *pTex)
{
	switch (m_TutoPage)
	{
		//---------------------------------
		// �y�[�W1
		//---------------------------------
	case TUTORIAL_PAGE_01:
		// ���̃y�[�W��
		if (CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			// ���艹
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);

			// �y�[�W2��
			m_TutoPage = TUTORIAL_PAGE_02;

			// �e�N�X�`���ύX
			pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_TUTORIAL02"));

			// �X�N���[��UI�̐���
			for (int nCnt = 0; nCnt < MAX_SCROLL_UI; nCnt++)
			{
				pScr[nCnt] = CUi::Create({ SCREEN_CENTER.x,SCREEN_CENTER.y + ((nCnt + 1) * SCROLL_UI_INDENT),0.0f }, SCROLL_UI_SIZE / 1.14f, CUi::UI_TYPE_NONE);
				pScr[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_CAUTION"));
				pScr[nCnt]->SetTex({ 0.0f,0.0f }, SCROLL_UI_SPLIT);
			}

			KeyExpCreate(&m_pKeyExp, "�`���c�܂��͏\���{�^�����ŃL�[�̐������o�܂�");							// WASD����

		}
		break;

		//---------------------------------
		// �y�[�W2
		//---------------------------------
	case TUTORIAL_PAGE_02:

		// �ǂ̃L�[�������؂�ւ���
		if (CInput::PressAnyAction(CInput::ACTION_LEFT) || CInput::PressAnyAction(CInput::ACTION_RIGHT))
		{
			if (CInput::PressAnyAction(CInput::ACTION_LEFT))
			{
				CursorMove(m_nSelectKey, OPR_INSTR_06, OPR_INSTR_01, false);
			}

			else if (CInput::PressAnyAction(CInput::ACTION_RIGHT))
			{
				CursorMove(m_nSelectKey, OPR_INSTR_01, OPR_INSTR_06, true);
			}

			// �e�N�X�`���ύX
			switch (m_nSelectKey)
			{
			case OPR_INSTR_01:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_KEY_SELECT01"));	// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�v�`�r�c�F�ړ����I�����؂�ւ�");			// WASD����
				break;

			case OPR_INSTR_02:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_KEY_SELECT02"));	// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�r�o�`�b�d�F�W�����v");					// SPACE�L�[����
				break;

			case OPR_INSTR_03:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_KEY_SELECT04"));	// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�o�L�[�F�|�[�Y��ʂ��J��������");		// �|�[�Y����
				break;

			case OPR_INSTR_04:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_KEY_SELECT03"));	// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�d�m�s�d�q�F����");						// �G���^�[����
				break;

			case OPR_INSTR_05:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_KEY_SELECT05"));	// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "���N���b�N�F���e������������");			// ���N���b�N����
				break;

			case OPR_INSTR_06:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_KEY_SELECT06"));	// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�}�E�X�J�[�\���F���_�𓮂���");			// �}�E�X�J�[�\���ړ�����
				break;
			}
		}

		// ���̃y�[�W��
		if (CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			// ���艹
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);

			// �y�[�W3��
			m_TutoPage = TUTORIAL_PAGE_03;

			// �e�N�X�`���ύX
			pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_TUTORIAL03"));

			m_nSelectKey = OPR_INSTR_07;
			KeyExpCreate(&m_pKeyExp, "�`���c�܂��͏\���{�^�����Ń{�^���̐������o�܂�");

		}
		break;

		//---------------------------------
		// �y�[�W3
		//---------------------------------
	case TUTORIAL_PAGE_03:
		// �ǂ̃L�[�������؂�ւ���
		if (CInput::PressAnyAction(CInput::ACTION_LEFT) || CInput::PressAnyAction(CInput::ACTION_RIGHT))
		{
			if (CInput::PressAnyAction(CInput::ACTION_LEFT))
			{
				CursorMove(m_nSelectKey, OPR_INSTR_MAX - 1, OPR_INSTR_07, false);
			}

			else if (CInput::PressAnyAction(CInput::ACTION_RIGHT))
			{
				CursorMove(m_nSelectKey, OPR_INSTR_07, OPR_INSTR_MAX - 1, true);
			}

			// �e�N�X�`���ύX
			switch (m_nSelectKey)
			{
			case OPR_INSTR_07:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_PAD_SELECT01"));			// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "���X�e�B�b�N�F�ړ�");								// �ړ�����
				break;

			case OPR_INSTR_08:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_PAD_SELECT06"));			// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�\���{�^���F�I�����؂�ւ�");						// �I��؂�ւ�����
				break;

			case OPR_INSTR_09:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_PAD_SELECT02"));			// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�E�X�e�B�b�N�F���_�𓮂���");						// ���_�ړ�����
				break;

			case OPR_INSTR_10:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_PAD_SELECT03"));			// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�r�s�`�q�s�{�^���F�|�[�Y��ʂ��J��������");		// �|�[�Y����
				break;

			case OPR_INSTR_11:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_PAD_SELECT04"));			// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�a�{�^���F�W�����v");								// �W�����v����
				break;

			case OPR_INSTR_12:
				pTutoUI[0]->BindTexture(pTex->GetTexture("TEX_TYPE_PAD_SELECT05"));			// �e�N�X�`���擾
				KeyExpCreate(&m_pKeyExp, "�`�{�^���F����E���e������������");				// ���e��������
				break;
			}
		}

		// �`���[�g���A�����I����
		if (CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			// �L�[����������j��
			if (m_pKeyExp)
			{
				m_pKeyExp->Uninit();
				m_pKeyExp = nullptr;
			}

			// �`���[�g���A��UI�̔j��
			if (pTutoUI[0])
			{
				pTutoUI[0]->Uninit();
				pTutoUI[0] = nullptr;
			}

			// �X�N���[��UI�̔j��
			for (int nCnt = 0; nCnt < MAX_SCROLL_UI; nCnt++)
			{
				if (pScr[nCnt])
				{
					pScr[nCnt]->Uninit();
					pScr[nCnt] = nullptr;
				}
			}

			// �^�C�g���y�[�W2�ɖ߂�
			m_Page = PAGE_02;
			m_TutoPage = TUTORIAL_PAGE_01;
		}
		break;
	}

		// �X�N���[���pUI�̃X�N���[������
		for (int nCnt = 0; nCnt < MAX_SCROLL_UI; nCnt++)
		{
			if (pScr[nCnt])
			{
				switch (nCnt)
				{
				case 0:
					pScr[nCnt]->SetTex(SCROLL_SPEED, 0.0f);
					break;

				case 1:
					pScr[nCnt]->SetTex(-SCROLL_SPEED, 0.0f);
					break;
				}
			}
		}
	
}

//============================================
// �J�[�\���ړ�����
//============================================
void CTitle::CursorMove(int &nNowSel, const int &nFirst, const int &nLast, const bool &bMinus)
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

//============================================
// �L�[���������񐶐��֐�
//============================================
void CTitle::KeyExpCreate(CStringManager **ppKeyExp, const char *cStr,const int& nFontSize)
{
	// �����񂪎c���Ă���Ȃ�j�����Ă��琶��
	if (ppKeyExp[0])
	{
		ppKeyExp[0]->Uninit();
		ppKeyExp[0] = nullptr;
	}

	// �����񐶐�
	if (!ppKeyExp[0])
	{
		ppKeyExp[0] = CStringManager::Create(KEY_EXP_POS, static_cast<float>(nFontSize), nFontSize, cStr, KEY_EXP_FONTNAME);
	}
}

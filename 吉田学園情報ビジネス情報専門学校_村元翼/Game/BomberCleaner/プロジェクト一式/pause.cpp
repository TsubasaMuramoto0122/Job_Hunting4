//===================================================
// �|�[�Y�̏���
// Author:������
//===================================================
#include "pause.h"
#include "XInput.h"
#include "keyboard.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "texture.h"
#include "fade.h"
#include "bg.h"
#include <assert.h>

//===================================================
// �}�N����`
//===================================================
#define PAUSE_INERVAL (100.0f)							// �|�[�Y�C���^�[�o��
#define PAUSE_BG_ALPHA (0.8f)							// �|�[�Y�w�i�̃��l
#define SELECT_HEIGHT (300.0f)							// �I�����̍���
#define SELECT_SIZE (D3DXVECTOR3(300.0f,120.0f,0.0f))	// �I�����̃T�C�Y
#define CURSOR_SIZE (D3DXVECTOR3(50.0f, 80.0f, 0.0f))	// �J�[�\���̍���

//===================================================
// �ÓI�����o�ϐ��̏�����
//===================================================
CBg *CPause::m_pBg = nullptr;	// �w�i�N���X

//===================================================
// �R���X�g���N�^
//===================================================
CPause::CPause(OBJTYPE nPriority) :CScene(nPriority)
{
	// �����o�ϐ��̏�����
	memset(m_pSelect, 0, sizeof(m_pSelect));	// �|���S���N���X
	m_pCursor			= nullptr;				// �J�[�\��(�|���S��)�N���X
	m_pTutorialUI		= nullptr;				// UI�N���X
	m_bNextMode			= false;				// ���̃��[�h�ɍs�����̔���
	m_bUninit			= false;				// �I������
	m_bMoveCursor		= true;					// �J�[�\���ړ�����
	m_nPauseType		= 0;					// �|�[�Y�I����Ԃ��Ǘ�����ϐ�
	m_fAlpha			= 0.0f;					// ���l�������邽�߂̕ϐ�
}

//===================================================
// �f�X�g���N�^
//===================================================
CPause::~CPause()
{

}

//===================================================
// ������
//===================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�ݒ�
	m_pos = pos;
	CScene::SetPos(m_pos);

	// �|�[�Y�w�i�̐����ƃJ���[�̐ݒ�
	m_pBg = CBg::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false, CScene::OBJTYPE_PAUSE);
	m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, PAUSE_BG_ALPHA));

	// �e�N�X�`���N���X�擾
	CTexture *pTex = CManager::GetInstance()->GetTexture();

	// �I�����̃|���S������
	int nType = 0;
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (!m_pSelect[nCnt])
		{
			m_pSelect[nCnt] = CScene2D::Create
			(
				D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 
				SELECT_HEIGHT + (PAUSE_INERVAL * nCnt), 0.0f), 
				SELECT_SIZE,OBJTYPE_PAUSE
			);

			// �e�N�X�`���擾
			switch (nCnt)
			{
			case SELECT_RETURNGAME:
				m_pSelect[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE01"));
				break;

			case SELECT_RETRY:
				m_pSelect[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE02"));
				break;

			case SELECT_RETURNTITLE:
				m_pSelect[nCnt]->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE03"));
				break;
			}

		}
	}

	// �J�[�\���̃|���S������
	if (!m_pCursor)
	{
		m_pCursor = CScene2D::Create(m_pos, CURSOR_SIZE, OBJTYPE_PAUSE);
		m_pCursor->BindTexture(pTex->GetTexture("TEX_TYPE_PAUSE_CURSOR"));
	}

	return S_OK;
}

//===================================================
// �I��
//===================================================
void CPause::Uninit(void)
{
	// �I�����̔j��
	for (int nCnt = 0; nCnt < SELECT_MAX; nCnt++)
	{
		if (m_pSelect[nCnt])
		{
			m_pSelect[nCnt]->Uninit();
			m_pSelect[nCnt] = nullptr;
		}
	}

	// �J�[�\���j��
	if (m_pCursor)
	{
		m_pCursor->Uninit();
		m_pCursor = nullptr;
	}

	// �|�[�Y�w�i�̔j��
	if (m_pBg)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	CManager::GetInstance()->SetPause(false);	// �|�[�Y�̏�Ԃ�false�ɂ���
	CManager::GetInstance()->SetStop(false);	// �|�[�Y�̏�Ԃ�false�ɂ���
	Release();									// �I�u�W�F�N�g�̔j��
}

//===================================================
// �X�V
//===================================================
void CPause::Update(void)
{
	if (!m_bNextMode)
	{
		// ���͌n�N���X�擾
		CXInput *pXInput = CManager::GetInstance()->GetXInput();
		CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

		// �I������ɂ��炷
		if (CInput::PressAnyAction(CInput::ACTION_UP))
		{
			// �|�[�Y�̃^�C�v�����炷
			m_nPauseType--;
			if (m_nPauseType < 0)
			{
				m_nPauseType = SELECT_MAX - 1;
			}
			m_bMoveCursor = true;
		}

		// �I�������ɂ��炷
		if (CInput::PressAnyAction(CInput::ACTION_DOWN))
		{
			// �|�[�Y�̃^�C�v�����炷
			m_nPauseType++;
			if (m_nPauseType >= SELECT_MAX)
			{
				m_nPauseType = 0;
			}
			m_bMoveCursor = true;
		}

		// �J�[�\���̈ʒu��ς���
		if (m_bMoveCursor)
		{
			if (m_pCursor)
			{
				switch (m_nPauseType)
				{
				case SELECT_RETURNGAME:		// �Q�[����ʂɖ߂�
					m_pCursor->SetPos(D3DXVECTOR3(m_pSelect[0]->GetPos().x - (m_pSelect[0]->GetSize().x), m_pSelect[0]->GetPos().y, 0.0f), m_pCursor->GetSize());
					break;

				case SELECT_RETRY:			// ���g���C
					m_pCursor->SetPos(D3DXVECTOR3(m_pSelect[1]->GetPos().x - (m_pSelect[1]->GetSize().x), m_pSelect[1]->GetPos().y, 0.0f), m_pCursor->GetSize());
					break;

				case SELECT_RETURNTITLE:	// �^�C�g���ɖ߂�
					m_pCursor->SetPos(D3DXVECTOR3(m_pSelect[2]->GetPos().x - (m_pSelect[2]->GetSize().x), m_pSelect[2]->GetPos().y, 0.0f), m_pCursor->GetSize());
					break;
				}

				m_bMoveCursor = false;
			}
		}

		// �I�������肷��
		if ((pKey->GetTrigger(DIK_RETURN) || pXInput->GetButtonTrigger(XINPUT_GAMEPAD_A)))
		{
			switch (m_nPauseType)
			{
			case SELECT_RETURNGAME:		// �Q�[���ɖ߂�
				CManager::GetInstance()->SetPause(false);	// �I�u�W�F�N�g��~��Ԃ�false�ɂ���
				CManager::GetInstance()->SetStop(false);	// �|�[�Y�j����ON�ɂ���
				break;
			case SELECT_RETRY:			// ���g���C
				CFade::SetFade(CManager::MODE::GAME);
				break;
			case SELECT_RETURNTITLE:	// �^�C�g�����
				CFade::SetFade(CManager::MODE::TITLE);
				break;
			}
			m_bNextMode = true;		// ���[�v�h�~�̂��߂�true�ɂ���
		}
	}
}

//===================================================
// �`��
//===================================================
void CPause::Draw(void)
{

}

//===================================================
// �C���X�^���X��������
//===================================================
CPause *CPause::Create(D3DXVECTOR3 pos)
{
	// �C���X�^���X����
	CPause *pPause = nullptr;
	if (!pPause)
	{
		pPause = new CPause(OBJTYPE_PAUSE);			// �������m��
		if (pPause)
		{
			pPause->Init(pos, {0.0f,0.0f,0.0f});	// ������
		}
	}

	return pPause;
}

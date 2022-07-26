//================================================
// manager.cpp�̍쐬
// Author:������
//================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "keyboard.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "XInput.h"
#include "DirectInput.h"
#include "playdata.h"
#include "LoadX.h"
#include "pause.h"
#include "mouse.h"
#include "sound.h"

//================================================
// �ÓI�����o�ϐ��̏�����
//================================================
CManager*		CManager::s_pManager				= nullptr;		// �V���O���g���}�l�[�W��
CRenderer*		CManager::m_pRenderer				= nullptr;		// �����_���[
CCamera*		CManager::m_apCamera[MAX_CAMERA]	= { nullptr };	// �J����
CLight*			CManager::m_pLight[MAX_LIGHT]		= { nullptr };	// ���C�g
CTexture*		CManager::m_pTexture				= nullptr;		// �e�N�X�`��
CInputkeyboard* CManager::m_pInputKeyboard			= nullptr;		// �L�[�{�[�h
CXInput*		CManager::m_pXInput					= nullptr;		// XInput
CDInput*		CManager::m_pDInput					= nullptr;		// DInput
CTitle*			CManager::m_pTitle					= nullptr;		// �^�C�g��
CGame*			CManager::m_pGame					= nullptr;		// �`���[�g���A��
CResult*		CManager::m_pResult					= nullptr;		// ���U���g
CPause*			CManager::m_pPause					= nullptr;		// �|�[�Y
CFade*			CManager::m_pFade					= nullptr;		// �t�F�[�h
CPlayData*		CManager::m_pPlayData				= nullptr;		// �v���C�f�[�^
CLoadX*			CManager::m_pLoadX					= nullptr;		// X�ǂݍ��݃N���X
CMouse*			CManager::m_pMouse					= nullptr;		// �}�E�X�N���X
CSound*			CManager::m_pSound					= nullptr;		// �T�E���h�N���X
bool			CManager::m_bOnlyOnce				= false;		// ��x�̂ݒʂ锻��
bool			CManager::m_bStop					= false;		// �X�g�b�v����
bool			CManager::m_bPause					= false;		// �|�[�Y����
HWND			CManager::m_hWnd					= nullptr;		// �E�B���h�E�n���h��

#ifdef _DEBUG
CManager::MODE	CManager::m_mode					= MODE::GAME;	// �f�o�b�O���������[�h
#else
CManager::MODE	CManager::m_mode					= MODE::TITLE;	// �����[�X���������[�h
#endif

//================================================
// �R���X�g���N�^
//================================================
CManager::CManager()
{

}

//================================================
// �f�X�g���N�^
//================================================
CManager::~CManager()
{

}

//================================================
// �}�l�[�W��������
//================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bReturn)
{
	m_hWnd = hWnd;	// �E�B���h�E�n���h���̕ۑ�

	// �����_���[�̐���
	if (!m_pRenderer)
	{
		m_pRenderer = new CRenderer;
		if (m_pRenderer)
		{
			m_pRenderer->Init(hWnd, bReturn);
		}
	}

	//�v���C�f�[�^�N���X�̐���
	if (!m_pPlayData)
	{
		m_pPlayData = new CPlayData;

		if (m_pPlayData)
		{
			m_pPlayData->Init();
		}
	}

	// X�ǂݍ��݃N���X�̐���
	if (!m_pLoadX)
	{
		m_pLoadX = new CLoadX;

		if (m_pLoadX)
		{
			m_pLoadX->Init();
		}
	}

	// �L�[�{�[�h�̐���
	if (!m_pInputKeyboard)
	{
		m_pInputKeyboard = new CInputkeyboard;
		if (m_pInputKeyboard)
		{
			m_pInputKeyboard->Init(hInstance, hWnd);
		}
	}

	// Xinput����
	if (!m_pXInput)
	{
		m_pXInput = new CXInput;
	}

	// Dinput����
	if (!m_pDInput)
	{
		m_pDInput = new CDInput;
		if (m_pDInput)
		{
			m_pDInput->Init(hInstance, hWnd);
		}
	}

	// �}�E�X����
	if (!m_pMouse)
	{
		if (m_pMouse = new CMouse)
		{
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// ���C�g�̐���
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (!m_pLight[nCnt])
		{
			m_pLight[nCnt] = new CLight;
		}
	}

	m_pLight[0]->Init({ 0.0f, 0.0f, 0.0f	},	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.2f, -0.8f, -0.4f });
	m_pLight[1]->Init({ 0.0f, 0.0f, 0.0f	},	{ 1.0f, 1.0f, 1.0f, 1.0f }, { -0.2f, -0.8f, 0.4f });
	m_pLight[2]->Init({ 0.0f, 0.0f, 0.0f	},	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.4f, 0.0f	});
	m_pLight[3]->Init({ 0.0f, 0.0f, -100.0f },	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, -1.0f });

	// �e�N�X�`���̐���
	if (!m_pTexture)
	{
		m_pTexture = new CTexture;
		if (m_pTexture)
		{
			m_pTexture->Init();
		}
	}

	// �t�F�[�h�̐���
	if (!m_pFade)
	{
		m_pFade = CFade::Create(m_mode);
	}

	// �T�E���h�̐���
	if (!m_pSound)
	{
		if (m_pSound = new CSound)
		{
			m_pSound->InitSound();
		}
	}

	// ���[�h�̐ݒ�
	SetMode(m_mode);

	return S_OK;
}

//================================================
// �}�l�[�W���I��
//================================================
void CManager::Uninit(void)
{
	//-----------------------------------------------
	// �S�Ă̏I������
	//-----------------------------------------------
	CScene::ReleaseAll();

	//*******************************//
	//  �e��I�u�W�F�N�g�̏I������   //
	//*******************************//
	// �T�E���h�̔j��
	if (m_pSound)
	{
		m_pSound->UninitSound();
		delete m_pSound;
		m_pSound = nullptr;
	}

	// �e�N�X�`���̔j��
	if (m_pTexture)
	{
		m_pTexture->Uninit();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// �t�F�[�h�̔j��
	if (m_pFade)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	// ���C�g�j��
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (m_pLight[nCnt])
		{
			delete m_pLight[nCnt];
			m_pLight[nCnt] = nullptr;
		}
	}

	// �J�����̔j��
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_apCamera[nCnt])
		{
			m_apCamera[nCnt]->Uninit();
			delete m_apCamera[nCnt];
			m_apCamera[nCnt] = nullptr;
		}
	}

	// �����_���[�j��
	if (m_pRenderer)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	// �L�[�{�[�h�̔j��
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	// DInput�̔j��
	if (m_pDInput)
	{
		m_pDInput->Uninit();
		delete m_pDInput;
		m_pDInput = nullptr;
	}

	// �}�E�X�̔j��
	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}

	// �v���C�f�[�^�̔j��
	if (m_pPlayData)
	{
		m_pPlayData->Uninit();
		delete m_pPlayData;
		m_pPlayData = nullptr;
	}

	// X�ǂݍ��݃N���X�̔j��
	if (m_pLoadX)
	{
		m_pLoadX->Uninit();
		delete m_pLoadX;
		m_pLoadX = nullptr;
	}

	// �V���O���g���}�l�[�W���̔j��
	if (s_pManager)
	{
		delete s_pManager;
		s_pManager = nullptr;
	}
}

//================================================
// �}�l�[�W���X�V
//================================================
void CManager::Update(void)
{
	// �����_���[�̍X�V
	if (m_pRenderer)
	{
		m_pRenderer->Update();
	}

	//�L�[�{�[�h�̍X�V
	if (m_pInputKeyboard)
	{
		m_pInputKeyboard->Update();
	}

	// �Q�[���p�b�h�̍X�V
	if (m_pXInput)
	{
		m_pXInput->UpdateGamepad();
	}

	// DInput�̍X�V
	if (m_pDInput)
	{
		m_pDInput->Update();
	}

	// �}�E�X�̍X�V
	if (m_pMouse)
	{
		m_pMouse->Update();
	}

	// �X�g�b�v���̓J�����͍X�V���Ȃ�����
	if (!m_bStop && !m_bPause)
	{
		// �J�����̍X�V����
		for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
		{
			if (m_apCamera[nCnt])
			{
				m_apCamera[nCnt]->Update();
			}
		}
	}

	//*******************************//
	//		�@���[�h�X�V����		 //
	//*******************************//
	switch (m_mode)
	{
	case MODE::TITLE:		// �^�C�g��
		if (m_pTitle)
		{
			m_pTitle->Update();
		}
		break;

	case MODE::GAME:		// �Q�[��
		if (m_pGame)
		{
			m_pGame->Update();

			if (!m_bStop)
			{
				// �|�[�Y�؂�ւ�(P������)
				if (m_pInputKeyboard->GetTrigger(DIK_P) || m_pXInput->GetButtonTrigger(XINPUT_GAMEPAD_START))
				{
					m_bPause = !m_bPause;		// �|�[�Y��؂�ւ���
				}

				if (m_bPause)
				{
					// �|�[�Y�̐���
					if (!m_pPause)
					{
						// �}�E�X�J�[�\���̕\��
						ShowCursor(true);
						m_pPause = CPause::Create({ 0.0f, 0.0f, 0.0f });
					}
				}

				else
				{
					// �|�[�Y�̔j��
					if (m_pPause)
					{
						// �}�E�X�J�[�\���̏���
						ShowCursor(false);
						// delete��Scene�ōs��
						m_pPause->Uninit();
						m_pPause = nullptr;
					}
				}

			}
		}
		break;

	case MODE::RESULT:	// ���U���g
		if (m_pResult)
		{
			m_pResult->Update();
		}
		break;
	}
}

//================================================
// �}�l�[�W���`��
//================================================
void CManager::Draw(void)
{
	// �����_���[�`��
	if (m_pRenderer)
	{
		m_pRenderer->Draw();
	}

	// �J�����`��
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_apCamera[nCnt])
		{
			m_apCamera[nCnt]->SetCamera();
		}
	}
}

//================================================
// �C���X�^���X�擾����
//================================================
CManager *CManager::GetInstance(void)
{
	if (!s_pManager)
	{
		s_pManager = new CManager;
	}
	return s_pManager;
}

//================================================
// ���[�h�̃Z�b�g
//================================================
void CManager::SetMode(MODE mode)
{
	//-----------------------------------------------
	// ���݂̃��[�h��j��(Uninit��Scene.cpp�ōs��)
	//-----------------------------------------------
	switch (m_mode)
	{
	case MODE::TITLE:		// �^�C�g��
		if (m_pTitle)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = nullptr;
		}
		break;

	case MODE::GAME:		// �Q�[��
		if (m_pGame)
		{
			// �J�����̔j��
			for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
			{
				if (m_apCamera[nCnt])
				{
					m_apCamera[nCnt]->Uninit();
					delete m_apCamera[nCnt];
					m_apCamera[nCnt] = nullptr;
				}
			}

			// �|�[�Y�̔j��
			if (m_pPause)
			{
				m_pPause->Uninit();
				m_pPause = nullptr;
			}

			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = nullptr;
		}
		break;

	case MODE::RESULT:		// ���U���g
		if (m_pResult)
		{
			m_pResult->Uninit();
			delete m_pResult;
			m_pResult = nullptr;
		}
		break;
	}

	// �T�E���h��~
	m_pSound->StopSound();

	//----------------------------------
	// �S�ẴI�u�W�F�N�g�̔j��
	//----------------------------------
	CScene::ReleaseAll();


	//----------------------------------
	// ���̃��[�h�ɂ���
	//----------------------------------
	switch (mode)
	{
	case MODE::TITLE:		// �^�C�g��
		if (!m_pTitle)
		{
			m_pTitle = new CTitle;
			if (m_pTitle)
			{
				m_pTitle->Init();
				m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);
				m_pSound->ControllVoice(CSound::SOUND_LABEL_BGM_TITLE, 0.5f);
			}
		}
		break;

	case MODE::GAME:		// �Q�[��
		if (!m_pGame)
		{
			m_pGame = new CGame;
			if (m_pGame)
			{
				// �J�����̐���
				for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
				{
					m_apCamera[nCnt] = CCamera::Create(
										D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI*-0.3f, 0.0f, 0.0f),
										(float)(SCREEN_WIDTH / MAX_CAMERA * nCnt), 0.0f,
										(float)(SCREEN_WIDTH / MAX_CAMERA), (float)SCREEN_HEIGHT, nCnt);
				}
				ShowCursor(false);
				m_pGame->Init();
				m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);
				m_pSound->ControllVoice(CSound::SOUND_LABEL_BGM_GAME, 0.5f);

			}
		}
		break;

	case MODE::RESULT:		// ���U���g
		if (!m_pResult)
		{
			m_pResult = new CResult;
			if (m_pResult)
			{
				ShowCursor(true);
				m_pResult->Init();
				m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RESULT);
				m_pSound->ControllVoice(CSound::SOUND_LABEL_BGM_RESULT, 0.5f);
			}
		}
		break;
	}

	// ���̃��[�h�����݂̃��[�h�ɂ���
	m_mode = mode;
	m_bOnlyOnce = false;
}

//================================================
// ���[�h�̃Z�b�g����
//================================================
bool CManager::SetModeBool(MODE modeNext)
{
	//=========================================================
	// ��ʑJ��(ENTER�܂���START�{�^��������)
	//=========================================================
	if ((CInput::PressAnyAction(CInput::ACTION_ENTER) || CInput::PressAnyAction(CInput::ACTION_ATTACK)) && !m_bOnlyOnce)
	{
		m_pFade->SetFade(modeNext);						// �Q�[�����[�h��
		m_bOnlyOnce = true;								// ENTER�A�Ŗh�~
		return true;
	}

	return false;
}

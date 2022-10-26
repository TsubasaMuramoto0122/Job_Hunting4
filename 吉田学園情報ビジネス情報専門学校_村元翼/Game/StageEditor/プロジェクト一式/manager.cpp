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
#include "game.h"
#include "fade.h"
#include "XInput.h"
#include "DirectInput.h"
#include "LoadX.h"
#include "mouse.h"

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
CGame*			CManager::m_pGame					= nullptr;		// �Q�[��
CFade*			CManager::m_pFade					= nullptr;		// �t�F�[�h
CLoadX*			CManager::m_pLoadX					= nullptr;		// X�ǂݍ��݃N���X
CMouse*			CManager::m_pMouse					= nullptr;		// �}�E�X�N���X
bool			CManager::m_bOnlyOnce				= false;		// ��x�̂ݒʂ锻��
HWND			CManager::m_hWnd					= nullptr;		// �E�B���h�E�n���h��
CManager::MODE	CManager::m_mode					= MODE::GAME;	// �f�o�b�O���������[�h

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

	// �J�����̍X�V����
	for (int nCnt = 0; nCnt < MAX_CAMERA; nCnt++)
	{
		if (m_apCamera[nCnt])
		{
			m_apCamera[nCnt]->Update();
		}
	}

	if (m_pGame)
	{
		m_pGame->Update();
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

		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}

	//----------------------------------
	// �S�ẴI�u�W�F�N�g�̔j��
	//----------------------------------
	CScene::ReleaseAll();

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
			m_pGame->Init();
		}
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

//=========================================================
//�Q�[����ʂ̏���
//=========================================================
#include "game.h"
#include "fade.h"
#include "manager.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "timercount.h"
#include "stage.h"
#include "score.h"
#include "Scene3D.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "Bomb.h"
#include "BombSpawner.h"
#include "LoadX.h"
#include "Ui.h"
#include "sound.h"
#include "playdata.h"
#include "StringManager.h"

//=========================================================
// �}�N����`
//=========================================================
#define COUNT_DOWN_FRAME	(60)										// �J�E���g�_�E���̃t���[��
#define SKY_SIZE			(D3DXVECTOR3(10000.0f,10000.0f,10000.0f))	// ��T�C�Y
#define UI_SIZE				(D3DXVECTOR3(1.0f,1.0f,0.0f))				// UI�T�C�Y
#define NUMBER_SIZE			(D3DXVECTOR3(50.0f, 30.0f, 0.0f))			// �X�R�A�T�C�Y
#define FADE_FRAME			(90)										// �I�����Ă��烊�U���g�Ƀt�F�[�h����܂ł̃t���[��
#define GAME_TIME			(60)										// �Q�[���^�C��

//=========================================================
// �ÓI�����o�ϐ��̏�����
//=========================================================

//=========================================================
// �R���X�g���N�^
//=========================================================
CGame::CGame()
{
	m_nFrame			= 0;		// �t���[��
	m_pScore			= nullptr;	// �X�R�A�N���X
	m_pAddScore			= nullptr;	// �ǉ��X�R�A�N���X
	m_pTimer			= nullptr;	// �^�C�}�[�N���X
	m_pMeshCylinder		= nullptr;	// ���b�V����
	m_pMeshField		= nullptr;	// ���b�V����
	m_pMeshSphere		= nullptr;	// ���b�V����
	m_pMeshWall			= nullptr;	// ���b�V����
	m_pStage			= nullptr;	// �X�e�[�W�N���X
	m_pUI				= nullptr;	// UI�N���X
	m_bOnce				= false;	// ��x�̂ݒʂ�ϐ�
	m_bStart			= false;	// �X�^�[�g����ϐ�
	m_bEnd				= false;	// �G���h����ϐ�
	m_bFrameCount		= false;	// �t���[���J�E���g�ϐ�
}

//=========================================================
// �f�X�g���N�^
//=========================================================
CGame::~CGame()
{

}

//=========================================================
// ������
//=========================================================
HRESULT CGame::Init(void)
{
	// �󐶐�
	if (!m_pMeshSphere)
	{
		m_pMeshSphere = CMeshSphere::Create({ 0.0f,0.0f,0.0f }, SKY_SIZE, { 0.0f,0.0f,0.0f }, 10, 10, true, CScene::OBJTYPE_MESH);
		m_pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_SKY"));
	}

	// �X�e�[�W����
	if (!m_pStage)
	{
		m_pStage = new CStage;
		if (m_pStage)
		{
			m_pStage->Init();									// ������
			m_pStage->SetStage("data/TEXT/StageSet001.txt");	// ���f���ݒ�̃t�@�C����
		}
	}

#ifdef _DEBUG
	// �^�C�}�[����
	if (!m_pTimer)
	{
		m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), NUMBER_SIZE, 10, false);
	}
	//m_pMeshField = CMeshField::Create({ 100.0f,100.0f,100.0f }, { 300.0f,0.0f,300.0f }, { 0.0f,0.0f,0.0f }, 6, 6);
	//m_pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_CAUTION"));

	CBombSpawner::Create({ 800.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, 5);

	CBomb::Create({ 0.0f,	10.0f, -300.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -300.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -300.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 0.0f,	10.0f, -400.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ 100.0f, 10.0f, -400.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
	CBomb::Create({ -100.0f,10.0f, -400.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
#else
	// �^�C�}�[����
	if (!m_pTimer)
	{
		m_pTimer = CTimerCount::Create(D3DXVECTOR3(SCREEN_WIDTH - 250.0f, 50.0f, 0.0f), NUMBER_SIZE, GAME_TIME, false);
	}
#endif

	// �X�R�A����
	if (!m_pScore)
	{
		m_pScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, SCREEN_HEIGHT - 50.0f, 0.0f), NUMBER_SIZE);
	}

	return S_OK;
}

//=========================================================
// �I��
//=========================================================
void CGame::Uninit(void)
{
	CManager::GetPlayData()->SetScore(m_pScore->GetScore());	// �v���C�f�[�^�ɃX�R�A��ݒ�

	// �X�g�b�v����
	if (CManager::GetInstance()->GetStop())
	{
		CManager::GetInstance()->SetStop(false);
	}

	//------------------------------------
	// �I�u�W�F�N�g��j��
	//------------------------------------
	// �X�R�A
	if (m_pScore)
	{
		m_pScore = nullptr;
	}

	// �ǉ��X�R�A
	if (m_pAddScore)
	{
		m_pAddScore = nullptr;
	}

	// ���b�V���t�B�[���h
	if (m_pMeshField)
	{
		m_pMeshField->Uninit();
		m_pMeshField = nullptr;
	}

	// ���b�V���E�H�[��
	if (m_pMeshWall)
	{
		m_pMeshWall->Uninit();
		m_pMeshWall = nullptr;
	}

	// ���b�V���X�t�B�A
	if (m_pMeshSphere)
	{
		m_pMeshSphere->Uninit();
		m_pMeshSphere = nullptr;
	}

	// ���b�V���V�����_�[
	if (m_pMeshCylinder)
	{
		m_pMeshCylinder->Uninit();
		m_pMeshCylinder = nullptr;
	}

	// �^�C�}�[
	if (m_pTimer)
	{
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	// �X�e�[�W
	if (m_pStage)
	{
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	// UI
	SetUiDelete();
}

//=========================================================
// �X�V
//=========================================================
void CGame::Update(void)
{
	//----------------------------------------------------
	// �J�n�J�E���g�_�E��
	//----------------------------------------------------
#ifndef _DEBUG
	if (!m_bStart)
	{
		StartGame();
	}
#endif

	//----------------------------------------------------
	// �I���T�C��
	//----------------------------------------------------
	if (m_bEnd)
	{
		EndGame();
	}

	else
	{
		if (m_pTimer->GetLimit())
		{
			m_bEnd = true;
		}

		// �S�ẴI�u�W�F�N�g��j�󂵏I������
		CScene *pScene = CScene::GetScene(CScene::OBJTYPE_MODEL);
		if (!pScene)
		{
			m_bEnd = true;
		}

#ifdef _DEBUG
		if (CInput::PressAnyAction(CInput::ACTION_ENTER))
		{
			m_bEnd = true;
		}
#endif
	}
}
//=========================================================
// �`��
//=========================================================
void CGame::Draw(void)
{

}

//=========================================================
// �J�n����
//=========================================================
void CGame::StartGame(void)
{
	// �T�E���h�N���X�擾
	CSound *pSound = CManager::GetInstance()->GetSound();

	// �t���[��0�̎�
	if (m_nFrame == 0)
	{
		CManager::GetInstance()->SetStop(true);
		m_pTimer->SetStop(true);
	}

	// �t���[���J�E���g
	m_nFrame++;		

	// �J�E���g�R
	if (m_nFrame == COUNT_DOWN_FRAME)
	{
		SetUiCreate("TEX_TYPE_NUMBER_THREE", 1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);		// �J�E���g��
	}

	// �J�E���g�Q
	else if (m_nFrame == COUNT_DOWN_FRAME * 2)
	{
		SetUiCreate("TEX_TYPE_NUMBER_TWO", 1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);		// �J�E���g��
	}

	// �J�E���g�P
	else if (m_nFrame == COUNT_DOWN_FRAME * 3)
	{
		SetUiCreate("TEX_TYPE_NUMBER_ONE", 1);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_COUNT);		// �J�E���g��
	}

	// �J�E���g�O
	else if (m_nFrame == COUNT_DOWN_FRAME * 4)
	{
		SetUiCreate("TEX_TYPE_GAMESTART", 3);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_GAMESTART);	// �X�^�[�g��
	}

	// �Q�[���X�^�[�g
	else if (m_nFrame == COUNT_DOWN_FRAME * 5)
	{
		SetUiDelete();
		m_nFrame = 0;
		m_bStart = true;
		CManager::GetInstance()->SetStop(false);
		m_pTimer->SetStop(false);
	}
}

//=========================================================
// �I������
//=========================================================
void CGame::EndGame(void)
{
	CTexture *pTex = CManager::GetInstance()->GetTexture();
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (!m_bOnce)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_TIMEUP);	// �I����

		// �I��UI����
		if (!m_pUI)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(pTex->GetTexture("TEX_TYPE_END"));
		}

		// �X�g�b�v����ON
		m_pTimer->SetStop(true);
		CManager::GetInstance()->SetStop(true);

		m_bOnce = true;
		m_bFrameCount = true;
	}

	// �t���[���J�E���g���s����
	if (m_bFrameCount)
	{
		m_nFrame++;	
	}

	if (m_nFrame == FADE_FRAME)
	{
		// �ǉ��X�R�A�̐���
		if (!m_pAddScore)
		{
			CStringManager::Create({ SCREEN_WIDTH / 2 - 200.0f,SCREEN_HEIGHT / 2 - 150.0f,0.0f }, 64, 64, "�c��^�C���~100","�l�r�S�V�b�N");
			m_pAddScore = CScore::Create({ SCREEN_WIDTH / 2 - NUMBER_SIZE.x * MAX_SCORE,SCREEN_HEIGHT / 2,0.0f }, NUMBER_SIZE * 2);
			m_pAddScore->SetScore(ADDSCORE_FORMER * m_pTimer->GetTimerCount());
		}
		SetUiDelete();
	}

	else if (m_nFrame == FADE_FRAME * 2)
	{
		// ���Z�X�R�A�̌v�Z���I���܂ň�x�J�E���g���~�߂�
		if (m_bFrameCount)
		{
			m_bFrameCount = false;
		}

		// �X�R�A	
		if (m_pAddScore->GetScore() > 0)
		{
			m_pAddScore->AddScore(-10);
			m_pScore->AddScore(10);
		}
		else
		{
			m_bFrameCount = true;	// �t���[���J�E���g��i�߂�
		}
	}

	else if (m_nFrame == FADE_FRAME * 3)
	{
		CManager::GetInstance()->GetFade()->SetFade(CManager::MODE::RESULT);
	}
}


//=========================================================
// UI�����֐�
//=========================================================
void CGame::SetUiCreate(const char* TexStr, float fSizeMulti)
{
	// �e�N�X�`���N���X�擾
	CTexture *pTex = CManager::GetInstance()->GetTexture();

	SetUiDelete();

	if (!m_pUI)
	{
		m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * fSizeMulti,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
		m_pUI->BindTexture(pTex->GetTexture(TexStr));
	}
}

//=========================================================
// UI�폜�֐�
//=========================================================
void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		m_pUI = nullptr;
	}
}






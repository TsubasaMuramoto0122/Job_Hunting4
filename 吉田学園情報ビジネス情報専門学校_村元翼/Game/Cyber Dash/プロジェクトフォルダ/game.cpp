//=========================================================
// �Q�[���V�[���̏���
// Author:������
//=========================================================
#include "game.h"
#include "texture.h"
#include "manager.h"
#include "fade.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshsphere.h"
#include "stage.h"
#include "score.h"
#include "Scene3D.h"
#include "field.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "LoadX.h"
#include "Ui.h"
#include "TextureFont.h"
#include "Goal.h"
#include "RemainCount.h"
#include "StringManager.h"
#include "ClearGauge.h"
#include "player.h"
#include "JumpCircle.h"
#include "sound.h"
#include "playdata.h"

//=========================================================
// �}�N����`
//=========================================================
#define START_FRAME		(60)									 // �X�^�[�g�t���[��
#define FONT_SIZE		(32)									 // �t�H���g�T�C�Y
#define UI_SIZE			(D3DXVECTOR3(1.0f,1.0f,0.0f))			 // UI�T�C�Y
#define SKY_SIZE		(D3DXVECTOR3(6000.0f,20000.0f,6000.0f))	 // ��̃T�C�Y
#define CYLINDER_ROT	(0.003f)								 // �V�����_�[��]��

//=========================================================
// �ÓI�����o�ϐ��̏�����
//=========================================================
CRemainCount *CGame::m_pRemainCount = nullptr;			// ���S�J�E���g
CScore		 *CGame::m_pScore = nullptr;				// �X�R�A
CStage		 *CGame::m_pStage = nullptr;				// �X�e�[�W
bool		  CGame::m_bRetry = false;					// ���g���C

//=========================================================
// �R���X�g���N�^
//=========================================================
CGame::CGame()
{
	m_nFrame		= 0;							// �t���[��
	m_CameraRot		= { 0.0f,0.0f,0.0f };			// �J������]��
	m_pMeshCylinder = nullptr;						// �V�����_�[
	m_pMeshField	= nullptr;						// �t�B�[���h
	m_pMeshSphere	= nullptr;						// �X�t�B�A
	m_pMeshWall		= nullptr;						// �E�H�[��
	m_pStage		= nullptr;						// �X�e�[�W
	m_pUI			= nullptr;						// UI
	m_bOnce			= false;						// ��x�̂ݒʂ点��ϐ�
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
	// �X�g�b�v����
	CManager::GetInstance()->SetStop(false);

	// �󐶐�
	m_pMeshCylinder = CMeshCylinder::Create({ 0.0f,0.0f,0.0f }, SKY_SIZE, { 0.0f,0.0f,0.0f }, 12, 12,CMeshCylinder::CYLINDER_TYPE::STAND_INSIDE);
	m_pMeshCylinder->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_CYBER"));

	// �X�e�[�W����	
	if (m_pStage = new CStage)
	{
		m_pStage->Init();									// ������
		m_pStage->SetStage("data/TEXT/StageSet000.txt");	// ���f���ݒ�̃t�@�C����
	}
	

	// �c�@�J�E���g����
	m_pRemainCount = CRemainCount::Create(SCREEN_CENTER, REMAIN_SIZE);

	// �N���A�Q�[�W
	CClearGauge::Create(GAUGE_POS, GAUGE_SIZE);

	// �e�N�X�`���t�H���g
#if _DEBUG
	CStringManager::Create(FONT_POS, (float)FONT_SIZE, FONT_SIZE, "�p�ŃJ�����؂�ւ�","�l�r�S�V�b�N");
	CStringManager::Create({ FONT_POS.x,FONT_POS.y + FONT_SIZE,FONT_POS.z }, (float)FONT_SIZE, FONT_SIZE, "�e�N�X�`���t�H���g" );
#endif

	return S_OK;
}

//=========================================================
// �I��
//=========================================================
void CGame::Uninit(void)
{
	// �X�e�[�W�I��
	if (m_pStage)
	{
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	// ���S�J�E���g�I��
	if (m_pRemainCount)
	{
		CPlayData *pPlayData = CManager::GetInstance()->GetPlayData();
		pPlayData->SetScore(m_pRemainCount->GetRemain());
		m_pRemainCount->Uninit();
		m_pRemainCount = nullptr;
	}
}

//=========================================================
// �X�V
//=========================================================
void CGame::Update(void)
{
	// �X�t�B�A��]
	if (m_pMeshSphere)
	{
		m_pMeshSphere->AddRot({ 0.0f,-CYLINDER_ROT,0.0f });
		m_pMeshSphere->CMesh::SetPos(m_pMeshSphere,m_pStage->GetPlayer()->GetPos(), m_pMeshSphere->GetSize());
	}

	// �w�i�V�����_�[�̉�]�E�ړ�
	if (m_pMeshCylinder)
	{
		m_pMeshCylinder->AddRot({ 0.0f,-CYLINDER_ROT,0.0f });
		if (!m_pStage->GetPlayer()->GetDeath())
		{
			m_pMeshCylinder->CMesh::SetPos(m_pMeshCylinder, { m_pStage->GetPlayer()->GetPos().x,0.0f, m_pStage->GetPlayer()->GetPos().z }, m_pMeshCylinder->GetSize());
		}
	}

#if (1)
	if (!m_bStart && !m_bEnd)
	{
		// �ŏ��͉�ʂ��X�g�b�v
		if (m_nFrame == 0)
		{
			CManager::GetInstance()->SetStop(true);
		}

		// �t���[���J�E���g
		m_nFrame++;

		// �X�^�[�g
		if (m_nFrame == START_FRAME)
		{
			m_pUI = CUi::Create(SCREEN_CENTER, { UI_SIZE.x * 3,UI_SIZE.y,0.0f }, CUi::UI_TYPE_ZOOM);
			m_pUI->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_GAMESTART"));
		}
		else if (m_nFrame == START_FRAME * 2)
		{
			SetUiDelete();
			m_nFrame = 0;
			m_bStart = true;
			CManager::GetInstance()->SetStop(false);
		}
		
	}
#endif
}
//=========================================================
// �`��
//=========================================================
void CGame::Draw(void)
{

}

//=========================================================
// UI�폜
//=========================================================
void CGame::SetUiDelete(void)
{
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}
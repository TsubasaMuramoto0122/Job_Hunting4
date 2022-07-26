//=============================================
// ���U���g��ʂ̏���
// Author:������
//=============================================
#include "result.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "ranking.h"
#include "fade.h"
#include "bg.h"
#include "Ui.h"
#include "XInput.h"
#include "DirectInput.h"
#include "score.h"
#include "sound.h"

//=============================================
// �}�N����`
//=============================================
#define RETURN_TITLE_TIME (120)									// �^�C�g���ɖ߂�UI���o��܂ł̃t���[��
#define RETURN_TITLE_POS  (D3DXVECTOR3(1150.0f,650.0f,0.0f))	// �^�C�g���ɖ߂�UI�̈ʒu
#define RETURN_TITLE_SIZE (D3DXVECTOR3( 450.0f,150.0f,0.0f))	// �^�C�g���ɖ߂�UI�̃T�C�Y

//=============================================
// �R���X�g���N�^
//=============================================
CResult::CResult()
{
	m_pReturnTitle = nullptr;
	m_pTitleLogo = nullptr;
	m_nFrame = 0;
	m_nSelectType = 0;
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
	// �w�i����
	CBg::Create();

	// �^�C�g�����S��\��
	m_pTitleLogo = CUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 100.0f, 0.0f), D3DXVECTOR3(800.0f, 200.0f, 0.0f), CUi::UI_TYPE_NONE);
	m_pTitleLogo->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_TITLE"));

	// �����L���O�̐���
	m_pRanking = CRanking::Create
	(
		D3DXVECTOR3(400.0f + RANKING_POLYGON_WIDTH * MAX_SCORE, RESULT_RANKING_POSY, 0.0f),
		D3DXVECTOR3(RANKING_POLYGON_WIDTH, RANKING_POLYGON_HEIGHT, 0.0f)
	);

	return S_OK;
}
//=============================================
// �I��
//=============================================
void CResult::Uninit(void)
{
	// �^�C�g�����SUI�j��
	if (m_pTitleLogo)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = nullptr;
	}

	// �^�C�g���ɖ߂�UI�̔j��
	if (m_pReturnTitle)
	{
		m_pReturnTitle->Uninit();
		m_pReturnTitle = nullptr;
	}

	// �����L���O�j��
	if (m_pRanking)
	{
		m_pRanking->Uninit();
		m_pRanking = nullptr;
	}
}

//=============================================
// �X�V
//=============================================
void CResult::Update(void)
{
	if (m_pRanking)
	{
		m_pRanking->Update();
	}

	// �^�C�g���ɖ߂��܂ŏ������Ԃ�u��
	if (m_nFrame >= RETURN_TITLE_TIME)
	{
		// �^�C�g���ɖ߂�UI��\��
		if (!m_pReturnTitle)
		{
			m_pReturnTitle = CUi::Create(RETURN_TITLE_POS, RETURN_TITLE_SIZE, CUi::UI_TYPE_BLINK);
			m_pReturnTitle->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_RETURN_TITLE"));
		}
		if (CManager::SetModeBool(CManager::MODE::TITLE))	// �Q�[�����[�h�J��
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);	// ���艹
		}
	}

	else
	{
		m_nFrame++;
	}

	// �f�o�b�O����R�������ƃ����L���O�����Z�b�g�����
#ifdef _DEBUG
	if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_R))
	{
		m_pRanking->DeleteRanking();
	}
#endif
}

//=============================================
// �`��
//=============================================
void CResult::Draw(void)
{
	if (m_pRanking)
	{
		m_pRanking->Draw();
	}
}


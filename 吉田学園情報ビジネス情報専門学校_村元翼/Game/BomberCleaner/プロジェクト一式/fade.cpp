//===================================================================
// �t�F�[�h�̏���
// Author:������
//===================================================================
#include "fade.h"
#include "bg.h"
#include "texture.h"
#include "manager.h"

//===================================================================
// �ÓI�����o�ϐ��錾
//===================================================================
CBg							*CFade::m_pBg		= nullptr;					// �w�i
CFade::FADE					CFade::m_fade		= CFade::FADE_IN;			// �t�F�[�h���
CManager::MODE				CFade::m_modeNext	= CManager::MODE::TITLE;	// ���̃��[�h���
float						CFade::m_colorAlpha = 1.0f;						// ���l

//===================================================================
// �R���X�g���N�^
//===================================================================
CFade::CFade(OBJTYPE nPriority) :CScene(nPriority)
{
	
}

//===================================================================
// �f�X�g���N�^
//===================================================================
CFade::~CFade()
{

}

//===================================================================
// �C���X�^���X��������
//===================================================================
CFade *CFade::Create(CManager::MODE modeNext,D3DXVECTOR3 pos,D3DXVECTOR3 size)
{
	//�C���X�^���X����
	CFade *pFade = nullptr;

	if (!pFade)
	{
		pFade = new CFade(OBJTYPE_FADE);
		if (pFade)
		{
			pFade->m_modeNext = modeNext;								// ���Ɉڂ郂�[�h
			pFade->Init(pos,size);										// ������
		}
	}
	return pFade;
}

//===================================================================
// ������
//===================================================================
HRESULT CFade::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �w�i����
	m_pBg = CBg::Create(pos, size, false, CScene::OBJTYPE_FADE);

	return S_OK;
}

//===================================================================
// �I��
//===================================================================
void CFade::Uninit(void)
{
	// �w�i�I��
	if (m_pBg)
	{
		m_pBg->Uninit();
		delete m_pBg;
		m_pBg = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//===================================================================
// �t�F�[�h�̍X�V����
//===================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		// �J���[�ݒ�
		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_colorAlpha));

		// �t�F�[�h��ʂ�����
		if (m_fade == FADE_IN)
		{
			m_colorAlpha -= FADE_IN_SPEED;
			if (m_colorAlpha <= 0.0f)
			{
				m_colorAlpha = 0.0f;
				m_fade = FADE_NONE;
			}

		}

		// �t�F�[�h��ʂ��o��
		else if (m_fade == FADE_OUT)
		{
			m_colorAlpha += FADE_OUT_SPEED;

			if (m_colorAlpha >= 1.0f)
			{
				m_colorAlpha = 1.0f;
				m_fade = FADE_IN;

				//���[�h�̐ݒ�
				CManager::SetMode(m_modeNext);
			}
		}
	}
}

//===================================================================
// �`��
//===================================================================
void CFade::Draw(void)
{

}

//===================================================================
// �t�F�[�h�̐ݒ菈��
//===================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	m_fade = FADE_OUT;
	m_modeNext = modeNext;
	m_colorAlpha = 0.0f;
}

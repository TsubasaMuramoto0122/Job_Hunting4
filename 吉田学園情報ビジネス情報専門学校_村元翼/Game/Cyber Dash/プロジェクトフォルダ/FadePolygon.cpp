//===============================================
// �t�F�[�h�p�|���S��
// Author:������
//===============================================
#include "FadePolygon.h"
#include "texture.h"
#include "manager.h"

//===============================================
// �ÓI�����o�ϐ��̏�����
//===============================================
const float CFadePolygon::m_fFadeInSpeed	= 0.02f;	// �t�F�[�h�C�����x
const float CFadePolygon::m_fFadeOutSpeed	= 0.01f;	// �t�F�[�h�A�E�g���x

//===============================================
// �R���X�g���N�^
//===============================================
CFadePolygon::CFadePolygon(float fFadeSize, OBJTYPE nPriority) : CScene2D(nPriority),
																 m_fInitFadeSize(fFadeSize)	// �ϐ�������
{
	m_fFadeSize = INIT_FADE_SIZE;		// �����t�F�[�h�T�C�Y
	m_pScene2D = nullptr;				// 2D�|���S��
	m_pos		= { 0.0f,0.0f,0.0f };	// �ʒu
	m_size		= { 0.0f,0.0f,0.0f };	// �T�C�Y
	m_fAlpha	= 0.0f;					// ���l
	m_bFadeIn	= false;				// �t�F�[�h�C������
}

//===============================================
// �f�X�g���N�^
//===============================================
CFadePolygon::~CFadePolygon()
{

}

//===============================================
// �I�u�W�F�N�g����
//===============================================
CFadePolygon *CFadePolygon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, float fInitFadeSize)
{
	// �I�u�W�F�N�g����
	CFadePolygon *pFadePolygon = nullptr;
	if (pFadePolygon = new CFadePolygon(fInitFadeSize))
	{
		pFadePolygon->Init(pos, size);
	}

	return pFadePolygon;
}

//===============================================
// ������
//===============================================
HRESULT CFadePolygon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�E�T�C�Y�擾
	m_pos = pos;
	m_size = size;

	// 2D�|���S���̐���
	if (!m_pScene2D)
	{
		m_pScene2D = CScene2D::Create(m_pos, m_size);
		m_pScene2D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_ROUNDFRAME"));
		m_pScene2D->SetCol({ 1.0f,1.0f,1.0f,0.0f });
	}

	return S_OK;
}

//===============================================
// �I��
//===============================================
void CFadePolygon::Uninit(void)
{
	// 2D�|���S���̔j��
	if (m_pScene2D)
	{
		m_pScene2D->Uninit();
		m_pScene2D = nullptr;
	}

	// ���̃I�u�W�F�N�g��j��
	Release();
}

//===============================================
// �X�V
//===============================================
void CFadePolygon::Update(void)
{
	// �t�F�[�h�����֐�
	Fade(m_fAlpha);
}

//===============================================
// �`��
//===============================================
void CFadePolygon::Draw(void)
{

}

//===============================================
// �t�F�[�h
//===============================================
void CFadePolygon::Fade(float& fAlpha)
{
	if (m_pScene2D)
	{
		// �J���[�E�T�C�Y�ݒ�
		m_pScene2D->SetCol({ 1.0f,1.0f,1.0f,fAlpha });
		m_pScene2D->SetPos(m_pos, m_size);

		// �t�F�[�h������
		if (m_bFadeIn)
		{
			m_fFadeSize += m_fInitFadeSize * 0.01f;	// �t�F�[�h�T�C�Y��傫��
			fAlpha -= m_fFadeInSpeed;				// ���l��������

			// ���X�ɃT�C�Y��������
			if (m_fFadeSize > 0.0f)
			{
				m_size.x -= m_fFadeSize;
				m_size.y -= m_fFadeSize;
			}

			// ���l0�ȉ��Ń|���S���j��
			if (fAlpha <= 0.0f)
			{
				fAlpha = 0.0f;
				Uninit();
			}

		}

		// �t�F�[�h���o��
		else
		{
			m_fFadeSize -= m_fInitFadeSize * 0.01f;	// �t�F�[�h�T�C�Y��������
			fAlpha += m_fFadeOutSpeed;				// ���l��傫��

			// ���X�ɃT�C�Y��傫��
			if (m_fFadeSize > 0.0f)
			{
				m_size.x += m_fFadeSize;
				m_size.y += m_fFadeSize;
			}

			// �t�F�[�h�C������on
			if (fAlpha >= 1.0f)
			{
				m_fFadeSize = 0.1f;
				fAlpha = 1.0f;
				m_bFadeIn = true;
			}
		}

	}
}
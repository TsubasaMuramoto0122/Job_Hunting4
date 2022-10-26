//========================================
// �����G�t�F�N�g(���b�V����)�̍쐬
// Author : ������
//========================================
#include "effect_blast.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "LoadX.h"
#include "meshsphere.h"
#include "score.h"
#include "game.h"

//===========================================
// �}�N����`
//===========================================
#define BLAST_SCALE_SPEED		(1.1f)	// �����X�P�[�����x
#define BLAST_ROLLING_SPEED		(0.5f)	// ������]���x
#define EXPLOSION_SCORE			(100)	// ���j�X�R�A
#define START_TRANSPARENT_FRAME (60)	// �����ɂȂ�n�߂�t���[����
#define ALPHA_SUB				(0.05f) // ���l������

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBlast::CBlast(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pSphere, 0, sizeof(m_pSphere));				// ���N���X
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_col			= D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// �F
	m_nFrame		= 0;									// �t���[����
	m_scale			= 0.0f;									// �X�P�[��
	m_fScalingSpeed = BLAST_SCALE_SPEED;					// �X�P�[���ύX���x
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBlast::~CBlast()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBlast::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;		// �ʒu
	m_size = size;		// �T�C�Y
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBlast::Uninit(void)
{
	// ���b�V�����j��
	if (m_pSphere)
	{
		// ���S����
		if (!m_pSphere->GetDeath())
		{
			m_pSphere->Uninit();
			m_pSphere = nullptr;
		}
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBlast::Update(void)
{
	// �������̏���
	if (m_pSphere)
	{
		Spread();	// �����L����
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBlast::Draw(void)
{

}

//=============================================================================
// �����̍L������̏���
//=============================================================================
void CBlast::Spread(void)
{
	// �l�N�X�g������
	float Nextsub = m_fScalingSpeed;

	// �t���[���J�E���g
	m_nFrame++;
	if (m_nFrame >= START_TRANSPARENT_FRAME)
	{
		m_col.a -= ALPHA_SUB;	// ���l��������
	}
	else
	{
		m_scale += m_fScalingSpeed;			// �T�C�Y��傫������
		m_fScalingSpeed -= Nextsub * 0.1f;	// �傫����ύX����X�s�[�h��10/1�ɂ���
	}

	m_rot.y += BLAST_ROLLING_SPEED;			// ��]����

	// ��]�E�傫���E�J���[��ݒ�
	m_pSphere->SetRot(m_rot);
	m_pSphere->SetScale({ m_scale,m_scale ,m_scale });
	m_pSphere->CMesh::SetCol(m_pSphere,m_col);

	// ���S�ɓ����ɂȂ�܂ŏI�����Ȃ�
	if (m_col.a <= 0.0f)
	{
		Uninit();
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CBlast *CBlast::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLine, int nVertical)
{
	// �I�u�W�F�N�g����
	CBlast *pBlast = nullptr;
	if (pBlast = new CBlast())
	{
		pBlast->m_col = col;					// �J���[
		pBlast->Init(pos, size);				// ������

		if (!pBlast->m_pSphere)
		{
			// ���e���f���̐���
			pBlast->m_pSphere = CMeshSphere::Create(pos, size, { 0.0f,0.0f,0.0f }, nLine, nVertical, false, CScene::OBJTYPE_EFFECTMESH);
		}

	}
	return pBlast;
}
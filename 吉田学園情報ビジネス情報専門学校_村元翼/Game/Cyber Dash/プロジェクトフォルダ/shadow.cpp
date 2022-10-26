//==============================================================================
// �t�B�[���h�̍쐬
// Author:������
//==============================================================================
#include "Scene3D.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "shadow.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CShadow::CShadow(OBJTYPE nPriority) : CScene3D(nPriority)
{
	// �e�평����
	m_pTexture = nullptr;							// �e�N�X�`���|�C���^
	m_pVtxBuff = nullptr;							// ���_�o�b�t�@�|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �T�C�Y
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ��]
	m_OriginSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���̃T�C�Y
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	// �C���X�^���X����
	CShadow *pShadow = nullptr;
	if (!pShadow)
	{
		// ���I�������m��
		pShadow = new CShadow();

		// NULL�`�F�b�N
		if (pShadow)
		{
			pShadow->m_pos = pos;			// �ʒu
			pShadow->m_size = size;			// �傫��
			pShadow->m_rot = rot;			// ��]
			pShadow->Init(pos, size);		// ������
		}
	}

	return pShadow;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_OriginSize = size;
	CScene3D::Init(pos, size);
	CScene3D::BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_EFFECT_MOVE"));
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CShadow::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CShadow::Update(void)
{
	m_pos.x = CScene::GetPosOld().x;
	m_pos.z = CScene::GetPosOld().z;
	float Y = CScene::GetPosOld().y;

	CScene::SetPosOld({ m_pos.x, Y, m_pos.z });
	CScene3D::Update();

	Y -= GRAVITY;
	CScene::SetPos({ m_pos.x, Y, m_pos.z });
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	CManager::GetInstance()->GetRenderer()->SetSubSynthesis(true, pDevice);

	CScene3D::Draw();

	CManager::GetInstance()->GetRenderer()->SetSubSynthesis(false, pDevice);
}

void CShadow::SetGravity(float GravitySpeed)
{
	m_pos.y -= GravitySpeed;
}
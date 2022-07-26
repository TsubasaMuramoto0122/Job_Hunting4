//========================================
// �^�[�Q�b�g����
// Author : ������
//========================================
#include "Target.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTarget::CTarget(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pModel, 0, sizeof(m_pModel));		// ���f���N���X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�P�[��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTarget::~CTarget()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTarget::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�E��]�ݒ�
	m_pos = pos;
	m_rot = rot;
	CScene::SetPos(m_pos);

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CTarget::Uninit(void)
{	
	// ���f���j��
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CTarget::Update(void)
{
	// ���f���X�V
	if (m_pModel)
	{
		m_pModel->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTarget::Draw(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���݂̃}�e���A�����擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	if (m_pModel)
	{
		m_pModel->Draw();
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CTarget *CTarget::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���X�^���X����
	CTarget *pTarget = nullptr;
	if (!pTarget)
	{
		pTarget = new CTarget();
		if (pTarget)
		{
			pTarget->m_scale = scale;
			pTarget->Init(pos, rot);		// ������

			// �p�x�����W�A���ɕϊ�
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			// ���f������
			if (!pTarget->m_pModel)
			{
				// ���e�X�|�i�[���f���̐���
				pTarget->m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
				// �e���f����NULL�ɐݒ�
				pTarget->m_pModel->SetParent(NULL);
				// ���f���^�C�v�̐ݒ�
				pTarget->CScene::SetModelType(CScene::MODTYPE_TARGET);
				// ���e�X�|�i�[�T�C�Y�̐ݒ�
				pTarget->SetSize(pTarget->m_pModel->GetSize());
			}
		}
	}
	return pTarget;
}
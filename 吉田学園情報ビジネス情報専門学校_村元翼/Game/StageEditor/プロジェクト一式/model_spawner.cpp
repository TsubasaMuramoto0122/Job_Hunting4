//========================================
// ���f���X�|�i�[�̏���
// Author : ������
//========================================
#include "model_spawner.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "model.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel_Spawner::CModel_Spawner(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel_Spawner::~CModel_Spawner()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel_Spawner::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�E��]�̐ݒ�
	m_pos = pos;
	m_rot = rot;
	CScene::SetPos(m_pos);
	CScene::SetRot(m_rot);

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CModel_Spawner::Uninit(void)
{
	// ���f���N���X�̔j��
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
void CModel_Spawner::Update(void)
{
	m_pos = CScene::GetPos();

	// ���f���X�V����
	if (m_pModel)
	{
		m_pModel->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel_Spawner::Draw(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���݂̃}�e���A�����擾
	D3DXMATRIX mtxRotModel, mtxTransModel;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;					// �e�̃}�g���b�N�X
	//D3DMATERIAL9 Matdef;

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

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
CModel_Spawner *CModel_Spawner::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���X�^���X����
	CModel_Spawner *pSpawner = nullptr;

	if (pSpawner = new CModel_Spawner())
	{
		pSpawner->Init(pos, rot);		// ������

		// �p�x�����W�A���ɕϊ�
		float rot_x = D3DXToRadian(rot.x);
		float rot_y = D3DXToRadian(rot.y);
		float rot_z = D3DXToRadian(rot.z);

		if (!pSpawner->m_pModel)
		{
			// ���f���̐���
			pSpawner->m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
			// �e���f����NULL�ɐݒ�
			pSpawner->m_pModel->SetParent(NULL);
			// ���f���^�C�v�̐ݒ�
			pSpawner->CScene::SetModelType(CScene::MODTYPE_NORMAL);
			// �V���O�����f���T�C�Y�̐ݒ�
			pSpawner->SetSize(pSpawner->m_pModel->GetSize());
		}

	}

	return pSpawner;
}

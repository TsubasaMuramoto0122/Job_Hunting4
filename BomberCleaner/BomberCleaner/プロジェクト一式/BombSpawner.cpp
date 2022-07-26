//========================================
// ���e�X�|�i�[�̏���
// Author : ������
//========================================
#include "BombSpawner.h"
#include "Bomb.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "LoadX.h"

//===========================================
// �}�N����`
//===========================================
#define SPAWN_TIME		(120)		// �X�|�[���Ԋu����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBombSpawner::CBombSpawner(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pBomb, 0, sizeof(m_pBomb));		// ���e�N���X
	memset(&m_pModel, 0, sizeof(m_pModel));		// ���f���N���X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �X�P�[��
	m_nSpawnTime = 0;							// �X�|�[���^�C��
	m_bIsThere = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBombSpawner::~CBombSpawner()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBombSpawner::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;			// �ʒu
	m_rot = rot;			// ��]
	CScene::SetPos(m_pos);	// �ʒu��ݒ�

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CBombSpawner::Uninit(void)
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
void CBombSpawner::Update(void)
{
	// ���f���X�V
	if (m_pModel)
	{
		m_pModel->Update();

		// ���e���X�|�i�[�̏��ɂ��Ȃ���Ԃ̏���
		if (!m_bIsThere)
		{
			m_nSpawnTime--;
			if (m_nSpawnTime <= 0)	// ���e�𐶐�
			{
				if (!m_pBomb)
				{
					m_pBomb = CBomb::Create({ m_pos.x,m_pos.y + (m_pModel->GetSize().y * m_scale.y),m_pos.z }, m_rot, m_scale, CManager::GetInstance()->GetLoadX()->GetNum("MODTYPE_BOMB"));
				}
				m_nSpawnTime = SPAWN_TIME;		// �G�X�|�[���^�C�}�[�����Z�b�g
				m_bIsThere = true;				// ���e���o�����Ă����Ԃɂ���
			}
		}
		else	// ���e���X�|�i�[�̏��ɂ����Ԃ̏���
		{
			if (m_pBomb)
			{
				if (m_pBomb->GetThrow())	// ���e�𓊂���������擾
				{
					m_pBomb = nullptr;		// ���e�j��
					m_bIsThere = false;		// ���e���X�|�i�[�̏ꏊ�ɂ��Ȃ���Ԃɂ���
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBombSpawner::Draw(void)
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
CBombSpawner *CBombSpawner::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���X�^���X����
	CBombSpawner *pBombSpanwer = nullptr;
	if (!pBombSpanwer)
	{
		pBombSpanwer = new CBombSpawner();
		if (pBombSpanwer)
		{
			pBombSpanwer->m_scale = scale;		// �X�P�[��
			pBombSpanwer->Init(pos, rot);		// ������

			// �p�x�����W�A���ɕϊ�
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pBombSpanwer->m_pModel)
			{
				// ���e�X�|�i�[���f���̐���
				pBombSpanwer->m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
				// �e���f����NULL�ɐݒ�
				pBombSpanwer->m_pModel->SetParent(NULL);
				// ���f���^�C�v�̐ݒ�
				pBombSpanwer->CScene::SetModelType(CScene::MODTYPE_BOMBSPAWNER);
				// ���e�X�|�i�[�T�C�Y�̐ݒ�
				pBombSpanwer->SetSize(pBombSpanwer->m_pModel->GetSize());
			}
		}
	}
	return pBombSpanwer;
}
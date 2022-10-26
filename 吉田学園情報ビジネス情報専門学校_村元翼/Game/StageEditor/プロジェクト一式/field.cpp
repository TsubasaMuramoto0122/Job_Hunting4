//==============================================================================
// �t�B�[���h�̍쐬
// Author:������
//==============================================================================
#include "Scene3D.h"
#include "model.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "field.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CField::CField()
{
	// �e�평����
	m_pTexture = nullptr;						// �e�N�X�`���|�C���^
	m_pVtxBuff = nullptr;						// ���_�o�b�t�@�|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]

	// ���_���W
	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		m_vtx[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vtxRot[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 3D�|���S��
	m_pScene3D = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CField::~CField()
{

}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot)
{
	// �C���X�^���X����
	CField *pField = nullptr;

	if (pField = new CField())
	{
		pField->m_pos = pos;			// �ʒu
		pField->m_size = size;			// �傫��
		pField->m_rot = rot;			// ��]

		pField->Init(pField->m_pos, pField->m_size);	// ������
	}
	return pField;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �V�[��3D�̐����ƃe�N�X�`���̎擾
	m_pScene3D = CScene3D::Create(pos, size, m_rot);
	m_pScene3D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));
	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CField::Uninit(void)
{
	m_pScene3D->Uninit();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CField::Update(void)
{
	m_pScene3D->Update();
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CField::Draw(void)
{
	m_pScene3D->Draw();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxTrans,mtxRot;															// �ʒu�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;																// �e�̃}�g���b�N�X

	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		D3DXVECTOR3 LocalVtx[FIELD_VTX];							// ���[�J�����W�ۑ��ϐ�
		LocalVtx[nCnt] = m_pScene3D->GetLocalVtx(nCnt);				// ���[�J�����W�擾

		// �e���_�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_vtxMtxWorld[nCnt]);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, LocalVtx[nCnt].x, LocalVtx[nCnt].y, LocalVtx[nCnt].z);
		D3DXMatrixMultiply(&m_vtxMtxWorld[nCnt], &m_vtxMtxWorld[nCnt], &mtxTrans);

		// �e�q�֌W�t������
		//*****************************************************************************
		// �e���_�̐e�̃}�g���b�N�X��ݒ�
		if (m_pScene3D->GetMatrix() != nullptr)
		{
			mtxParent = m_pScene3D->GetMatrix();
		}

		else
		{
			// �ŐV�̃}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		// �Z�o�����e���_�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&m_vtxMtxWorld[nCnt], &m_vtxMtxWorld[nCnt], &mtxParent);
		//****************************************************************************
		// �����܂�

		// �e���_�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_vtxMtxWorld[nCnt]);
	}

	// �e���_�̃��[���h���W�̕ۑ�
	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		m_vtx[nCnt].x = m_vtxMtxWorld[nCnt]._41;
		m_vtx[nCnt].y = m_vtxMtxWorld[nCnt]._42;
		m_vtx[nCnt].z = m_vtxMtxWorld[nCnt]._43;
	}
}

//================================================
// ���[���h�}�g���b�N�X�ݒ菈��
//================================================
void CField::SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;						

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_vtxMtxWorld[nCntVtx]);		

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vtxRot[nCntVtx].y, m_vtxRot[nCntVtx].x, m_vtxRot[nCntVtx].z);
	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, vtxPos.x, vtxPos.y, vtxPos.z);
	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxTrans);

	D3DXMATRIX mtxParent;		//�e�̃}�g���b�N�X
	mtxParent = m_mtxWorld;

	//�Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_vtxMtxWorld[nCntVtx], &m_vtxMtxWorld[nCntVtx], &mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_vtxMtxWorld[nCntVtx]);
}
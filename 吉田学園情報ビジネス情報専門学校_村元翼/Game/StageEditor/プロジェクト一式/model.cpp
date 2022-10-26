//=============================================================================
// ���f���N���X����
// Author : ������
//=============================================================================
#include "model.h"
#include "Scene3D.h"
#include "scene.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "LoadX.h"
#include <assert.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define RANGE_COLLISION (100000.0f)	// �����蔻��͈�
#define IKD_EPSIRON		(0.00001f)	// �덷

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	memset(m_vtx, NULL, sizeof(m_vtx));				// ���_
	m_pMesh			= nullptr;						// ���b�V���i���_���j�ւ̃|�C���^
	m_nNumVtx		= 0;							// ���_�̐�
	m_vtxMin		= { 0.0f, 0.0f, 0.0f };			// ���f���̍ŏ��l
	m_vtxMax		= { 0.0f, 0.0f, 0.0f };			// ���f���̍ő�l
	m_pos			= { 0.0f, 0.0f, 0.0f };			// ���f���̈ʒu�i�I�t�Z�b�g�j
	m_rot			= { 0.0f, 0.0f, 0.0f };			// ���f���̌���
	m_size			= { 0.0f, 0.0f, 0.0f };			// ���f���̑傫��
	m_scale			= { 0.0f, 0.0f, 0.0f };			// ���f���̋K��
	m_SaveEmissive	= { 0.0f, 0.0f, 0.0f,1.0f };	// �����F�ۑ�
	m_bDraw			= false;						// �`�悷�邩					
	m_bDoOnce		= false;						// ���̂ݒʂ鏈��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, bool bCollision)
{
	//�C���X�^���X����
	CModel *pModel = nullptr;
	if (!pModel)
	{
		pModel = new CModel;
		if (pModel)
		{
			pModel->m_pos = pos;							// �ʒu
			pModel->m_rot = rot;							// ��]
			pModel->m_scale = scale;						// �K��
			pModel->m_bCollision = bCollision;				// �����蔻������邩
			pModel->Init(nXType);							// ������
		}
	}
	return pModel;
}


//=============================================================================
// ���f���̏���������
//=============================================================================
void CModel::Init(int nXType)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// X�t�@�C���f�[�^�擾
	m_nXType = nXType;
	CLoadX::XData pXData = CManager::GetInstance()->GetLoadX()->GetXData(m_nXType);
	m_pBuffMat		= pXData.m_pBuffMat;
	m_pMesh			= pXData.m_pMesh;
	m_nNumMat		= pXData.m_nNumMat;
	m_pMat			= pXData.m_pMat;
	m_pTexture		= pXData.m_pTex;
	m_SaveEmissive	= pXData.m_pMat->MatD3D.Emissive;

	// ���_�����擾
	m_nNumVtx = m_pMesh->GetNumVertices();
	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	m_sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&m_pVtxBuff);
	//���f���̑傫���𑪂�
	for (int nCnt = 0; nCnt < m_nNumVtx; nCnt++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)m_pVtxBuff;	// ���_���W�̑��
		
		// x�̍ő�l�̔�r
		if (m_vtxMax.x <= vtx.x)
		{
			m_vtxMax.x = vtx.x;
		}
		// x�̍ŏ��l�̔�r
		if (m_vtxMin.x >= vtx.x)
		{
			m_vtxMin.x = vtx.x;
		}
		// y�̍ő�l�̔�r
		if (m_vtxMax.y <= vtx.y)
		{
			m_vtxMax.y = vtx.y;
		}
		// y�̍ŏ��l�̔�r
		if (m_vtxMin.y >= vtx.y)
		{
			m_vtxMin.y = vtx.y;
		}
		// z�̍ő�l�̔�r
		if (m_vtxMax.z <= vtx.z)
		{
			m_vtxMax.z = vtx.z;
		}
		// z�̍ŏ��l�̔�r
		if (m_vtxMin.z >= vtx.z)
		{
			m_vtxMin.z = vtx.z;
		}

		// xyz�̍ő�l�ƍŏ��l�������A�T�C�Y�ɂ���
		m_size.x = m_vtxMax.x - m_vtxMin.x;
		m_size.y = m_vtxMax.y - m_vtxMin.y;
		m_size.z = m_vtxMax.z - m_vtxMin.z;

		m_pVtxBuff += m_sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	// 8�̒��_���̕ۑ�
	// �����i��ʁj
	m_vtx[0].vtx.x = m_vtxMin.x;
	m_vtx[0].vtx.y = m_vtxMax.y;
	m_vtx[0].vtx.z = m_vtxMax.z;

	// �E���i��ʁj
	m_vtx[1].vtx.x = m_vtxMax.x;
	m_vtx[1].vtx.y = m_vtxMax.y;
	m_vtx[1].vtx.z = m_vtxMax.z;
		
	// ���O�i��ʁj
	m_vtx[2].vtx.x = m_vtxMin.x;
	m_vtx[2].vtx.y = m_vtxMax.y;
	m_vtx[2].vtx.z = m_vtxMin.z;

	// �E�O�i��ʁj
	m_vtx[3].vtx.x = m_vtxMax.x;
	m_vtx[3].vtx.y = m_vtxMax.y;
	m_vtx[3].vtx.z = m_vtxMin.z;
		
	// �����i���ʁj
	m_vtx[4].vtx.x = m_vtxMin.x;
	m_vtx[4].vtx.y = m_vtxMin.y;
	m_vtx[4].vtx.z = m_vtxMax.z;
	
	// �E���i���ʁj
	m_vtx[5].vtx.x = m_vtxMax.x;
	m_vtx[5].vtx.y = m_vtxMin.y;
	m_vtx[5].vtx.z = m_vtxMax.z;

	// ���O�i���ʁj
	m_vtx[6].vtx.x = m_vtxMin.x;
	m_vtx[6].vtx.y = m_vtxMin.y;
	m_vtx[6].vtx.z = m_vtxMin.z;

	// �E�O�i���ʁj
	m_vtx[7].vtx.x = m_vtxMax.x;
	m_vtx[7].vtx.y = m_vtxMin.y;
	m_vtx[7].vtx.z = m_vtxMin.z;

	//�@�ϐ�������
	m_SaveEmissive = { 0.0f,0.0f,0.0f,1.0f };
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CModel::Uninit(void)
{
 
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CModel::Draw(void)
{
	//========================================================================
	// ���f���{�̂̃��[���h���W�ݒ�
	//========================================================================
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���݂̃}�e���A�����擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;				// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;								// �e�̃}�g���b�N�X
	D3DMATERIAL9 Matdef;

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �T�C�Y�ύX
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �}�e���A���̎擾
	pDevice->GetMaterial(&Matdef);

	// �����p�[�c�̐e�̃}�g���b�N�X��ݒ�
	if (m_pParent)
	{
		mtxParent = m_pParent->GetMatrix();
	}

	else
	{
		// �ŐV�̃}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// �Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply
	(
		&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent
	);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (!m_bDraw)
	{
		// �}�e���A���f�[�^�҂̃|�C���^���擾
		m_pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
		{
			 m_pMat[nCntMat].MatD3D.Emissive = m_SaveEmissive;

			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&m_pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			if (m_pTexture.at(nCntMat))
			{
				pDevice->SetTexture(0, m_pTexture[nCntMat]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// ���f���i�p�[�c�j�̕`��
			m_pMesh->DrawSubset(nCntMat);
		}
		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&Matdef);
	}

	//========================================================================
	// �e���f���̒��_�̃��[���h���W�ݒ�
	//========================================================================
	D3DXMATRIX vtxParent;	// �e���f���̒��_�̐e�}�g���b�N�X
	D3DXMATRIX vtxTrans;	// �v�Z�p�}�g���b�N�X

	// �z��T�C�Y��ݒ�
	for (int nCnt = 0; nCnt < MAX_MODEL_VTX; nCnt++)
	{
		// �e���_�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_vtx[nCnt].mtxWorld);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&vtxTrans, m_vtx[nCnt].vtx.x, m_vtx[nCnt].vtx.y, m_vtx[nCnt].vtx.z);
		D3DXMatrixMultiply(&m_vtx[nCnt].mtxWorld, &m_vtx[nCnt].mtxWorld, &vtxTrans);

		//*****************************************************************************
		// �e�q�֌W�t������
		//*****************************************************************************
		// �e���_�̐e�̃}�g���b�N�X��ݒ�
		if (m_mtxWorld)
		{
			vtxParent = m_mtxWorld;
		}

		else
		{
			// �ŐV�̃}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_WORLD, &vtxParent);
		}

		// �Z�o�����e���_�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply
		(
			&m_vtx[nCnt].mtxWorld,
			&m_vtx[nCnt].mtxWorld,
			&vtxParent
		);
		//****************************************************************************
		// �����܂�

		// �e���_�̃��[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_vtx[nCnt].mtxWorld);

		// �e���_�̃��[���h���W�̕ۑ�
		m_vtx[nCnt].vtxWorld.x = m_vtx[nCnt].mtxWorld._41;
		m_vtx[nCnt].vtxWorld.y = m_vtx[nCnt].mtxWorld._42;
		m_vtx[nCnt].vtxWorld.z = m_vtx[nCnt].mtxWorld._43;
	}
}
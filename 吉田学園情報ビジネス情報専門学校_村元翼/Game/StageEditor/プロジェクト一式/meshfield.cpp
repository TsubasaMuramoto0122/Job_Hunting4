//==============================================================================
// ���b�V���t�B�[���h�̍쐬
// Author:������
//==============================================================================
#include "meshfield.h"
#include "renderer.h"
#include "manager.h"
#include "model.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define RANGE_COLLISION_MESH (10000000.0f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pTexture = nullptr;						// �e�N�X�`���|�C���^
	m_pVtxBuff = nullptr;						// ���_�o�b�t�@�|�C���^
	m_pVtxIndexBuff = nullptr;					// �C���f�b�N�X�o�b�t�@�|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}

//---------------------------------------------------------------
// �C���X�^���X��������
//---------------------------------------------------------------
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical)
{
	// �C���X�^���X����
	CMeshField *pMeshField = nullptr;

	if (pMeshField = new CMeshField())
	{
		float rot_x = D3DXToRadian(rot.x);
		float rot_y = D3DXToRadian(rot.y);
		float rot_z = D3DXToRadian(rot.z);

		pMeshField->m_rot = { rot_x,rot_y,rot_z };	// ��]
		pMeshField->m_nLine = nLine;				// ���|���S����
		pMeshField->m_nVertical = nVertical;		// �c�|���S����
		pMeshField->Init(pos, size);				// ������
		pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(1));
	}
	return pMeshField;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �ʒu�E�T�C�Y
	m_pos = pos;
	m_size = size;

	// �ʒu�E�T�C�Y�ݒ菈��
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// ���_����ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * (m_nLine + 1) * (m_nVertical + 1),	// ���_��
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// ���_�C���f�b�N�X��ݒ�
	pDevice->CreateIndexBuffer
	(
		(sizeof(WORD) * ((m_nVertical - 1) * (m_nLine + 3) + (m_nLine + 1) * (m_nVertical + 1))),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pVtxIndexBuff,
		NULL
	);

	// 3D���_�o�b�t�@�̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// �t�B�[���h
			if (m_size.y <= 0)
			{
				pVtx[nNum].pos.x = -m_size.x / 2.0f + (m_size.x / m_nLine) * nLine;
				pVtx[nNum].pos.y = 0.0f;
				pVtx[nNum].pos.z = m_size.z / 2.0f - (m_size.z / m_nVertical) * nVertical;
			}

			// �E�H�[��
			else if (m_size.y > 0)
			{
				pVtx[nNum].pos.x = -m_size.x / 2.0f + (m_size.x / m_nLine) * nLine;
				pVtx[nNum].pos.y = m_size.y - (m_size.y / m_nVertical) * nVertical;
				pVtx[nNum].pos.z = 0.0f;
			}

			// �@��
			pVtx[nNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//�J���[
			pVtx[nNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//�e�N�X�`��
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + (1.0f * nLine), 0.0f + (1.0f * nVertical));

			// �z��̍Ō�ɒ��_�̈ʒu�f�[�^��ǉ�
			m_vtxLocal.emplace_back(pVtx[nNum].pos);
		}
	}

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	// ���_�C���f�b�N�X�o�b�t�@�̃��b�N
	m_pVtxIndexBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̊��蓖��
	for (int nCnt1 = 0; nCnt1 < m_nVertical; nCnt1++)
	{
		for (int nCnt2 = 0; nCnt2 < m_nLine + 1; nCnt2++)
		{
			// �ԍ����蓖��
			pIdx[(nCnt2 * 2) + 0 + (m_nLine + 2) * 2 * nCnt1] = (m_nLine + 1) + nCnt2 + (m_nLine + 1) * nCnt1;
			pIdx[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] =					nCnt2 + (m_nLine + 1) * nCnt1;
		}
	}
	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		// �k�ރ|���S���̔ԍ����蓖��
		pIdx[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = m_nLine +		  (m_nLine + 1) * nCnt3;
		pIdx[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3;
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pVtxIndexBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CMeshField::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pVtxIndexBuff)
	{
		m_pVtxIndexBuff->Release();
		m_pVtxIndexBuff = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CMeshField::Update(void)
{
	m_pos = CScene::GetPos();
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // �f�o�C�X�̃|�C���^

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);		// ���[���h�}�g���b�N�X�̏�����

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);					// ���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetIndices(m_pVtxIndexBuff);			// ���_�C���f�b�N�X�̓o�^

	pDevice->SetTexture(0, m_pTexture);				// �e�N�X�`���̐ݒ�

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		((m_nLine + 1) * (m_nVertical + 1)),					// ���_�̐�
		0,														// �J�n���钸�_�̃C���f�b�N�X
		2 * m_nLine * m_nVertical + (m_nVertical * 4) - 4		// �`�悷��v���~�e�B�u��
		// ��1�t�B�[���h��2�v���~�e�B�u	���k�ރ|���S���̕�(Z������1�t�B�[���h�L�΂���4�������邩��)
	);

	D3DXMATRIX mtxParent;	// �e�̃}�g���b�N�X

	// �z��T�C�Y��ݒ�
	m_mtxVec.resize((m_nLine + 1) * (m_nVertical + 1));
	m_vtxWorld.resize((m_nLine + 1) * (m_nVertical + 1));

	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// �e���_�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_mtxVec[nNum]);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_vtxLocal[nNum].x, m_vtxLocal[nNum].y, m_vtxLocal[nNum].z);
			D3DXMatrixMultiply(&m_mtxVec[nNum], &m_mtxVec[nNum], &mtxTrans);

			//*****************************************************************************
			// �e�q�֌W�t������
			//*****************************************************************************
			// �e���_�̐e�̃}�g���b�N�X��ݒ�
			if (m_mtxWorld != nullptr)
			{
				mtxParent = m_mtxWorld;
			}

			else
			{
				// �ŐV�̃}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
			}

			// �Z�o�����e���_�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
			D3DXMatrixMultiply
			(
				&m_mtxVec[nNum],
				&m_mtxVec[nNum],
				&mtxParent
			);
			//****************************************************************************
			// �����܂�

			// �e���_�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &m_mtxVec[nNum]);

			// �e���_�̃��[���h���W�̕ۑ�
			m_vtxWorld[nNum].x = m_mtxVec[nNum]._41;
			m_vtxWorld[nNum].y = m_mtxVec[nNum]._42;
			m_vtxWorld[nNum].z = m_mtxVec[nNum]._43;
		}
	}

}

//------------------------------------------------------------
// ���_���W�̐ݒ�
//------------------------------------------------------------
void CMeshField::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScene::SetPos(pos);
	CScene::SetSize(size);
	m_pos = pos;
	m_size = size;
}

//------------------------------------------------------------
//�e�N�X�`���̐ݒ�
//------------------------------------------------------------
void CMeshField::SetTex(int nAnim, int nPartU)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 0.0);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 1.0f);

	m_pVtxBuff->Unlock();

}
void CMeshField::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[1].tex = D3DXVECTOR2(1.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fSpeedX, 1.0f + fSpeedY);
	pVtx[3].tex = D3DXVECTOR2(1.0f + fSpeedX, 1.0f + fSpeedY);

	m_pVtxBuff->Unlock();
}
void CMeshField::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();
}

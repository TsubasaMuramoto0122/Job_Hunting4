//==============================================================================
// ���b�V���X�t�B�A�̍쐬
// Author:������
//==============================================================================
#include "meshsphere.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshSphere::CMeshSphere(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pTexture = nullptr;						// �e�N�X�`���|�C���^
	m_pVtxBuff = nullptr;						// ���_�o�b�t�@�|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �T�C�Y
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �X�P�[��

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//---------------------------------------------------------------
// �C���X�^���X��������
//---------------------------------------------------------------
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical,bool bInsideDraw,CScene::OBJTYPE objtype)
{
	// �C���X�^���X����
	CMeshSphere *pMeshSphere = nullptr;
	if (!pMeshSphere)
	{
		pMeshSphere = new CMeshSphere(objtype);

		if (pMeshSphere)
		{
			pMeshSphere->m_rot = rot;					// ��]
			pMeshSphere->m_nLine = nLine;				// ���|���S����
			pMeshSphere->m_nVertical = nVertical;		// �c�|���S����
			pMeshSphere->m_bInsideDraw = bInsideDraw;	// ���`��ɂ��邩
			pMeshSphere->Init(pos, size);				// ������
		}
	}
	return pMeshSphere;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CMeshSphere::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �ʒu�E�T�C�Y
	m_pos = pos;
	m_size = size;

	// �ʒu�E�T�C�Y�E�J���[�ݒ菈��
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);
	CScene::SetCol({ 1.0f,1.0f,1.0f,1.0f });

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
				// ���_�ʒu
				pVtx[nNum].pos.x = cosf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * (m_size.x / 2.0f);
				pVtx[nNum].pos.y = sinf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * (m_size.x / 2.0f);
				pVtx[nNum].pos.z = m_size.z / 2.0f - (m_size.z / m_nVertical) * nVertical;
			}

			// �E�H�[��
			else if (m_size.y > 0)
			{

				pVtx[nNum].pos.x = cosf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * sinf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_size.x / 2.0f);
				pVtx[nNum].pos.y = cosf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_size.y / 2.0f);
				pVtx[nNum].pos.z = sinf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * sinf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_size.z / 2.0f);
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
			pIdx[(nCnt2 * 2) + 1 + (m_nLine + 2) * 2 * nCnt1] = nCnt2 + (m_nLine + 1) * nCnt1;
		}
	}
	for (int nCnt3 = 0; nCnt3 < m_nVertical - 1; nCnt3++)
	{
		// �k�ރ|���S���̔ԍ����蓖��
		pIdx[((m_nLine + 1) * 2 + 0) * (nCnt3 + 1) + (2 * nCnt3)] = m_nLine + (m_nLine + 1) * nCnt3;
		pIdx[((m_nLine + 1) * 2 + 1) * (nCnt3 + 1) + (1 * nCnt3)] = m_nLine * 2 + 2 + (m_nLine + 1) * nCnt3;
	}

	// �C���f�b�N�X�o�b�t�@�̃A�����b�N
	m_pVtxIndexBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CMeshSphere::Uninit(void)
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
void CMeshSphere::Update(void)
{
	m_size = CScene::GetSize();
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CMeshSphere::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	if (GetObjType() == OBJTYPE_EFFECTMESH)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);										// ���C�e�B���O�L��
		CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);					// Ztest
		CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true, pDevice);				// ���Z����
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);		// ���[���h�}�g���b�N�X�̏�����

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

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

	if (GetObjType() == OBJTYPE_EFFECTMESH)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O�L��
		CManager::GetInstance()->GetRenderer()->SetAddSynthesis(false, pDevice);// ���Z����
		CManager::GetInstance()->GetRenderer()->SetZTest(false, pDevice);		// Ztest
	}

	//*****************************************************************************
	// ���_�̃��[���h�}�g���b�N�X
	//*****************************************************************************
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
void CMeshSphere::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	CScene::SetPos(pos);
	CScene::SetSize(scale);
	m_pos = pos;
	m_size = scale;
}

//------------------------------------------------------------
//�e�N�X�`���̐ݒ�
//------------------------------------------------------------
void CMeshSphere::SetTex(int nAnim, int nPartU)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 0.0);
	pVtx[1].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)*nAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((1.0f / nPartU)*(nAnim + 1), 1.0f);

	m_pVtxBuff->Unlock();

}
void CMeshSphere::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[1].tex = D3DXVECTOR2(1.0f + fSpeedX, 0.0f + fSpeedY);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fSpeedX, 1.0f + fSpeedY);
	pVtx[3].tex = D3DXVECTOR2(1.0f + fSpeedX, 1.0f + fSpeedY);

	m_pVtxBuff->Unlock();
}

void CMeshSphere::SetCol(D3DXCOLOR col)
{
	CScene::SetCol(col);

	VERTEX_3D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			pVtx[nNum].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
		}
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
// ���f���̓����蔻��(���Ƌ�)
//=============================================================================
bool CMeshSphere::SphereCollisionSphere(float fRadius, CScene *pScene)
{
	D3DXVECTOR3 pos1 = pScene->GetPos();
	D3DXVECTOR3 pos2;
	pos2.x = m_mtxWorld._41;
	pos2.y = m_mtxWorld._42;
	pos2.z = m_mtxWorld._43;

	float fRadius1 = pScene->GetSize().x;
	float fRadius2 = fRadius;

	if (powf(fRadius1 + fRadius2, 2) >= powf(pos1.x - pos2.x, 2) + powf(pos1.y - pos2.y, 2) + powf(pos1.z - pos2.z, 2))
	{
		return true;
	}

	return false;
}
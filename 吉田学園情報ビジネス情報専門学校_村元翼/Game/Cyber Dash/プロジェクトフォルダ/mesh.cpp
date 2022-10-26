//==============================================================================
// ���b�V���N���X�̍쐬
// Author:������
//==============================================================================
#include "mesh.h"
#include "Scene3D.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMesh::CMesh()
{
	// �e�평����
	m_pTexture = nullptr;						// �e�N�X�`���|�C���^
	m_pVtxBuff = nullptr;						// ���_�o�b�t�@�|�C���^
	m_pVtxIndexBuff = nullptr;					// ���_�C���f�b�N�X�o�b�t�@�|�C���^
	m_posMS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rotMS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]
	m_sizeMS = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �T�C�Y
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMesh::~CMesh()
{

}

//------------------------------------------------------------
// ���_���W�̐ݒ�
//------------------------------------------------------------
void CMesh::SetPos(CScene *pScene,D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�E�傫���ݒ�E�擾
	pScene->SetPos(pos);
	pScene->SetSize(size);
	m_posMS = pos;
	m_sizeMS = size;
}

//------------------------------------------------------------
// �e�N�X�`���̐ݒ�(�w�i�������p)
//------------------------------------------------------------
void CMesh::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	int nNum = 0;
	for (int nVertical = 0; nVertical < m_nVertical + 1; nVertical++)
	{
		for (int nLine = 0; nLine < m_nLine + 1; nLine++, nNum++)
		{
			// �e�N�X�`��
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + ((1.0f / m_nLine) * nLine) + fSpeedX, 0.0f + ((1.0f / m_nVertical) * nVertical) + fSpeedY);
		}
	}
	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// �J���[�̐ݒ�
//------------------------------------------------------------
void CMesh::SetCol(CScene *pScene, D3DXCOLOR col)
{
	pScene->SetCol(col);

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

//------------------------------------------------------------
// ��]�̐ݒ�
//------------------------------------------------------------
void CMesh::AddRot(D3DXVECTOR3 rot)
{
	m_rotMS += rot;

	//++++++++++++++++++++++++++++++//
	//			Y����]���E		  �@//
	//++++++++++++++++++++++++++++++//
	if (m_rotMS.y < -D3DX_PI)
	{
		m_rotMS.y = D3DX_PI;
	}
	else if (m_rotMS.y > D3DX_PI)
	{
		m_rotMS.y = -D3DX_PI;
	}

	//++++++++++++++++++++++++++++++//
	//			X����]���E		  �@//
	//++++++++++++++++++++++++++++++//
	if (m_rotMS.x < -D3DX_PI)
	{
		m_rotMS.x = D3DX_PI;
	}
	else if (m_rotMS.x > D3DX_PI)
	{
		m_rotMS.x = -D3DX_PI;
	}

	//++++++++++++++++++++++++++++++//
	//			Z����]���E		  �@//
	//++++++++++++++++++++++++++++++//
	if (m_rotMS.z < -D3DX_PI)
	{
		m_rotMS.z = D3DX_PI;
	}
	else if (m_rotMS.z > D3DX_PI)
	{
		m_rotMS.z = -D3DX_PI;
	}
}
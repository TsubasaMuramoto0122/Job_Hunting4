//==============================================================================
// �V�[��3D�̍쐬
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
	// �e�평����
	m_bInsideDraw	= false;							// �����`�攻��
	m_scale			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �X�P�[��
	objtype			= nPriority;						// �I�u�W�F�N�g�̃^�C�v
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshSphere::~CMeshSphere()
{

}

//---------------------------------------------------------------
// �I�u�W�F�N�g��������
//---------------------------------------------------------------
CMeshSphere *CMeshSphere::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical, bool bInsideDraw, CScene::OBJTYPE objtype)
{
	// �I�u�W�F�N�g����
	CMeshSphere *pMeshSphere = nullptr;

	// NULL�`�F�b�N
	if (pMeshSphere = new CMeshSphere(objtype))
	{
		pMeshSphere->m_rotMS = rot;					// ��]
		pMeshSphere->m_nLine = nLine;				// ���|���S����
		pMeshSphere->m_nVertical = nVertical;		// �c�|���S����
		pMeshSphere->m_bInsideDraw = bInsideDraw;	// ���`��ɂ��邩
		pMeshSphere->Init(pos, size);				// ������
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
	m_posMS = pos;
	m_sizeMS = size;

	// �ʒu�E�T�C�Y�E�J���[�ݒ菈��
	CScene::SetPos(m_posMS);
	CScene::SetSize(m_sizeMS);
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
			if (m_sizeMS.y <= 0)
			{
				// ���_�ʒu
				pVtx[nNum].pos.x = cosf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * (m_sizeMS.x / 2.0f);
				pVtx[nNum].pos.y = sinf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * (m_sizeMS.x / 2.0f);
				pVtx[nNum].pos.z = m_sizeMS.z / 2.0f - (m_sizeMS.z / m_nVertical) * nVertical;
			}

			// �E�H�[��
			else if (m_sizeMS.y > 0)
			{
				pVtx[nNum].pos.x = cosf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * sinf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_sizeMS.x / 2.0f);
				pVtx[nNum].pos.y = cosf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_sizeMS.y / 2.0f);
				pVtx[nNum].pos.z = sinf((-D3DX_PI / 2) + (D3DX_PI / m_nLine) * (nLine * 2)) * sinf((-D3DX_PI * m_bInsideDraw) + (D3DX_PI / m_nVertical) * (nVertical)) * (m_sizeMS.z / 2.0f);
			}

			// �@��
			pVtx[nNum].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//�J���[
			pVtx[nNum].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			//�e�N�X�`��
			pVtx[nNum].tex = D3DXVECTOR2(0.0f + ((1.0f/m_nLine) * nLine), 0.0f + ((1.0f/m_nVertical) * nVertical));

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
	// �傫���擾
	m_sizeMS = CScene::GetSize();
}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CMeshSphere::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	if (objtype == CScene::OBJTYPE_EFFECTMESH)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);										// ���C�e�B���O�L��
		CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);		// Ztest
		CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true, pDevice);	// ���Z����
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X
	D3DXMatrixIdentity(&m_mtxWorld);		// ���[���h�}�g���b�N�X�̏�����

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMS.y, m_rotMS.x, m_rotMS.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_posMS.x, m_posMS.y, m_posMS.z);
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

	if (objtype == CScene::OBJTYPE_EFFECTMESH)
	{
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);							// ���C�e�B���O�L��
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
			if (m_mtxWorld)
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

//=============================================================================
// �|���S��(�t�B�[���h)�Ɛ��̓����蔻��
//=============================================================================
bool CMeshSphere::LineCollisionMesh(CScene *pScene, const int *nVtx)
{
	// �|���S����0,1,2,3�Ԗڂ����ꂼ��A,B,C,D�Ƃ�
	// pScene�̈ʒu��P�Ƃ���
	// pScene�̍Ō�̈ʒu��Q�Ƃ���

	// Oldpos����pos�̈ʒu�����ԃx�N�g��(���ꂪ�����ɂȂ�(�i�s�x�N�g��))
	const D3DXVECTOR3 vecQP = pScene->GetPos() - pScene->GetPosOld();

	// �e���_�̊O���������x�N�g��
	const D3DXVECTOR3 vecAB = m_vtxWorld[nVtx[1]] - m_vtxWorld[nVtx[0]];
	const D3DXVECTOR3 vecBD = m_vtxWorld[nVtx[3]] - m_vtxWorld[nVtx[1]];
	const D3DXVECTOR3 vecDC = m_vtxWorld[nVtx[2]] - m_vtxWorld[nVtx[3]];
	const D3DXVECTOR3 vecCA = m_vtxWorld[nVtx[0]] - m_vtxWorld[nVtx[2]];

	// �e���_�ƃV�[���̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtxWorld[nVtx[0]];
	const D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtxWorld[nVtx[1]];
	const D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtxWorld[nVtx[2]];
	const D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtxWorld[nVtx[3]];

	// �e���_�ƃV�[���̍Ō�̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAQ = pScene->GetPosOld() - m_vtxWorld[nVtx[0]];
	const D3DXVECTOR3 vecBQ = pScene->GetPosOld() - m_vtxWorld[nVtx[1]];
	const D3DXVECTOR3 vecCQ = pScene->GetPosOld() - m_vtxWorld[nVtx[2]];
	const D3DXVECTOR3 vecDQ = pScene->GetPosOld() - m_vtxWorld[nVtx[3]];

	// �I�u�W�F�N�g�ƃ|���S����2D����
	float crossXY[MESH_VTX];		// XY�͈�(X�����̕�)
	float crossZY[MESH_VTX];		// ZY�͈�(Z�����̕�)
	float crossXZ[MESH_VTX];		// XZ�͈�(����)

	crossXY[0] = vecAB.x * vecAP.y - vecAP.x * vecAB.y;
	crossXY[1] = vecBD.x * vecBP.y - vecBP.x * vecBD.y;
	crossXY[2] = vecDC.x * vecDP.y - vecDP.x * vecDC.y;
	crossXY[3] = vecCA.x * vecCP.y - vecCP.x * vecCA.y;

	crossZY[0] = vecAB.z * vecAP.y - vecAP.z * vecAB.y;
	crossZY[1] = vecBD.z * vecBP.y - vecBP.z * vecBD.y;
	crossZY[2] = vecDC.z * vecDP.y - vecDP.z * vecDC.y;
	crossZY[3] = vecCA.z * vecCP.y - vecCP.z * vecCA.y;

	crossXZ[0] = vecAB.x * vecAP.z - vecAP.x * vecAB.z;
	crossXZ[1] = vecBD.x * vecBP.z - vecBP.x * vecBD.z;
	crossXZ[2] = vecDC.x * vecDP.z - vecDP.x * vecDC.z;
	crossXZ[3] = vecCA.x * vecCP.z - vecCP.x * vecCA.z;

	//***************************************************************************************
	// ���̓����蔻��
	//***************************************************************************************
	if (m_sizeMS.y <= 0)
	{
		// �|���S���͈͓̔��ɂ��邩�̌v�Z(4��2D�O�ό��ʂ�0��艺�Ȃ�)
		if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
			crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
		{
			// �@���x�N�g��
			D3DXVECTOR3 normalVec;
			// �|���S���̖@���x�N�g�������߂�
			D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
			// �P�ʃx�N�g����
			D3DXVec3Normalize(&normalVec, &normalVec);

			// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
			float Dot = D3DXVec3Dot(&normalVec, &vecAP);
			float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

			// 2�̓��ό��ʂ̃x�N�g����+-���قȂ�ƒʂ鏈��(�r���I�_���a)
			//if(Dot * DotOld < 0)
			// �ォ�炠����
			if ((Dot <= 0.0f && DotOld >= -ALLOWABLE_ERROR))
			{
				// �|���S���̍������v�Z���A���
				D3DXVECTOR3 &pos = pScene->GetPos();
				pos.y = m_vtxWorld[nVtx[0]].y -
					(1 / normalVec.y * (normalVec.x * (pos.x - m_vtxWorld[nVtx[0]].x) +
						normalVec.z * (pos.z - m_vtxWorld[nVtx[0]].z)));

				pScene->SetPos(pos);

				// �d�͂�0�ɂ���
				CPlayer *pPlayer = (CPlayer*)pScene;
				pPlayer->SetGravity(0.0f, false);


				// �������������Ԃ�
				return true;
			}

			else
			{
				// �����蔻�菈������߂�
				return false;
			}
		}

		else
		{
			// �����蔻�菈������߂�
			return false;
		}
	}

	//***************************************************************************************
	// �ǂ̓����蔻��
	//***************************************************************************************
	// �ǂ̌�����-90 <= x < 90 �̎��̂� ����ȊO��if���̕������t�ɂȂ� 
	else if (m_sizeMS.y > 0)
	{
		if (m_rotMS.y >= -D3DX_PI / 2 && m_rotMS.y < D3DX_PI / 2)
		{

		}

		else
		{
			for (int nCnt = 0; nCnt < MESH_VTX; nCnt++)
			{
				crossXY[nCnt] *= -1;
				crossZY[nCnt] *= -1;
			}
		}


		if ((crossXY[0] < 0.0f && crossXY[1] < 0.0f && crossXY[2] < 0.0f && crossXY[3] < 0.0f) ||	// X�����͈̔�
			(crossZY[0] < 0.0f && crossZY[1] < 0.0f && crossZY[2] < 0.0f && crossZY[3] < 0.0f))		// Z�����͈̔�
		{
			// �@���x�N�g��
			D3DXVECTOR3 normalVec;
			// �|���S���̖@���x�N�g�������߂�
			D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
			// �P�ʃx�N�g����
			D3DXVec3Normalize(&normalVec, &normalVec);

			// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
			float Dot = D3DXVec3Dot(&normalVec, &vecAP);
			float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

			if (Dot <= 0.0f && DotOld > -ALLOWABLE_ERROR)
			{
				// �ǂ���x�N�g�������߁A���K������
				//D3DXVECTOR3 SlideVec = vecQP - D3DXVec3Dot(&vecQP, &normalVec) * normalVec;
				//D3DXVec3Normalize(&SlideVec, &SlideVec);

				// �|���S���̍������v�Z���A���
				D3DXVECTOR3 &pos = pScene->GetPos();
				D3DXVECTOR3 &posOld = pScene->GetPosOld();

				if (Dot != 0.0f || DotOld != 0.0f)
				{
					// ����������߂�
					const float &DividRatio = fabsf(DotOld) / (fabsf(DotOld) + fabsf(Dot));
					// ���_����Փ˓_�Ɍ������x�N�g��
					const D3DXVECTOR3 &DividVec = ((1 - DividRatio) * vecAQ) + (DividRatio * vecAP);
					// �����_�����߂�
					const D3DXVECTOR3 &DividP = m_vtxWorld[nVtx[0]] + DividVec;
					// �v���C���[���߂荞�񂾈ʒu����ǂ̓����_�܂ł̃x�N�g�������߂�(* PERCENT_EXTRA�͕ǂ�菭����O�ɖ߂����� )
					const D3DXVECTOR3 &old = normalVec * D3DXVec3Dot(&normalVec, &((DividP - pos) * PERCENT_EXTRA));
					pos += old;
					pScene->SetPos(pos);
				}
				// �������������Ԃ�
				return true;
			}

			else
			{
				return false;
			}
		}

		else
		{
			return false;
		}
	}

	else
	{
		return false;
	}
}

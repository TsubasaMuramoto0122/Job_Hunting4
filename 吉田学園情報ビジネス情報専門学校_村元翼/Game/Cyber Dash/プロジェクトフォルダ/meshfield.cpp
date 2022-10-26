//==============================================================================
// ���b�V���t�B�[���h�̍쐬
// Author:������
//==============================================================================
#include "meshfield.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "shadow.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMeshField::CMeshField(OBJTYPE nPriority) :CScene(nPriority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}

//---------------------------------------------------------------
// �I�u�W�F�N�g��������
//---------------------------------------------------------------
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical)
{
	// �I�u�W�F�N�g����
	CMeshField *pMeshField = nullptr;

	if (pMeshField = new CMeshField())
	{
		float rot_x = D3DXToRadian(rot.x);
		float rot_y = D3DXToRadian(rot.y);
		float rot_z = D3DXToRadian(rot.z);

		pMeshField->m_rotMS = { rot_x,rot_y,rot_z };	// ��]
		pMeshField->m_nLine = nLine;					// ���|���S����
		pMeshField->m_nVertical = nVertical;			// �c�|���S����
		pMeshField->Init(pos, size);					// ������

		// �e�N�X�`���擾
		pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR2"));
	}


	return pMeshField;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CMeshField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �ʒu�E�T�C�Y�ݒ菈��
	m_posMS = pos;
	m_sizeMS = size;
	CScene::SetPos(m_posMS);
	CScene::SetSize(m_sizeMS);

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
				pVtx[nNum].pos.x = -m_sizeMS.x / 2.0f + (m_sizeMS.x / m_nLine) * nLine;
				pVtx[nNum].pos.y = 0.0f;
				pVtx[nNum].pos.z = m_sizeMS.z / 2.0f - (m_sizeMS.z / m_nVertical) * nVertical;
			}

			// �E�H�[��
			else if (m_sizeMS.y > 0)
			{
				pVtx[nNum].pos.x = -m_sizeMS.x / 2.0f + (m_sizeMS.x / m_nLine) * nLine;
				pVtx[nNum].pos.y = m_sizeMS.y - (m_sizeMS.y / m_nVertical) * nVertical;
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
	CollisionManager();
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
// �����蔻��̊Ǘ�
//=============================================================================
void  CMeshField::CollisionManager(void)
{
	// �V�[���擾
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		CScene *pScene = nullptr;
		switch (nCnt)
		{
		case 0:
			pScene = CScene::GetScene(OBJTYPE_PLAYER);
			break;

		case 1:
			pScene = CScene::GetScene(OBJTYPE_SHADOW);
			break;
		}

		// �V�[����null�ɂȂ�܂Œʂ�
		CScene *pScenePlayer = CScene::GetScene(OBJTYPE_PLAYER);
		while (pScene && pScenePlayer && !m_vtxWorld.empty())
		{
			// ���̃V�[�����擾
			CScene *pSceneNext = CScene::GetSceneNext(pScene);

			float fDist;								// �v���C���[�Ɣ��e�̋��������ϐ�
			D3DXVECTOR3 ObjectPos = pScene->GetPos();	// �I�u�W�F�N�g�̈ʒu�擾
			D3DXVECTOR3 PlayerPos = pScenePlayer->GetPos();

			// �O�����̒藝���g�������𑪂�
			fDist = ((ObjectPos.x - PlayerPos.x) * (ObjectPos.x - PlayerPos.x)) + ((ObjectPos.y - PlayerPos.y) * (ObjectPos.y - PlayerPos.y)) + ((ObjectPos.z - PlayerPos.z) * (ObjectPos.z - PlayerPos.z));

			if (fDist < 10000000.0f)
			{
				// 4���_�̔ԍ����蓖��
				int nID[4];
				for (int nCnt = 0; nCnt < m_nVertical; nCnt++)
				{
					for (int nCnt2 = 0; nCnt2 < m_nLine; nCnt2++)
					{
						nID[0] = (0 + nCnt2) + (nCnt * (m_nLine + 1));
						nID[1] = (1 + nCnt2) + (nCnt * (m_nLine + 1));
						nID[2] = ((2 + (m_nLine - 1)) + nCnt2) + (nCnt * (m_nLine + 1));
						nID[3] = ((3 + (m_nLine - 1)) + nCnt2) + (nCnt * (m_nLine + 1));

						if (LineCollisionMesh(pScene, &nID[0]))
						{
							break;
						}
					}
				}
			}
				// ���̃V�[�������݂̃V�[���ɂ���
				pScene = pSceneNext;
		}
	}
}

//=============================================================================
// �|���S��(�t�B�[���h)�Ɛ��̓����蔻��
//=============================================================================
bool CMeshField::LineCollisionMesh(CScene *pScene,const int *pnVtx)
{
	// �|���S����0,1,2,3�Ԗڂ����ꂼ��A,B,C,D�Ƃ�
	// pScene�̈ʒu��P�Ƃ���
	// pScene�̍Ō�̈ʒu��Q�Ƃ���
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 posOld = pScene->GetPosOld();

	// Oldpos����pos�̈ʒu�����ԃx�N�g��(���ꂪ�����ɂȂ�(�i�s�x�N�g��))
	const D3DXVECTOR3 vecQP = pos - posOld;

	// �e���_�̊O���������x�N�g��
	const D3DXVECTOR3 vecAB = m_vtxWorld[pnVtx[1]] - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBD = m_vtxWorld[pnVtx[3]] - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecDC = m_vtxWorld[pnVtx[2]] - m_vtxWorld[pnVtx[3]];
	const D3DXVECTOR3 vecCA = m_vtxWorld[pnVtx[0]] - m_vtxWorld[pnVtx[2]];

	// �e���_�ƃV�[���̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAP = pos - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBP = pos - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCP = pos - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDP = pos - m_vtxWorld[pnVtx[3]];

	// �e���_�ƃV�[���̍Ō�̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAQ = posOld - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBQ = posOld - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCQ = posOld - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDQ = posOld - m_vtxWorld[pnVtx[3]];

	// �I�u�W�F�N�g�ƃ|���S����2D����
	float crossXY[MESH_VTX];		// XY�͈�(X�����̕�)
	float crossZY[MESH_VTX];		// ZY�͈�(Z�����̕�)
	float crossXZ[MESH_VTX];		// XZ�͈�(����)

	// �v���C���[�̃T�C�YX�𔼌a�Ƃ��Ď擾
	float Radius = pScene->GetSize().y / 2;

	//***************************************************************************************
	// ���̓����蔻��
	//***************************************************************************************
	if (m_sizeMS.y <= 0)
	{
		// ���b�V���̌����𔽑΂ɂ����Ƃ�
		if (m_rotMS.z >= D3DX_PI)
		{
			crossXZ[0] = vecAB.x * -vecAP.z - vecAP.x * -vecAB.z;
			crossXZ[1] = vecBD.x * -vecBP.z - vecBP.x * -vecBD.z;
			crossXZ[2] = vecDC.x * -vecDP.z - vecDP.x * -vecDC.z;
			crossXZ[3] = vecCA.x * -vecCP.z - vecCP.x * -vecCA.z;

			// �|���S���͈͓̔��ɂ��邩�̌v�Z(4��2D�O�ό��ʂ�0��艺�Ȃ�)
			if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
				crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
			{

				// �@���x�N�g��
				D3DXVECTOR3 normalVec;
				// �|���S���̖@���x�N�g�������߂�
				D3DXVec3Cross(&normalVec, &-vecAB, &vecBD);
				// �P�ʃx�N�g����
				D3DXVec3Normalize(&normalVec, &normalVec);

				// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
				float Dot = D3DXVec3Dot(&normalVec, &vecAP);
				float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

				// �����炠����
				if ((Dot >= -Radius && DotOld <= ALLOWABLE_ERROR - Radius))
				{
					// �|���S���̍������v�Z���A���
					D3DXVECTOR3 &pos = pScene->GetPos();
					pos.y = m_vtxWorld[pnVtx[0]].y - (1 / normalVec.y * (normalVec.x * (pos.x - m_vtxWorld[pnVtx[0]].x) + normalVec.z * (pos.z - m_vtxWorld[pnVtx[0]].z)));
					pos.y -= Radius;


					// �^�C�v�ʏ���
					ProcessByObjtype(pScene, pos);

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
				return false;
			}
		}
		else
		{
			crossXZ[0] = vecAB.x * vecAP.z - vecAP.x * vecAB.z;
			crossXZ[1] = vecBD.x * vecBP.z - vecBP.x * vecBD.z;
			crossXZ[2] = vecDC.x * vecDP.z - vecDP.x * vecDC.z;
			crossXZ[3] = vecCA.x * vecCP.z - vecCP.x * vecCA.z;

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

				// �ォ�炠����
				if ((Dot <= Radius && DotOld >= -ALLOWABLE_ERROR + Radius))
				{
					// �|���S���̍������v�Z���A���
					D3DXVECTOR3 &pos = pScene->GetPos();
					pos.y = m_vtxWorld[pnVtx[0]].y - (1 / normalVec.y * (normalVec.x * (pos.x - m_vtxWorld[pnVtx[0]].x) + normalVec.z * (pos.z - m_vtxWorld[pnVtx[0]].z)));
					pos.y += Radius;

					// �^�C�v�ʏ���
					ProcessByObjtype(pScene, pos);

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
	}

	//***************************************************************************************
	// �ǂ̓����蔻��
	//***************************************************************************************
	// �ǂ̌�����-90 <= x < 90 �̎��̂� ����ȊO��if���̕������t�ɂȂ� 
	else if (m_sizeMS.y > 0)
	{
		crossXY[0] = vecAB.x * vecAP.y - vecAP.x * vecAB.y;
		crossXY[1] = vecBD.x * vecBP.y - vecBP.x * vecBD.y;
		crossXY[2] = vecDC.x * vecDP.y - vecDP.x * vecDC.y;
		crossXY[3] = vecCA.x * vecCP.y - vecCP.x * vecCA.y;

		crossZY[0] = vecAB.z * vecAP.y - vecAP.z * vecAB.y;
		crossZY[1] = vecBD.z * vecBP.y - vecBP.z * vecBD.y;
		crossZY[2] = vecDC.z * vecDP.y - vecDP.z * vecDC.y;
		crossZY[3] = vecCA.z * vecCP.y - vecCP.z * vecCA.y;

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

			if (Dot <= Radius && DotOld > -ALLOWABLE_ERROR)
			{
				// �ǂ���x�N�g�������߁A���K������
				//D3DXVECTOR3 SlideVec = vecQP - D3DXVec3Dot(&vecQP, &normalVec) * normalVec;
				//D3DXVec3Normalize(&SlideVec, &SlideVec);

				// �|���S���̍������v�Z���A���
				D3DXVECTOR3 &pos = pScene->GetPos();
				D3DXVECTOR3 &posOld = pScene->GetPosOld();

				// �ړ��x�N�g�������߂�
				const D3DXVECTOR3 moveVec = pos - posOld;

				if (Dot != 0.0f || DotOld != 0.0f)
				{

					// �Ώۂ̉��o������߂�
					// ���[�u�x�N�g���Ɩ@���̓��ς����߂�
					const float fDotMoveVec = -D3DXVec3Dot(&moveVec, &normalVec);
					// �@���̕����ɓ��ϕ��L�΂����x�N�g�������߂�
					const D3DXVECTOR3 Push = D3DXVECTOR3(normalVec.x * fDotMoveVec, normalVec.y * fDotMoveVec, normalVec.z * fDotMoveVec);
					pos += Push;
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

//=============================================================================
// �|���S��(�t�B�[���h)�Ɖe�̓����蔻��
//=============================================================================
bool CMeshField::ShadowCollisionMesh(CScene *pScene, const int *pnVtx)
{
	// �|���S����0,1,2,3�Ԗڂ����ꂼ��A,B,C,D�Ƃ�
	// pScene�̈ʒu��P�Ƃ���
	// pScene�̍Ō�̈ʒu��Q�Ƃ���

	// Oldpos����pos�̈ʒu�����ԃx�N�g��(���ꂪ�����ɂȂ�(�i�s�x�N�g��))
	const D3DXVECTOR3 vecQP = pScene->GetPos() - pScene->GetPosOld();

	// �e���_�̊O���������x�N�g��
	const D3DXVECTOR3 vecAB = m_vtxWorld[pnVtx[1]] - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBD = m_vtxWorld[pnVtx[3]] - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecDC = m_vtxWorld[pnVtx[2]] - m_vtxWorld[pnVtx[3]];
	const D3DXVECTOR3 vecCA = m_vtxWorld[pnVtx[0]] - m_vtxWorld[pnVtx[2]];

	// �e���_�ƃV�[���̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtxWorld[pnVtx[3]];

	// �e���_�ƃV�[���̍Ō�̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[0]];
	const D3DXVECTOR3 vecBQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[1]];
	const D3DXVECTOR3 vecCQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[2]];
	const D3DXVECTOR3 vecDQ = pScene->GetPosOld() - m_vtxWorld[pnVtx[3]];

	// �I�u�W�F�N�g�ƃ|���S����2D����
	float crossXZ[MESH_VTX];		// XZ�͈�(����)

	crossXZ[0] = vecAB.x * vecAP.z - vecAP.x * vecAB.z;
	crossXZ[1] = vecBD.x * vecBP.z - vecBP.x * vecBD.z;
	crossXZ[2] = vecDC.x * vecDP.z - vecDP.x * vecDC.z;
	crossXZ[3] = vecCA.x * vecCP.z - vecCP.x * vecCA.z;

	// �v���C���[�̃T�C�YX�𔼌a�Ƃ��Ď擾
	CPlayer *pPlayer = (CPlayer*)pScene;
	float Radius = pPlayer->GetSize().x / 2;

	//***************************************************************************************
	// ���̓����蔻��
	//***************************************************************************************
		// �|���S���͈͓̔��ɂ��邩�̌v�Z(4��2D�O�ό��ʂ�0��艺�Ȃ�)
	if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
		crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
	{
			return true;
	}

	return false;
}

//------------------------------------------------------------
// ���b�V���t�B�[���h�̃I�u�W�F�N�g�^�C�v�ʏ���
//------------------------------------------------------------
void CMeshField::ProcessByObjtype(CScene *pScene,D3DXVECTOR3 &pos)
{
	OBJTYPE objtype = pScene->GetObjType();
	CPlayer *pPlayer = nullptr;
	CShadow *pShadow = nullptr;
	switch(objtype)
	{
	case OBJTYPE_PLAYER:					// �v���C���[�����蔻��
		pPlayer = (CPlayer*)pScene;
		pPlayer->SetGravity(0.0f, false);	// �d�͂�0�ɂ���
		pPlayer->SetPlayerHit(true);		// �v���C���[�N���X�ɓ������������ݒ肷��
		pPlayer->SetSpeed({ pPlayer->GetSpeed().x,pPlayer->GetBuoyancy() + 0.1f ,pPlayer->GetSpeed().z });
		break;
		
	case OBJTYPE_SHADOW:					// �e�����蔻��
		pShadow = (CShadow*)pScene;
		pShadow->SetHeight(m_posMS.y);
		break;
	}
	pScene->SetPos(pos);
}
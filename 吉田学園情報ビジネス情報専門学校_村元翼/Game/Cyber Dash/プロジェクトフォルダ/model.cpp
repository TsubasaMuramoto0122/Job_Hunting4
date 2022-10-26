//=============================================================================
// �|���S���̏���
// Author : ������
//=============================================================================
#include "model.h"
#include "Scene3D.h"
#include "scene.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "LoadX.h"
#include "shadow.h"
#include <assert.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	memset(m_vtx, NULL, sizeof(m_vtx));
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
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, bool bCollision, bool bPush)
{
	//�C���X�^���X����
	CModel *pModel = nullptr;

	if (pModel = new CModel)
	{
		D3DXVECTOR3 Radian;
		Radian.x = D3DXToRadian(rot.x);					// �p�x�����W�A���ɕϊ�
		Radian.y = D3DXToRadian(rot.y);
		Radian.z = D3DXToRadian(rot.z);

		pModel->m_rot = Radian;							// ��]
		pModel->m_pos = pos;							// �ʒu
		pModel->m_scale = scale;						// �K��
		pModel->m_bCollision = bCollision;				// �����蔻������邩
		pModel->m_bPush = bPush;						// �����o����
		pModel->Init(nXType);							// ������
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
	CLoadX::XData pXData = CManager::GetInstance()->GetLoadX()->GetXData(nXType);
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
	// �I�u�W�F�N�g�Ƃ̓����蔻�����邩
	if (m_bCollision)
	{
		m_bHit = false;
		m_bHitLeft = false;

		if (m_bDoOnce)
		{
			CScene::OBJTYPE objtype;
			CScene *pScene = nullptr;
			HIT_TYPE hitType;
			for (int nCnt = 0; nCnt < 2; nCnt++)
			{
				// �v���C���[�̃V�[���擾
				switch (nCnt)
				{
				case 0:
					objtype = CScene::OBJTYPE_PLAYER;
					hitType = TYPE_SPHERE;
					break;
				case 1:
					objtype = CScene::OBJTYPE_SHADOW;
					hitType = TYPE_LINE;
					break;
				case 2:
					objtype = CScene::OBJTYPE_MODEL;
					hitType = TYPE_SPHERE;
					break;
				}

				pScene = CScene::GetScene(objtype);
				// �V�[����null�ɂȂ�܂Œʂ�
				while (pScene)
				{
					// ���̃V�[�����擾
					CScene *pSceneNext = CScene::GetSceneNext(pScene);
					
					float fDist;								// �v���C���[�Ɣ��e�̋��������ϐ�
					D3DXVECTOR3 ObjectPos = pScene->GetPos();	// �I�u�W�F�N�g�̈ʒu�擾

					// �O�����̒藝���g�������𑪂�
					fDist = ((ObjectPos.x - m_mtxWorld._41) * (ObjectPos.x - m_mtxWorld._41)) + ((ObjectPos.y - m_mtxWorld._42) * (ObjectPos.y - m_mtxWorld._42)) + ((ObjectPos.z - m_mtxWorld._43) * (ObjectPos.z - m_mtxWorld._43));

					if (fDist < 100000.0f)
					{
						// �����蔻��
						if (LineCollisionCube(pScene, hitType))
						{
							m_bHit = true;	// ���ł�������΃q�b�g����Ƃ���
							if (objtype == CScene::OBJTYPE_PLAYER)
							{
								m_bHitPlayer = true;
							}
						}
					}

					// ���̃V�[�������݂̃V�[���ɂ���
					pScene = pSceneNext;
				}
			}
		}

		else
		{
			m_bDoOnce = true;
		}
	}
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
	D3DMATERIAL9 Matdef;								// �ۑ��p�}�e���A���ϐ�

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
		// �}�e���A���f�[�^�̃|�C���^���擾
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

#define IKD_EPSIRON 0.00001f // �덷

///////////////////////////////////////////////////
// ���ʃp�[�e�B�N���Փ˔���E�����E�ʒu�Z�o�֐�
// r : �p�[�e�B�N���̔��a
// pPre_pos : �p�[�e�B�N���̑O�̈ʒu
// pPos : �p�[�e�B�N���̎��̓��B�ʒu
// pNormal : ���ʂ̖@��
// pPlane_pos : ���ʏ��1�_
// pOut_t : �Փˎ��Ԃ��i�[����FLOAT�^�ւ̃|�C���^
// pOut_colli : �p�[�e�B�N���̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
// �߂�l : �Փ�(true), ��Փ�(false)

bool CModel::CalcParticlePlaneCollision(
	FLOAT r,
	D3DXVECTOR3 *pPre_pos, D3DXVECTOR3 *pPos,
	D3DXVECTOR3 *pNormal, D3DXVECTOR3 *pPlane_pos,
	FLOAT *t,
	D3DXVECTOR3 *pOut_colli
)
{
	D3DXVECTOR3 C0 = *pPre_pos - *pPlane_pos;	// ���ʏ�̈�_���猻�݈ʒu�ւ̃x�N�g��
	D3DXVECTOR3 D = *pPos - *pPre_pos;			// ���݈ʒu����\��ʒu�܂ł̃x�N�g��
	D3DXVECTOR3 N;								// �@��
	D3DXVec3Normalize(&N, pNormal);				// �@����W����

	// ���ʂƒ��S�_�̋������Z�o
	FLOAT Dot_C0 = D3DXVec3Dot(&C0, &N);
	FLOAT dist_plane_to_point = fabs(Dot_C0);

	// �i�s�����Ɩ@���̊֌W���`�F�b�N
	FLOAT Dot = D3DXVec3Dot(&D, &N);

	// ���ʂƕ��s�Ɉړ����Ă߂荞��ł���X�y�V�����P�[�X
	if ((IKD_EPSIRON - fabs(Dot) > 0.0f) && (dist_plane_to_point < r)) {
		// �ꐶ�����o���Ȃ��̂ōő厞����Ԃ�
		*t = FLT_MAX;
		// �Փˈʒu�͎d���Ȃ��̂ō��̈ʒu���w��
		*pOut_colli = *pPre_pos;
		return true;
	}

	// �������Ԃ̎Z�o
	*t = (r - Dot_C0) / Dot;

	// �Փˈʒu�̎Z�o
	*pOut_colli = *pPre_pos + (*t) * D;

	// �߂荞��ł�����Փ˂Ƃ��ď����I��
	if (dist_plane_to_point < r)
		return true;

	// �ǂɑ΂��Ĉړ����t�����Ȃ�Փ˂��Ă��Ȃ�
	if (Dot >= 0)
		return false;

	// ���Ԃ�0�`1�̊Ԃɂ���ΏՓ�
	if ((0 <= *t) && (*t <= 1))
		return true;

	return false;
}


//=============================================================================
// ���f���̓����蔻��(���Ɩ�)
//=============================================================================
bool CModel::SurfaceCollisionSphere(CScene *pScene)
{
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 size = pScene->GetSize();

	D3DXVECTOR3 posSphere;
	posSphere.x = m_mtxWorld._41;
	posSphere.y = m_mtxWorld._42;
	posSphere.z = m_mtxWorld._43;
	float fRadius = m_size.x;

	return false;
}

//=============================================================================
// ���f���̓����蔻��(���Ƌ�)
//=============================================================================
bool CModel::SphereCollisionSphere(float fRadius, CScene *pScene)
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
// ���f���̓����蔻��(�_�ƒ�����)
//=============================================================================
bool CModel::DotCollisionCube(CScene *pScene, const HIT_TYPE &hit_type)
{
	//------------------------------------------------
	// �|���S���̖@���x�N�g�������߂�
	//------------------------------------------------
	// 6�ʂ̖@���x�N�g��
	D3DXVECTOR3 normalVec[6];
	// ���
	D3DXVec3Cross(&normalVec[0], &(m_vtx[1].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld));
	// ����
	D3DXVec3Cross(&normalVec[1], &(m_vtx[5].vtxWorld - m_vtx[7].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[7].vtxWorld));
	// �E��
	D3DXVec3Cross(&normalVec[2], &(m_vtx[1].vtxWorld - m_vtx[3].vtxWorld), &(m_vtx[7].vtxWorld - m_vtx[3].vtxWorld));
	// ����
	D3DXVec3Cross(&normalVec[3], &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[4].vtxWorld - m_vtx[0].vtxWorld));
	// �O��
	D3DXVec3Cross(&normalVec[4], &(m_vtx[3].vtxWorld - m_vtx[2].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[2].vtxWorld));
	// �w��
	D3DXVec3Cross(&normalVec[5], &(m_vtx[0].vtxWorld - m_vtx[1].vtxWorld), &(m_vtx[5].vtxWorld - m_vtx[1].vtxWorld));

	// �P�ʃx�N�g����
	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		D3DXVec3Normalize(&normalVec[nCnt], &normalVec[nCnt]);
	}

	//------------------------------------------------
	// ���_�̈ʒu����Ώۂ̈ʒu�܂ł̃x�N�g��
	//------------------------------------------------
	D3DXVECTOR3 pos = pScene->GetPos();	// ���݈ʒu�擾
	D3DXVECTOR3 vecPos[SURFACE_NUM];	// �ʒu�Ɩʂ̃x�N�g��

	// ���
	vecPos[0] = pos - m_vtx[0].vtxWorld;
	// ����
	vecPos[1] = pos - m_vtx[7].vtxWorld;
	// �E��
	vecPos[2] = pos - m_vtx[3].vtxWorld;
	// ����
	vecPos[3] = pos - m_vtx[0].vtxWorld;
	// �O��
	vecPos[4] = pos - m_vtx[2].vtxWorld;
	// �w��
	vecPos[5] = pos - m_vtx[1].vtxWorld;

	// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
	float fDot[SURFACE_NUM];

	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		fDot[nCnt] = D3DXVec3Dot(&normalVec[nCnt], &vecPos[nCnt]);
	}

	float Radius = 0.0f;

	switch (hit_type)
	{
	case TYPE_DOT:
		break;

	case TYPE_LINE:
		break;

	case TYPE_SPHERE:
		Radius = pScene->GetSize().x / 2;
		break;

	case TYPE_CUBE:
		Radius = pScene->GetSize().x / 2;
		break;

	default:
		break;
	}

	if (fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[0] <= Radius && fDot[1] <= Radius)
	{
		return true;
	}

	else
	{
		return false;
	}
}

//=============================================================================
// ���f���̓����蔻��(���ƒ�����)
//=============================================================================
bool CModel::LineCollisionCube(CScene *pScene,const HIT_TYPE &hit_type)
{
	//------------------------------------------------
	// �|���S���̖@���x�N�g�������߂�
	//------------------------------------------------
	// 6�ʂ̖@���x�N�g��
	D3DXVECTOR3 normalVec[6];
	// ���
	D3DXVec3Cross(&normalVec[0], &(m_vtx[1].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld));
	// ����
	D3DXVec3Cross(&normalVec[1], &(m_vtx[5].vtxWorld - m_vtx[7].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[7].vtxWorld));
	// �E��
	D3DXVec3Cross(&normalVec[2], &(m_vtx[1].vtxWorld - m_vtx[3].vtxWorld), &(m_vtx[7].vtxWorld - m_vtx[3].vtxWorld));
	// ����
	D3DXVec3Cross(&normalVec[3], &(m_vtx[2].vtxWorld - m_vtx[0].vtxWorld), &(m_vtx[4].vtxWorld - m_vtx[0].vtxWorld));
	// �O��
	D3DXVec3Cross(&normalVec[4], &(m_vtx[3].vtxWorld - m_vtx[2].vtxWorld), &(m_vtx[6].vtxWorld - m_vtx[2].vtxWorld));
	// �w��
	D3DXVec3Cross(&normalVec[5], &(m_vtx[0].vtxWorld - m_vtx[1].vtxWorld), &(m_vtx[5].vtxWorld - m_vtx[1].vtxWorld));

	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		// �P�ʃx�N�g����
		D3DXVec3Normalize(&normalVec[nCnt], &normalVec[nCnt]);
	}

	//------------------------------------------------
	// ���_�̈ʒu����Ώۂ̈ʒu�܂ł̃x�N�g��
	//------------------------------------------------
	D3DXVECTOR3 pos = pScene->GetPos();							// ���݈ʒu�擾
	D3DXVECTOR3 posOld = pScene->GetPosOld();					// 1�t���[���O�̈ʒu�擾
	D3DXVECTOR3 vecPos[SURFACE_NUM], vecPosOld[SURFACE_NUM];	// �ʒu�Ɩʂ̃x�N�g��
	// ���
	vecPos[0] = pos - m_vtx[0].vtxWorld, vecPosOld[0] = posOld - m_vtx[0].vtxWorld;
	// ����
	vecPos[1] = pos - m_vtx[7].vtxWorld, vecPosOld[1] = posOld - m_vtx[7].vtxWorld;
	// �E��
	vecPos[2] = pos - m_vtx[3].vtxWorld, vecPosOld[2] = posOld - m_vtx[3].vtxWorld;
	// ����
	vecPos[3] = pos - m_vtx[0].vtxWorld, vecPosOld[3] = posOld - m_vtx[0].vtxWorld;
	// �O��
	vecPos[4] = pos - m_vtx[2].vtxWorld, vecPosOld[4] = posOld - m_vtx[2].vtxWorld;
	// �w��
	vecPos[5] = pos - m_vtx[1].vtxWorld, vecPosOld[5] = posOld - m_vtx[1].vtxWorld;

	// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
	float fDot[SURFACE_NUM], fDotOld[SURFACE_NUM];

	for (int nCnt = 0; nCnt < SURFACE_NUM; nCnt++)
	{
		fDot[nCnt] = D3DXVec3Dot(&normalVec[nCnt], &vecPos[nCnt]);
		fDotOld[nCnt] = D3DXVec3Dot(&normalVec[nCnt], &vecPosOld[nCnt]);
	}

	float Radius = 0.0f;
	switch (hit_type)
	{
	case TYPE_DOT:
		break;

	case TYPE_LINE:
		break;

	case TYPE_SPHERE:
		Radius = pScene->GetSize().y / 2;
		break;

	case TYPE_CUBE:
		Radius = pScene->GetSize().y / 2;
		break;

	default:
		break;
	}


	// �ړ��x�N�g�������߂�
	const D3DXVECTOR3 moveVec = pos - posOld;
	D3DXVECTOR3 PushVec;	// �����o���x�N�g��

	// �ォ�瓖����
	if (HitFrom(FROM_UP, fDot, fDotOld, Radius))
	{
		// �����o���ʒu�����߂�
		// PushPos.y = ((normalVec[0].x * normalVec[0].x) - (normalVec[0].z * normalVec[0].z)) / normalVec[0].y;
		pos.y = m_vtx[0].vtxWorld.y - (1 / normalVec[0].y * (normalVec[0].x * (pos.x - m_vtx[0].vtxWorld.x) + normalVec[0].z * (pos.z - m_vtx[0].vtxWorld.z)));
		pos.y += Radius;

		CScene::OBJTYPE objtype = pScene->GetObjType();
		CPlayer *pPlayer = nullptr;
		CShadow *pShadow = nullptr;
		switch (objtype)
		{
		case CScene::OBJTYPE_PLAYER:
			pPlayer = (CPlayer*)pScene;
			pPlayer->SetGravity(0.0f, false);
			pPlayer->SetSpeed({ pPlayer->GetSpeed().x ,pPlayer->GetBuoyancy() + 0.1f,pPlayer->GetSpeed().z});
			break;

		case CScene::OBJTYPE_SHADOW:
			pShadow = (CShadow*)pScene;
			pShadow->SetHeight(pos.y);
			break;
		}

		pScene->SetPos(pos);	// SetPos�����邱�ƂŁA�ŏ��ɓ��������I�u�W�F�N�g�݂̂𔻒�Ƃ���
		return true;
	}

	// �����瓖����
	else if (HitFrom(FROM_DOWN, fDot, fDotOld, Radius))
	{
		// �����o���ʒu�����߂�
		// PushPos.y = ((normalVec[0].x * normalVec[0].x) - (normalVec[0].z * normalVec[0].z)) / normalVec[0].y;
		pos.y = m_vtx[4].vtxWorld.y - (1 / normalVec[0].y * (normalVec[0].x * (pos.x - m_vtx[4].vtxWorld.x) + normalVec[0].z * (pos.z - m_vtx[4].vtxWorld.z)));
		pos.y -= Radius;

		CScene::OBJTYPE objtype = pScene->GetObjType();
		CPlayer *pPlayer = nullptr;
		CShadow *pShadow = nullptr;
		switch (objtype)
		{
		case CScene::OBJTYPE_PLAYER:
			pPlayer = (CPlayer*)pScene;
			pPlayer->SetGravity(0.0f, false);
			pPlayer->SetSpeed({ pPlayer->GetSpeed().x ,0.0f,pPlayer->GetSpeed().z });

			break;

		case CScene::OBJTYPE_SHADOW:
			pShadow = (CShadow*)pScene;
			pShadow->SetHeight(pos.y);
			break;
		}

		pScene->SetPos(pos);	// SetPos�����邱�ƂŁA�ŏ��ɓ��������I�u�W�F�N�g�݂̂𔻒�Ƃ���

		return true;
	}

	// �E���炠����
	if (HitFrom(FROM_RIGHT, fDot, fDotOld, Radius))
	{
		if (m_bPush)
		{
			// �����o��
			PushVec = PushDistanceSide(moveVec, normalVec[2]);
			pos += PushVec;
			pScene->SetPos(pos);
		}

		return true;
	}

	// �����瓖����
	else if (HitFrom(FROM_LEFT, fDot, fDotOld, Radius))
	{
		if (m_bPush)
		{
			// �����o��
			PushVec = PushDistanceSide(moveVec, normalVec[3]);
			pos += PushVec;
			pScene->SetPos(pos);
		}

		m_bHitLeft = true;
		return true;
	}

	// �O���瓖����
	if (HitFrom(FROM_FRONT, fDot, fDotOld, Radius))
	{
		// �����o��
		if (m_bPush)
		{
			PushVec = PushDistanceSide(moveVec, normalVec[4]);
			pos += PushVec;
			pScene->SetPos(pos);
		}

		return true;

	}

	// ��납�炠����
	else if (HitFrom(FROM_BACK, fDot, fDotOld, Radius))
	{
		if (m_bPush)
		{
			// �����o��
			PushVec = PushDistanceSide(moveVec, normalVec[5]);
			pos += PushVec;
			pScene->SetPos(pos);
		}

		return true;
	}

	return false;
}

//===================================
// �����߂����������߂�
//===================================
D3DXVECTOR3 CModel::PushDistanceSide(const D3DXVECTOR3 &moveVec, const D3DXVECTOR3 &vecNor)
{
	// ���[�u�x�N�g���Ɩ@���̓��ς����߂�
	const float fDotMoveVec = -D3DXVec3Dot(&moveVec, &vecNor);
	// �@���̕����ɓ��ϕ��L�΂����x�N�g�������߂�
	const D3DXVECTOR3 Push = D3DXVECTOR3(vecNor.x * fDotMoveVec, vecNor.y * fDotMoveVec, vecNor.z * fDotMoveVec);

	return Push;
}

//===================================
// �ǂ����瓖�����Ă��邩�̔���
//===================================
bool CModel::HitFrom(const HIT_FROM &hitFrom, const float *fDot, const float *fDotOld,const float &Radius)
{
	switch (hitFrom)
	{
	case FROM_UP:
		return fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[0] <= Radius && fDotOld[0] > -ALLOWABLE_ERROR + Radius;

	case FROM_DOWN:
		return fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[1] <= Radius && fDotOld[1] > -ALLOWABLE_ERROR + Radius;

	case FROM_RIGHT:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[4] <= Radius && fDot[5] <= Radius && fDot[2] <= Radius && fDotOld[2] > -ALLOWABLE_ERROR + Radius;

	case FROM_LEFT:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[4] <= Radius && fDot[5] <= Radius && fDot[3] <= Radius && fDotOld[3] > -ALLOWABLE_ERROR + Radius;

	case FROM_FRONT:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[4] <= Radius && fDotOld[4] > -ALLOWABLE_ERROR + Radius;

	case FROM_BACK:
		return fDot[0] <= Radius && fDot[1] <= Radius && fDot[2] <= Radius && fDot[3] <= Radius && fDot[5] <= Radius && fDotOld[5] > -ALLOWABLE_ERROR + Radius;

	default:
		assert(hitFrom <= -1 || hitFrom >= FROM_MAX);
		return false;
	}
}

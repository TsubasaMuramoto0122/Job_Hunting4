//==============================================================================
// �t�B�[���h�̍쐬
// Author:������
//==============================================================================
#include "Scene3D.h"
#include "player.h"
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

	for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
	{
		m_vtx[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_vtxRot[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

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
	if (!pField)
	{
		// ���I�������m��
		pField = new CField();

		// NULL�`�F�b�N
		if (pField)
		{
			pField->m_pos = pos;			// �ʒu
			pField->m_size = size;			// �傫��
			pField->m_rot = rot;			// ��]

			pField->Init(pField->m_pos, pField->m_size);	// ������
		}
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

	// �v���C���[�̃V�[���擾
	CScene *pScene = CScene::GetScene(OBJTYPE_PLAYER);

	// �V�[����null�ɂȂ�܂Œʂ�
	while (pScene)
	{
		// ���̃V�[�����擾
		CScene *pSceneNext = CScene::GetSceneNext(pScene);
		CPlayer *pPlayer = (CPlayer*)pScene;

		// �����蔻��
		if (LineCollisionField(pScene))
		{
			// �d�͂�����ƃg�����|�����ɂȂ�
			//pPlayer->SetGravity(20.0f, true);
		}

		// ���̃V�[�������݂̃V�[���ɂ���
		pScene = pSceneNext;
	}
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

		//// �����𔽉f
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		//D3DXMatrixMultiply(&m_vtxMtxWorld[nCnt], &m_vtxMtxWorld[nCnt], &mtxRot);

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



//=============================================================================
// �|���S��(�t�B�[���h)�Ɠ_�̓����蔻��
//=============================================================================
bool CField::DotCollisionField(CScene *pScene)
{
	// �|���S����0,1,2,3�Ԗڂ����ꂼ��A,B,C,D�Ƃ�
	// pScene�̈ʒu��P�Ƃ���

	// �e���_�ƃV�[���̈ʒu�����ԃx�N�g��
	D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtx[0];
	D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtx[1];
	D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtx[2];
	D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtx[3];

	// �e���_�̃x�N�g�����v�Z
	D3DXVECTOR3 vecAB = m_vtx[1] - m_vtx[0];
	D3DXVECTOR3 vecBD = m_vtx[3] - m_vtx[1];
	D3DXVECTOR3 vecDC = m_vtx[2] - m_vtx[3];
	D3DXVECTOR3 vecCA = m_vtx[0] - m_vtx[2];

	// �|���S���͈͓̔��ɂ��邩�̌v�Z(4��2D�O�ό��ʂ�0�ȉ��Ȃ�)
	if (vecAB.x * vecAP.z - vecAP.x * vecAB.z <= 0 &&
		vecBD.x * vecBP.z - vecBP.x * vecBD.z <= 0 &&
		vecDC.x * vecDP.z - vecDP.x * vecDC.z <= 0 &&
		vecCA.x * vecCP.z - vecCP.x * vecCA.z <= 0)
	{
		// �@���x�N�g��
		D3DXVECTOR3 normalVec;
		// �|���S���̖@���x�N�g�������߂�		��vecAC
		D3DXVec3Cross(&normalVec, &vecAB, &(m_vtx[2] - m_vtx[0]));
		// �P�ʃx�N�g����
		D3DXVec3Normalize(&normalVec, &normalVec);

		// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
		float Dot = D3DXVec3Dot(&normalVec,&vecAP);

		// ���ό��ʂ̃x�N�g���̕�����0�ȉ���������
		if (Dot <= 0)
		{
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

//=============================================================================
// �|���S��(�t�B�[���h)�Ɛ��̓����蔻��
//=============================================================================
bool CField::LineCollisionField(CScene *pScene)
{
	const D3DXVECTOR3 vecQP = pScene->GetPos() - pScene->GetPosOld();

	// �e���_�ƃV�[���̍Ō�̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAQ = pScene->GetPosOld() - m_vtx[0];
	const D3DXVECTOR3 vecBQ = pScene->GetPosOld() - m_vtx[1];
	const D3DXVECTOR3 vecCQ = pScene->GetPosOld() - m_vtx[2];
	const D3DXVECTOR3 vecDQ = pScene->GetPosOld() - m_vtx[3];

	// �e���_�ƃV�[���̈ʒu�����ԃx�N�g��
	const D3DXVECTOR3 vecAP = pScene->GetPos() - m_vtx[0];
	const D3DXVECTOR3 vecBP = pScene->GetPos() - m_vtx[1];
	const D3DXVECTOR3 vecCP = pScene->GetPos() - m_vtx[2];
	const D3DXVECTOR3 vecDP = pScene->GetPos() - m_vtx[3];

	// �e���_�̃x�N�g�����v�Z
	const D3DXVECTOR3 vecAB = m_vtx[1] - m_vtx[0];
	const D3DXVECTOR3 vecBD = m_vtx[3] - m_vtx[1];
	const D3DXVECTOR3 vecDC = m_vtx[2] - m_vtx[3];
	const D3DXVECTOR3 vecCA = m_vtx[0] - m_vtx[2];

	// �I�u�W�F�N�g�ƃ|���S����2D����
	float crossXY[FIELD_VTX];		// XY�͈�(X�����̕�)
	float crossZY[FIELD_VTX];		// ZY�͈�(Z�����̕�)
	float crossXZ[FIELD_VTX];		// XZ�͈�(����)
	
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

	// �ǂ̌�����-90 <= x < 90 �̎��̂� ����ȊO��if���̕������t�ɂȂ� 
	if (m_rot.y >= -D3DX_PI / 2 && m_rot.y < D3DX_PI / 2)
	{

	}

	else
	{
		for (int nCnt = 0; nCnt < FIELD_VTX; nCnt++)
		{
			crossXY[nCnt] *= -1;
			crossZY[nCnt] *= -1;
		}
	}

	// �@���x�N�g��
	D3DXVECTOR3 normalVec;
	// �|���S���̖@���x�N�g�������߂�
	D3DXVec3Cross(&normalVec, &vecAB, &vecBD);
	// �P�ʃx�N�g����
	D3DXVec3Normalize(&normalVec, &normalVec);

	// �|���S���̖@���x�N�g���ƁA�@���ƃV�[���̈ʒu�����ԃx�N�g���̓��ς����߂�
	float Dot = D3DXVec3Dot(&normalVec, &vecAP);
	float DotOld = D3DXVec3Dot(&normalVec, &vecAQ);

	CPlayer *pPlayer = (CPlayer*)pScene;
	const float fPlayerRad = pPlayer->GetSize().x / 2;

	//***************************************************************************************
	// �ǂ̓����蔻��
	//***************************************************************************************
	if (m_size.y > 0)
	{
		if ((crossXY[0] < 0.0f && crossXY[1] < 0.0f && crossXY[2] < 0.0f && crossXY[3] < 0.0f)	||	// X�����͈̔�
			(crossZY[0] < 0.0f && crossZY[1] < 0.0f && crossZY[2] < 0.0f && crossZY[3] < 0.0f))		// Z�����͈̔�
		{
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
					const D3DXVECTOR3 &DividP = m_vtx[0] + DividVec;
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

	else if (m_size.y <= 0)
	{
		//***************************************************************************************
		// ���̓����蔻��
		//***************************************************************************************
		// �|���S���͈͓̔��ɂ��邩�̌v�Z(4��2D�O�ό��ʂ�0��艺�Ȃ�)
		if (crossXZ[0] < 0.0f && crossXZ[1] < 0.0f &&
			crossXZ[2] < 0.0f && crossXZ[3] < 0.0f)
		{
			// 2�̓��ό��ʂ̃x�N�g����+-���قȂ�ƒʂ鏈��(�r���I�_���a)
			//if(Dot * DotOld < 0)
			// �ォ�炠����
			if ((Dot <= fPlayerRad && DotOld >= -ALLOWABLE_ERROR + fPlayerRad))
			{
				// �|���S���̍������v�Z���A���
				D3DXVECTOR3 &pos = pScene->GetPos();
				pos.y = m_vtx[0].y - (1 / normalVec.y * (normalVec.x * (pos.x - m_vtx[0].x) + normalVec.z * (pos.z - m_vtx[0].z)));
				pos.y += fPlayerRad;
				pScene->SetPos(pos);

				// �d�͂�0�ɂ���
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

	else
	{
		return false;
	}
}


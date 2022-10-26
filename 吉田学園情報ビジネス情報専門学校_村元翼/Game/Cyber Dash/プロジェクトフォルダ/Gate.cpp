//===========================================
// �S�[���n�_�̃��f��
// Author:������
//===========================================
#include "Gate.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "Ui.h"
#include "ClearGauge.h"
#include "player.h"

//===========================================
// �R���X�g���N�^
//===========================================
CGate::CGate(OBJTYPE nPriority) : CModel_Spawner(nPriority)
{
	m_scale = { 0.0f,0.0f,0.0f };	// �X�P�[��
	m_Type = GATE_TYPE::A;			// �Q�[�g�̃^�C�v
	m_bDoOnce = false;				// ��x�̂ݒʂ点��ϐ�
}

//===========================================
// �f�X�g���N�^
//===========================================
CGate::~CGate()
{

}

//===========================================
// �I�u�W�F�N�g�����֐�
//===========================================
CGate *CGate::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, GATE_TYPE Type)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �I�u�W�F�N�g����
	CGate *pGate = nullptr;

	// NULL�`�F�b�N
	if (pGate = new CGate())
	{
		pGate->m_Type = Type;		// �Q�[�g�^�C�v
		pGate->m_scale = scale;		// �X�P�[��
		pGate->Init(pos, rot);		// ������

		if (!pGate->m_pModel)
		{
			// ���f���̐���
			pGate->m_pModel = CModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, scale, nXType, true, false);
			// �e���f����NULL�ɐݒ�
			pGate->m_pModel->SetParent(NULL);
			// ���f���^�C�v�̐ݒ�
			pGate->CScene::SetModelType(CScene::MODTYPE_GATE);
			// �V���O�����f���T�C�Y�̐ݒ�
			pGate->SetSize(pGate->m_pModel->GetSize());

			// �^�C�v�ʏ���
			switch (pGate->m_Type)
			{
			case GATE_TYPE::A:
				break;

			case GATE_TYPE::B:
				pGate->m_pModel->SetEmissive(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				break;

			case GATE_TYPE::C:
				pGate->m_pModel->SetEmissive(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				break;

			case GATE_TYPE::D:
				pGate->m_pModel->SetEmissive(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
				break;

			default:
				break;

			}
		}
	}

	return pGate;
}

//===========================================
// �������֐�
//===========================================
HRESULT CGate::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// ���f��������
	CModel_Spawner::Init(pos, rot);
	return S_OK;
}

//===========================================
// �I���֐�
//===========================================
void CGate::Uninit(void)
{
	// ���f���I��
	CModel_Spawner::Uninit();
}

//===========================================
// �X�V�֐�
//===========================================
void CGate::Update(void)
{
	// ���f���X�V
	if (m_pModel)
	{
		m_pModel->Update();

#if(1)
		// �����ŃQ�[�g�̃^�C�v�ɂ���ăv���C���[�̎p���ς��
		if (m_pModel->GetPlayerHitBool() && !m_bDoOnce)
		{
			CPlayer *pPlayer = (CPlayer*)CScene::GetScene(CScene::OBJTYPE_PLAYER);

			// �^�C�v�ʏ���
			switch (m_Type)
			{
			case GATE_TYPE::A:
				pPlayer->SetState(CPlayer::PLAYER_STATE::AIRSHIP);
				break;

			case GATE_TYPE::B:
				pPlayer->SetState(CPlayer::PLAYER_STATE::CUBE);
				break;

			case GATE_TYPE::C:
				pPlayer->SetState(CPlayer::PLAYER_STATE::BALL);
				break;

			case GATE_TYPE::D:
				pPlayer->SetState(CPlayer::PLAYER_STATE::UFO);
				break;

			default:
				pPlayer->SetState(CPlayer::PLAYER_STATE::AIRSHIP);
				break;
			}

			m_bDoOnce = true;
		}
#endif
	}
}

//===========================================
// �`��֐�
//===========================================
void CGate::Draw(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;		// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;				// �e�̃}�g���b�N�X

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

//===========================================
// ��Q�����f��
// Author:������
//===========================================
#include "Obstacle.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "fade.h"
#include "Ui.h"
#include "RemainCount.h"
#include "effect_blast.h"
#include "sound.h"

//===========================================
// �R���X�g���N�^
//===========================================
CObstacle::CObstacle(OBJTYPE nPriority) : CModel_Spawner(nPriority)
{
	m_pUI = nullptr;		// UI
	m_nFrame = 0;			// �t���[��
	m_bDoOnce = false;		// ��x�̂ݒʂ点��ϐ�
	m_bRetry = false;		// ���g���C����
}

//===========================================
// �f�X�g���N�^
//===========================================
CObstacle::~CObstacle()
{

}

//===========================================
// �I�u�W�F�N�g����
//===========================================
CObstacle *CObstacle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType, OBSTACLE obstype)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �I�u�W�F�N�g����
	CObstacle *pObstacle = nullptr;
	if (pObstacle = new CObstacle())
	{
		pObstacle->m_ObsType = obstype;	// ��Q���^�C�v
		pObstacle->Init(pos, rot);		// ������

		if (!pObstacle->m_pModel)
		{
			// ���f���̐���
			pObstacle->m_pModel = CModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, scale, nXType);
			// �e���f����NULL�ɐݒ�
			pObstacle->m_pModel->SetParent(NULL);
			// ���f���^�C�v�̐ݒ�
			pObstacle->CScene::SetModelType(CScene::MODTYPE_OBSCUBE);
			// �V���O�����f���T�C�Y�̐ݒ�
			pObstacle->SetSize(pObstacle->m_pModel->GetSize());
		}
	}

	return pObstacle;
}

//===========================================
// �������֐�
//===========================================
HRESULT CObstacle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// ���f��������
	CModel_Spawner::Init(pos, rot);
	return S_OK;
}


//===========================================
// �I���֐�
//===========================================
void CObstacle::Uninit()
{
	// ���f���I��
	CModel_Spawner::Uninit();

	// UI�폜
	if (m_pUI)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = nullptr;
	}
}

//===========================================
// �X�V�֐�
//===========================================
void CObstacle::Update()
{
	// ���f���X�V
	if (m_pModel)
	{
		m_pModel->Update();

		// �^�C�v�ʏ���
		switch(m_ObsType)
		{
			// �ǂ����瓖�����Ă��ʂ�
		case OBSTACLE::NEEDLE:
			if (m_pModel->GetPlayerHitBool() && !m_bDoOnce)
			{
				m_bDoOnce = true;
				DeletePlayer();
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DEATH);
			}
			break;

			// �����瓖���������̂ݒʂ�
		case OBSTACLE::CUBE:
			if (m_pModel->GetPlayerHitBool()  && !m_bDoOnce && m_pModel->GetHitFromLeft())
			{
				m_bDoOnce = true;
				DeletePlayer();
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DEATH);
			}
			break;
		}
	}
}

//===========================================
// �`��֐�
//===========================================
void CObstacle::Draw()
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���݂̃}�e���A�����擾
	D3DXMATRIX mtxRotModel, mtxTransModel;	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent;					// �e�̃}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRotModel);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	if (m_pModel)
	{
		m_pModel->Draw();
	}
}

//===========================================
// �v���C���[�����֐�
//===========================================
void CObstacle::DeletePlayer(void)
{
	// �v���C���[���擾
	CPlayer *pPlayer = (CPlayer*)CScene::GetScene(OBJTYPE_PLAYER);
	if (pPlayer)
	{
		float fRad = pPlayer->GetSize().y;														// �v���C���[�T�C�YY���擾
		pPlayer->SetUninit();																	// �v���C���[�̏I��
		CBlast::Create(pPlayer->GetPos(), { fRad,fRad,fRad }, { 0.0f,1.0f,1.0f,1.0f }, 20, 20);	// ��������
		CRemainCount::SetRemainChange();														// �c�@�����炷
	}
}
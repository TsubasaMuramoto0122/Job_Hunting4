//=============================================================================
// ���e�̏���
// Author : ������
//=============================================================================
#include "Bomb.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "effect_blast.h"
#include "control_throw.h"
#include "shadow.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define BOMB_TIME				(240)								// ��������
#define SHRINK_AND_SWELL_SPEED	(0.05f)								// �L�k���x
#define MAX_SCALE_X				(1.1f)								// �ő�X�P�[��
#define DIFFUSE_SPEED			(0.05f)								// �F�ύX���x

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBomb::CBomb(OBJTYPE nPriority) : CScene(nPriority)
{
	memset(&m_pShadow, 0, sizeof(m_pShadow));				// �V���h�E
	memset(&m_pControl, 0, sizeof(m_pControl));				// �R���g���[��
	memset(&m_pModel, 0, sizeof(m_pModel));					// ���f��
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ��]
	m_scale			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �X�P�[��
	m_Emissive		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �g�U��
	m_nBombTime		= BOMB_TIME;							// �����܂ł̎���
	m_fSphereRot	= 0.0f;									// �����̉�]
	m_fSphereSize	= 1.0f;									// �����̃T�C�Y
	m_fSphereAlpha	= 1.0f;									// �����̃��l
	m_bIgnition		= false;								// ���Δ���
	m_bShrink		= false;								// �L�k����
	m_bBlink		= false;								// �_�Ŕ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBomb::~CBomb()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBomb::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�E��]�擾
	m_pos = pos;
	m_rot = rot;
	m_fOriginScaleX = m_scale.x;	// �X�P�[��X��ۑ�(�����T�C�Y)

	CScene::SetPos(m_pos);			// �ʒu�ݒ�

	return S_OK;
}
//=============================================================================
// �I������
//=============================================================================
void CBomb::Uninit(void)
{
	// ���f���j��
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// �R���g���[���j��
	if (m_pControl)
	{
		m_pControl->Uninit();
		delete m_pControl;
		m_pControl = nullptr;
	}

	// �e�̔j��
	if (m_pShadow)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CBomb::Update(void)
{
	CScene::SetPosOld(m_pos);	// �ړ��O�̈ʒu�ݒ�
	m_pos = CScene::GetPos();	// �ړ���̈ʒu�擾

	// ���e�������Ԃ̏���
	if (m_pModel)
	{
		m_pModel->Update();

		// ���Ώ�ԏ���
		if (m_bIgnition)
		{
			// ���΂̃G�t�F�N�g
			CEffect::Create({ m_pos.x,m_pos.y + (m_pModel->GetSize().y * m_scale.y),m_pos.z }, IGNITION_SIZE, IGNITION_COL, IGNITION_SUB, 2);
			Ignition();
		}

		// �����Ă����Ԃ̏���
		if (m_bThrow)
		{
			// �R���g���[���N���X�X�V
			if (m_pControl)
			{
				m_pControl->Update(this);
			}
		}

		if (m_pShadow)	// �e�̈ʒu�ݒ�
		{
			m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y, m_pos.z });
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBomb::Draw(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���݂̃}�e���A�����擾
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;	// �v�Z�p�}�g���b�N�X

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

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CBomb *CBomb::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �C���X�^���X����
	CBomb *pBomb = nullptr;
	if (!pBomb)
	{
		pBomb = new CBomb();
		if (pBomb)
		{
			pBomb->m_scale = scale;		// �X�P�[��
			pBomb->Init(pos, rot);		// ������

			// �p�x�����W�A���ɕϊ�
			float rot_x = D3DXToRadian(rot.x);
			float rot_y = D3DXToRadian(rot.y);
			float rot_z = D3DXToRadian(rot.z);

			if (!pBomb->m_pModel)
			{
				// ���e���f���̐���
				pBomb->m_pModel = CModel::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(rot_x, rot_y, rot_z), scale, nXType);
				// �e���f����NULL�ɐݒ�
				pBomb->m_pModel->SetParent(NULL);
				// ���f���^�C�v�̐ݒ�
				pBomb->CScene::SetModelType(CScene::MODTYPE_BOMB);
				// ���e�T�C�Y�̐ݒ�
				pBomb->SetSize(pBomb->m_pModel->GetSize());
			}
		}
	}
	return pBomb;
}

//=============================================================================
// ������ԏ���
//=============================================================================
void CBomb::Explosion(void)
{
	// ������
	CSound *pSound = CManager::GetInstance()->GetSound();
	pSound->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);

	// �����Ă����Ԃ�false�ɂ���
	m_bThrow = false;

	// �������̐���
	if (m_pModel)
	{
		float fRad = m_pModel->GetSize().x;
		CBlast::Create(m_pos, { 30.0f ,30.0f,30.0f }/*{ fRad, fRad ,fRad }*/, { 1.0f,0.0f,0.0f,1.0f }, 20, 20);
	}
}

//=============================================================================
// ���Ώ�ԏ���
//=============================================================================
void CBomb::Ignition(void)
{
	// �����܂ł̃J�E���g
	m_nBombTime--;

	// ���e�T�C�Y�ύX����
	if (!m_bShrink)		// ���e���c���
	{
		m_scale.x += SHRINK_AND_SWELL_SPEED;
		m_scale.y += SHRINK_AND_SWELL_SPEED;
		m_scale.z += SHRINK_AND_SWELL_SPEED;
		if (m_scale.x > m_fOriginScaleX + MAX_SCALE_X)
		{
			m_bShrink = !m_bShrink;
		}
	}
	else				// ���e���k��
	{
		m_scale.x -= SHRINK_AND_SWELL_SPEED;
		m_scale.y -= SHRINK_AND_SWELL_SPEED;
		m_scale.z -= SHRINK_AND_SWELL_SPEED;
		if (m_scale.x < m_fOriginScaleX)
		{
			m_bShrink = !m_bShrink;
		}
	}

	// �����F�ύX����
	if (!m_bBlink)		// �Ԃ��Ȃ�
	{
		m_Emissive.r += DIFFUSE_SPEED;
		if (m_Emissive.r >= 1.0f)
		{
			m_bBlink = !m_bBlink;
		}
	}
	else				// �����Ȃ�
	{
		m_Emissive.r -= DIFFUSE_SPEED;
		if (m_Emissive.r <= 0.0f)
		{
			m_bBlink = !m_bBlink;
		}
	}

	m_pModel->SetEmissive(m_Emissive);
	m_pModel->SetScale(m_scale);

	if (m_nBombTime <= 0)	// 0�Ŕ�������
	{
		CSound *pSound = CManager::GetInstance()->GetSound();
		pSound->StopSound(CSound::SOUND_LABEL_SE_IGNITION);	// ���Ή�

		Explosion();
		Uninit();
	}
}

//=============================================================================
// ���������̏���
//=============================================================================
void CBomb::Throw(D3DXVECTOR3 ThrowDir)
{
	if (!m_bThrow)
	{
		CSound *pSound = CManager::GetInstance()->GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_IGNITION);				// ���Ή�
		pSound->ControllVoice(CSound::SOUND_LABEL_SE_IGNITION, 0.2f);

		m_pControl	= CControlThrow::Create();				// ���������̈ړ��N���X
		m_bThrow	= true;									// �����Ă����Ԃɂ���
		m_bIgnition = true;									// ���Ώ��
		m_ThrowDir	= ThrowDir;								// �������������擾
		D3DXVec3Normalize(&m_ThrowDir, &m_ThrowDir);		// ������P�ʉ�
	}
}
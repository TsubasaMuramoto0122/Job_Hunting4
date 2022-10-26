//=============================================================================
// �v���C���[�̏���
// Author:������
//=============================================================================
#include "main.h"
#include "manager.h"
#include "LoadX.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "keyboard.h"
#include "camera.h"
#include "Scene3D.h"
#include "XInput.h"
#include "DirectInput.h"
#include "effect.h"
#include "model_spawner.h"
#include "meshfield.h"
#include "shadow.h"
#include "mouse.h"
#include "InputManager.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_SHADOWSIZE	(D3DXVECTOR3(m_size.x * 1.5f,0.0f,m_size.z * 1.5f))	// �v���C���[�e�T�C�Y
#define MOVE_EFFECTSIZE		(D3DXVECTOR3(10.0f,10.0f,0.0f))						// �ړ��G�t�F�N�g�T�C�Y
#define JUMP_EFFECT_NUM		(10)												// �W�����v�����Ƃ��̈�x�ɏo���G�t�F�N�g��
#define MARK_SIZE			(D3DXVECTOR3(10.0f,40.0f,0.0f))						// �ڈ�̃T�C�Y
#define INERTIA				(0.1f)												// ����
#define ACCELERATION		(0.2f)												// ������	
#define POS_Y_LIMIT			(1000.0f)											// �c�̈ړ����E�l
#define REVERSE				(-1)												// �l�𔽓]������

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
const float CPlayer::m_fMaxSpeed		= 5.0f;		// �ő呬�x
const float CPlayer::m_fMaxGravity		= 12.0f;	// �d�͍ō����x
float		CPlayer::m_fFallSpeed		= 0.4f;		// �������x
float		CPlayer::m_fGravitySpeed	= 0.6f;		// �d�͂̋���
float		CPlayer::m_fJump			= 11.0f;	// �W�����v��
float		CPlayer::m_fHop				= 8.0f;		// �W�����v��(UFO��)
float		CPlayer::m_fCubeRotate		= 0.1f;		// ��]��(�L���[�u��)
float		CPlayer::m_fBallRotate		= 0.3f;		// ��]��(�{�[����)
float		CPlayer::m_fBuoyancy		= 0.3f;		// ����

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(OBJTYPE nPriority) : CScene(nPriority)
{
	m_pModel		= nullptr;							  // ���f��
	m_pBillboard	= nullptr;							  // �r���{�[�h
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // �ʒu
	m_posOld		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // �Ō�ɍX�V�����ʒu
	m_Speed			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // ���x
	m_fGravity		= 0.0f;								  // �d��
	m_fAngle		= 0.0f;								  // �p�x
	m_bJumpLock		= false;							  // �W�����v���b�N
	m_bReverse		= false;							  // ���]�̔���
	m_bJump			= false;							  // �W�����v�̔���
	m_bUninit		= false;							  // �I������
	m_bSwitch		= false;							  // �؂�ւ�����
	m_bMove			= false;							  // �ړ�����
	m_bHit			= false;							  // �q�b�g����
	m_bCameraMove	= false;							  // �J�����ړ�����
	m_state			= PLAYER_STATE::MAX;				  // �v���C���[�̏��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// �I�u�W�F�N�g����
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYER_STATE nType, bool bMove, bool bCameraMove)
{
	// �I�u�W�F�N�g����
	CPlayer *pPlayer = nullptr;
	if (pPlayer = new CPlayer)
	{
		// ���f���̐���
		if (!pPlayer->m_pModel)
		{
			// ��ԕʃ��f������
			pPlayer->SetState(nType);
			// ���f���^�C�v�̐ݒ�
			pPlayer->CScene::SetModelType(CScene::MODTYPE_PLAYER);
		}

		pPlayer->m_bCameraMove = bCameraMove;	// �J�����ړ��\��
		pPlayer->m_bMove = bMove;				// �ړ��\��
		pPlayer->Init(pos, rot);				// ������
	}

	return pPlayer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�E��]�擾
	m_pos = pos;
	m_rot = rot;

	// �T�C�Y�擾
	m_size = m_pModel->GetSize();

	// �e�̐ݒ�
	if (!m_pShadow)
	{
		m_pShadow = CShadow::Create({m_pos.x , m_pos.y - (m_size.y / 2),m_pos.z}, PLAYER_SHADOWSIZE, { 0.0f,0.0f,0.0f });
	}

	// �ʒu�E��]�ݒ�
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// ��]�p�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	// �J�����̎擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
	pCamera->SetPlayerCamera(this);

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CPlayer::Uninit()
{
	// �v���C���[���f���j��
	ModelDelete();

	// �e�̔j��
	if (m_pShadow)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// �I�u�W�F�N�g�j��
	Release();

	// �Q�[���T�E���h��~
	CManager::GetInstance()->GetSound()->StopSound(CSound::SOUND_LABEL_BGM_GAME);
	
	if (m_bReverse)
	{
		SetReverse();			// �d�͔��]
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CPlayer::Update()
{
	// �V�[���ɕۑ������ʒu���擾
	 m_pos = CScene::GetPos();

	 // ���݂̈ʒu���Ō�̈ʒu�ɂ���
	 m_posOld = m_pos;
	 CScene::SetPosOld(m_posOld);

	 if (m_bCameraMove)
	 {
		 // �J�����̎擾
		 CCamera *pCamera = CManager::GetInstance()->GetCamera(0);

#ifdef _DEBUG
		 //-------------------------------------------
		 // �J�����Ǐ]�֐��̌Ăяo��
		 //-------------------------------------------
		 if (pCamera)
		 {
			 if (m_bSwitch)
			 {
				 switch (m_state)
				 {
					 // ��s�D��p�J�����ړ�
				 case PLAYER_STATE::AIRSHIP:
					 pCamera->SetPlayerCamera2(this);
					 break;

					 // �v���C���[�Ǐ]�J�����ړ�
				 default:
					 pCamera->SetPlayerCamera(this);
					 break;
				 }
			 }
			 else
			 {
				 // �J�������R�ړ�
				 pCamera->NoFollowingPlayer();
			 }

			 //-------------------------------------------
			 // �J�����Ǐ]�؂�ւ�
			 //-------------------------------------------
			 if (CManager::GetInstance()->GetKeyboard()->GetTrigger(DIK_Q))
			 {
				 m_bSwitch = !m_bSwitch;
				 if (m_bSwitch)
				 {
					 pCamera->SetLong(CAMERA_DISTANCE);
					 pCamera->SetRot(CAMERA_ROT);
				 }
				 else
				 {

				 }
			 }
		 }

#else
		 switch (m_state)
		 {
			 // ��s�D��p�J�����ړ�
		 case PLAYER_STATE::AIRSHIP:
			 pCamera->SetPlayerCamera2(this);
			 break;

			 // �v���C���[�Ǐ]�J�����ړ�
		 default:
			 pCamera->SetPlayerCamera(this);
			 break;
		 }
#endif
	 }

	if (m_bMove)
	{
		 //------------------------------------------
		 // �v���C���[����n�֐�
		 //------------------------------------------
		 Move();		// �ړ�
		 Action();		// �A�N�V����
	}

	// �V�[���Ɉʒu��ݒ肷��
	CScene::SetPos(m_pos);

	if (m_pModel)	// �v���C���[���f���X�V
	{
		m_pModel->Update();
	}

	if (m_pShadow)	// �V���h�E�̈ʒu�ݒ�
	{
		m_pShadow->CScene::SetPosOld({ m_pos.x ,m_pos.y, m_pos.z });
	}

	// �I������
	if (m_bUninit)
	{
		Uninit();
	}

	// �q�b�g����OFF
	m_bHit = false;
}

//==========================================================================================================
// �`��
//==========================================================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRotModel, mtxTransModel;											 // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// �N�H�[�^�j�I���Ő���������]�s����|����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxRot, &m_mtxWorld);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTransModel);

	// �e�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���f���̕`��
	m_pModel->Draw();
}

//==========================================================================================================
// �ړ�����
//==========================================================================================================
void CPlayer::Move(void)
{
#ifndef _DEBUG
	m_Speed.x += ACCELERATION;	// �ړ�
#endif

	//=============================================================================
	// �ړ�����(���W�X�V)
	//=============================================================================
	if (m_Speed.x != 0.0f)
	{
		m_pos.x += m_Speed.x;

		// �ړ��̃G�t�F�N�g
		CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 1.0f, 1.0f, 1.0f ,1.0f }, 0.1f, 1);
	}

	//=============================================================================
	// ��ԕʈړ�����
	//=============================================================================
	switch (m_state)
	{
	//----------------------------------
	// �{�[��
	//----------------------------------
	case PLAYER_STATE::BALL:
		m_fAngle -= m_fBallRotate;					// ��ɉ�]������
		Quaternion();								// �N�H�[�^�j�I����]

		// �W�����v
		if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_SPACE) && !m_bJump && !m_bJumpLock)	// A�{�^��
		{
			JumpAction(m_fGravity);	// �W�����v
			SetReverse();			// �d�͔��]
		}

		// �d�͌��E
		if (m_fGravity <= -m_fMaxGravity)
		{
			m_fGravity = -m_fMaxGravity;
		}
		else if (m_fGravity >= m_fMaxGravity)
		{
			m_fGravity = m_fMaxGravity;
		}

		Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// �d��
		Fall(m_pos, m_posOld);

		break;

	//----------------------------------
	// �L���[�u
	//----------------------------------
	case PLAYER_STATE::CUBE:
		if (m_bJump)
		{
			m_fAngle -= m_fCubeRotate;				// �W�����v���ɉ�]
			Quaternion();							// �N�H�[�^�j�I����]
		}
		else
		{
			D3DXMatrixIdentity(&m_mtxRot);			// ���n���ɉ�]������
		}

		// �W�����v
		if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_SPACE) && !m_bJump && !m_bJumpLock)	// A�{�^��
		{
			JumpAction(m_fJump);										// �W�����v
		}

		Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);			// �d��
		Fall(m_pos, m_posOld);

		break;

	//----------------------------------
	// ��s�D
	//----------------------------------
	case PLAYER_STATE::AIRSHIP:
		m_fAngle = atan2(m_Speed.y, m_Speed.x);	// �c���̈ړ��ʂ���p�x�����߂�
		Quaternion();							// �N�H�[�^�j�I����]

		if (CInputManager::PressAnyAction(CInputManager::PRESS_SPACE))
		{
			// ���V�G�t�F�N�g
			CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 0.0f,1.0f,0.0f,1.0f }, 0.05f, 2, "");
			m_Speed.y += m_fBuoyancy;		// �㏸����
		}
		else
		{
			if (m_bHit)
			{
				m_Speed.y = 0.0f;			// �V�䂩���ɏ���Ă���Ƃ��̓X�s�[�h��0�ɂ���
			}
			else
			{
				m_Speed.y -= m_fBuoyancy;	// ������
			}
		}

		m_fGravity = m_Speed.y;										// �d�͂ɃX�s�[�h����
		Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// �d�͊֐�

		break;

	//----------------------------------
	// �t�e�n
	//----------------------------------
	case PLAYER_STATE::UFO:
		// �W�����v
		if (CInputManager::TriggerAnyAction(CInputManager::TRIGGER_SPACE) && !m_bJumpLock)	// A�{�^��
		{
			JumpAction(m_fHop);									// ���˂�
		}

		Gravity(m_pos, m_fGravity, m_fFallSpeed, m_bJump);		// �d��
		break;
	}

	Inertia(m_Speed);								// ����
	SpeedAndRotLimit(m_Speed, m_rot, m_fMaxSpeed);	// ���x�Ɖ�]���E
	PosYLimit(m_pos.y);								// �������E
}

//==========================================================================================================
// �v���C���[�̊���
//==========================================================================================================
void CPlayer::Inertia(D3DXVECTOR3 &speed)
{
	//------------------------------------------------
	// ���E���͂��Ă��Ȃ����
	//------------------------------------------------
	// 0�ɖ߂葱���鏈��
	if (speed.x > 0.0f)
	{
		speed.x -= INERTIA;

		if (speed.x <= 0.0f)
		{
			speed.x = 0.0f;
		}
	}

	else if (speed.x < 0.0f)
	{
		speed.x += INERTIA;

		if (speed.x >= 0.0f)
		{
			speed.x = 0.0f;
		}
	}

	//------------------------------------------------
	// �㉺���͂��Ă��Ȃ����
	//------------------------------------------------
	// 0�ɖ߂葱���鏈��
	if (speed.z > 0.0f)
	{
		speed.z -= INERTIA;

		if (speed.z <= 0.0f)
		{
			speed.z = 0.0f;
		}
	}

	else if (speed.z < 0.0f)
	{
		speed.z += INERTIA;

		if (speed.z >= 0.0f)
		{
			speed.z = 0.0f;
		}
	}
}

//==========================================================================================================
// �N�H�[�^�j�I����]�����֐�
//==========================================================================================================
void CPlayer::Quaternion(void)
{
	D3DXVECTOR3 moveVec = { m_pos - m_posOld };			// �ړ��x�N�g��

	// ���x�N�g��(�ړ��x�N�g���Ɛ����̃x�N�g��)
	D3DXVECTOR3 Axis = { 0.0f,0.0f,0.0f };
	Axis.x = -moveVec.z;
	Axis.z = moveVec.x;

	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		D3DXQUATERNION QuaAns = { 0.0f,0.0f,0.0f,1.0f };					// �P�ʃN�H�[�^�j�I��
		D3DXQuaternionRotationAxis(&QuaAns, &Axis, m_fAngle);				// ���x�N�g���Ɗp�x����N�H�[�^�j�I�������߂�
		D3DXMatrixRotationQuaternion(&m_mtxRot, &QuaAns);					// �N�H�[�^�j�I������]�s��ɕϊ�
	}
}

//==========================================================================================================
// ���x �� ��]�̌��E
//==========================================================================================================
void CPlayer::SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot,const float fMaxSpeed)
{
	//==========================================================================================================
	// ��]���E
	//==========================================================================================================
	if (rot.x < -D3DX_PI)
	{
		rot.x = D3DX_PI;
	}
	else if (rot.x > D3DX_PI)
	{
		rot.x = -D3DX_PI;
	}
	if (rot.z < -D3DX_PI)
	{
		rot.z = D3DX_PI;
	}
	else if (rot.z > D3DX_PI)
	{
		rot.z = -D3DX_PI;
	}

	if (m_fAngle > D3DX_PI)
	{
		m_fAngle = -D3DX_PI;
	}
	else if (m_fAngle < -D3DX_PI)
	{
		m_fAngle = D3DX_PI;
	}

	//==========================================================================================================
	// ���x���E
	//==========================================================================================================
	if (speed.x >= fMaxSpeed)
	{
		speed.x = fMaxSpeed;
	}
	else if (speed.x <= -fMaxSpeed)
	{
		speed.x = -fMaxSpeed;
	}
	if (speed.y >= fMaxSpeed)
	{
		speed.y = fMaxSpeed;
	}
	else if (speed.y <= -fMaxSpeed)
	{
		speed.y = -fMaxSpeed;
	}
	if (speed.z >= fMaxSpeed)
	{
		speed.z = fMaxSpeed;
	}
	else if (speed.z <= -fMaxSpeed)
	{
		speed.z = -fMaxSpeed;
	}
}

//==========================================================================================================
// �v���C���[�A�N�V����(�f�o�b�O�p)
//==========================================================================================================
void CPlayer::Action(void)
{
#ifdef _DEBUG
	// ���̓f�o�C�X�̎擾
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	// ���[�h�ؑ�
	if (pKey->GetTrigger(DIK_1))
	{
		SetState(PLAYER_STATE::BALL);
	}
	else if (pKey->GetTrigger(DIK_2))
	{
		SetState(PLAYER_STATE::CUBE);
	}
	else if (pKey->GetTrigger(DIK_3))
	{
		SetState(PLAYER_STATE::AIRSHIP);
	}
	else if (pKey->GetTrigger(DIK_4))
	{
		SetState(PLAYER_STATE::UFO);
	}
#endif
}

//==========================================================================================================
// �W�����v�֐�
//==========================================================================================================
void CPlayer::JumpAction(const float& fJump)
{
	m_fGravity = fJump;
	m_bJump = true;

	// �W�����v�G�t�F�N�g
	if (m_bReverse)
	{
		for (int nCnt = 0; nCnt < JUMP_EFFECT_NUM; nCnt++)
		{
			CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 0.0f,1.0f,0.0f,1.0f }, 0.05f, 4, "");
		}
	}

	else
	{
		for (int nCnt = 0; nCnt < JUMP_EFFECT_NUM; nCnt++)
		{
			CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 0.0f,1.0f,0.0f,1.0f }, 0.05f, 2, "");
		}
	}
}

//==========================================================================================================
// �v���C���[��Ԑݒ�֐�
//==========================================================================================================
void CPlayer::SetState(PLAYER_STATE state)
{
	if (m_state != state)	// ���݂̏�ԂƓ�����ԂȂ�ʂ�Ȃ�(�p���ς��Ȃ��̂ŏ���������)
	{
		// ���f���I��
		ModelDelete();

		// �J�����擾
		CCamera *pCamera = CManager::GetInstance()->GetCamera(0);

		// ���f����؂�ւ���
		if (!m_pModel)
		{
			char *pStr = nullptr;

			switch (state)
			{
			case PLAYER_STATE::BALL:
				pStr = "PLAYER_ONE";
				m_bJump = true;
				break;

			case PLAYER_STATE::CUBE:
				pStr = "PLAYER_TWO";
				m_bJump = true;
				break;

			case PLAYER_STATE::AIRSHIP:
				pStr = "PLAYER_THREE";
				m_Speed.y = m_fGravity;

				// �ړ��\��
				if (m_bMove)
				{
					pCamera->SetPosVY(200.0f);	// �J�������_��Y�Œ�
					pCamera->SetPosRY(200.0f);	// �J���������_��Y�Œ�
				}

				break;

			case PLAYER_STATE::UFO:
				pStr = "PLAYER_FOUR";
				D3DXMatrixIdentity(&m_mtxRot);
				if (m_bReverse)
				{
					m_fAngle = D3DXToRadian(180.0f);		// ���]������
					Quaternion();							// �N�H�[�^�j�I����]
				}
				break;
			}

			// X�t�@�C���̎�ސݒ�
			CLoadX *pLoad = CManager::GetInstance()->GetLoadX();
			m_nType = pLoad->GetNum(pStr);

			// ���f������
			m_pModel = CModel::Create({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, m_nType, false);
			// �v���C���[�T�C�Y�̐ݒ�
			CScene::SetSize(m_pModel->GetSize());
		}

		m_state = state;	// ��Ԑݒ�
	}
}

//==========================================================================================================
// �d�͂̏���(�v���C���[�ɏd�͂����炷�l(�����l�͌��炵�������̐�)���������āA��O�����ɓ����)
//==========================================================================================================
void CPlayer::Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool& bJump)
{
	// �d�͉��Z
	fGravity -= fGravitySpeed;
	pos.y += fGravity;

	// ���ɂ������ǂ���
	if (pos.y <= 0.0f)
	{
		// �ʒu���Œ肷��
		pos.y = 0.0f;
		fGravity = 0.0f;
		bJump = false;
	}
}

//==========================================================================================================
// ���f���I������
//==========================================================================================================
void CPlayer::ModelDelete(void)
{
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
}

//==========================================================================================================
// �ʒuY���E
//==========================================================================================================
void CPlayer::PosYLimit(float& PosY)
{
	if (PosY > POS_Y_LIMIT)
	{
		switch (m_state)
		{
		case PLAYER_STATE::BALL:
			if(m_fGravity > 0.0f)
			m_bJump = false;
			break;

		case PLAYER_STATE::CUBE:
			if (m_fGravity > 0.0f)
			m_bJump = false;
			break;

		case PLAYER_STATE::AIRSHIP:
			m_bHit = true;
			break;

		case PLAYER_STATE::UFO:
			break;
		}

		PosY = POS_Y_LIMIT;
		m_fGravity = 0.0f;
	}
}

//==========================================================================================================
// �d�͔��]����
//==========================================================================================================
void CPlayer::SetReverse(void)
{
	m_bReverse = !m_bReverse;	// �d�͔��]
	m_fJump			*= REVERSE;
	m_fHop			*= REVERSE;
	m_fGravitySpeed *= REVERSE;
	m_fFallSpeed	*= REVERSE;
	m_fCubeRotate	*= REVERSE;
	m_fBallRotate	*= REVERSE;
	m_fBuoyancy		*= REVERSE;
}

//==========================================================================================================
// ��������
//==========================================================================================================
void CPlayer::Fall(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld)
{
	//��������
	float moveY = pos.y - posOld.y;

	if (moveY != 0.0f)
	{
		m_bJump = true;
	}
}

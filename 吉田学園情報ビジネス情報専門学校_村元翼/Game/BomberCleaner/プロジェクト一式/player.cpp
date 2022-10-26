//=============================================================================
// �v���C���[�̏���
// Author : ������
//=============================================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "keyboard.h"
#include "camera.h"
#include "XInput.h"
#include "DirectInput.h"
#include "effect.h"
#include "Bomb.h"
#include "model_spawner.h"
#include "shadow.h"
#include "billboard.h"
#include "sound.h"
#include <assert.h>

//=============================================================================
// �}�N����`
//=============================================================================
#define PLAYER_SHADOWSIZE	(D3DXVECTOR3(m_size.x * 1.5f,0.0f,m_size.z * 1.5f))	// �v���C���[�e�T�C�Y
#define MOVE_EFFECTSIZE		(D3DXVECTOR3(5.0f,5.0f,0.0f))						// �ړ��G�t�F�N�g�T�C�Y
#define CARRY_RANGE			(600.0f)											// �����^�ׂ�͈�
#define CARRY_RANGE_DIST	(10000.0f)											// �����^�ׂ�͈�(2�_�Ԃ̋����Ɣ�r����)
#define MARK_SIZE			(D3DXVECTOR3(10.0f,40.0f,0.0f))						// �ڈ�̃T�C�Y
#define MAX_PATTERN			(5)													// �ڈ�̃e�N�X�`���p�^�[���̐�
#define DEATH_POSY			(-200.0f)											// ���SY���W

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(OBJTYPE nPriority) : CScene(nPriority)
{
	//-----------------------------------------------
	// �ϐ�������
	//-----------------------------------------------
	m_pBomb			= nullptr;							// ���e�N���X
	m_pModel		= nullptr;							// ���f���N���X
	m_pBillboard	= nullptr;							// �r���{�[�h�N���X
	m_Direction		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_Oldpos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �Ō�̈ʒu
	m_Speed			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ���x
	m_fMaxSpeed		= MAX_SPEED;						// �ő呬�x
	m_fGravity		= 0.0f;								// ���݂̏d��
	m_fGravitySpeed = GRAVITY_SPEED;					// �d�͉����x
	m_nFrame		= 0;								// �t���[����
	m_nPattern		= 0;								// �e�N�X�`���p�^�[��
	m_fAngle		= 0.0f;								// �p�x
	m_bJump			= false;							// �W�����v����
	m_state			= STATE_NORMAL;						// ���
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ����
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	// �C���X�^���X����
	CPlayer *pPlayer = nullptr;
	if (!pPlayer)
	{
		pPlayer = new CPlayer;
		if (pPlayer)
		{
			pPlayer->m_posInit = pos;		// �����ʒu�ۑ�
			pPlayer->m_nType = nType;		// �v���C���[�^�C�v
			pPlayer->Init(pos, rot);		// ������
		}
	}
	return pPlayer;
}

//=============================================================================
// ������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �ʒu�E��]�ۑ�
	m_pos = pos;
	m_rot = rot;

	if (!m_pModel)
	{
		// �p�x�����W�A���ɕϊ�
		float rot_x = D3DXToRadian(rot.x);
		float rot_y = D3DXToRadian(rot.y);
		float rot_z = D3DXToRadian(rot.z);

		// �v���C���[���f������
		m_pModel = CModel::Create({ 0.0f,0.0f,0.0f }, { rot_x,rot_y,rot_z }, { 1.0f,1.0f,1.0f }, m_nType, false);
		// ���f���^�C�v�̐ݒ�
		CScene::SetModelType(CScene::MODTYPE_PLAYER);
		// �v���C���[�T�C�Y�̐ݒ�
		CScene::SetSize(m_pModel->GetSize());
	}

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

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CPlayer::Uninit()
{
	// ���f���N���X�j��
	if (m_pModel)
	{
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}

	// ���e�N���X�̔j��
	if (m_pBomb)
	{
		m_pBomb = nullptr;
	}

	// �e�N���X�̔j��
	if (m_pShadow)
	{
		m_pShadow = nullptr;
	}

	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V
//=============================================================================
void CPlayer::Update()
{
	// �V�[���ɕۑ������ʒu���擾
	 m_pos = CScene::GetPos();

	 // ���݂̈ʒu���Ō�̈ʒu�ɂ���
	 m_Oldpos = m_pos;
	 CScene::SetPosOld(m_Oldpos);

	// �J�����Ǐ]�֐��̌Ăяo��
	CManager::GetInstance()->GetCamera(0)->SetPlayerCamera(this);

	//------------------------------------------
	// �v���C���[����n�֐�
	//------------------------------------------
	Move();														// �ړ�
	Action();													// �A�N�V����
	Gravity(m_pos, m_fGravity, m_fGravitySpeed, m_bJump);		// �d��

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
}

//=============================================================================
// �`��
//=============================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice(); // �f�o�C�X�̃|�C���^
	D3DXMATRIX mtxTrans;															 // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	
	// �N�H�[�^�j�I���Ő���������]�s����|����
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxRot, &m_mtxWorld);

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
// �v���C���[�̊���
//=============================================================================
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
// �ړ�����
//==========================================================================================================
void CPlayer::Move(void)
{
	//=============================================================================
	// �ړ�����(���W�E��]�X�V)
	//=============================================================================
	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		float fLength = sqrtf((m_Speed.x * m_Speed.x) + (m_Speed.z * m_Speed.z));
		float move_x = m_Speed.x / fLength;
		float move_z = m_Speed.z / fLength;
		move_x *= m_Speed.x;
		move_z *= m_Speed.z;

		//---------------------------------------------------
		// X�ړ�(���ړ�)
		//---------------------------------------------------
		if (m_Speed.x != 0.0f)
		{
			m_fAngle -= move_x * ROTATING_VELOCITY;
			m_pos.x += move_x * sinf(m_rot.y);					// �ړ��̓v���C���[�̌����ɉe������
			m_pos.z += move_x * cosf(m_rot.y);
		}

		//---------------------------------------------------
		// Z�ړ�(�c�ړ�)
		//---------------------------------------------------
		if (m_Speed.z != 0.0f)
		{
			m_fAngle -= move_z * ROTATING_VELOCITY;
			m_pos.x += move_z * sinf(m_rot.y);
			m_pos.z += move_z * cosf(m_rot.y);
		}

		// �ړ��̃G�t�F�N�g
		CEffect::Create(m_pos, MOVE_EFFECTSIZE, { 1.0f, 1.0f, 1.0f ,1.0f }, 0.1f, 1);
	}

	//==========================================================================================================
	// �ړ����͔���
	//==========================================================================================================
	if (InputDirection(DIRECTION_FORWARD) || InputDirection(DIRECTION_BACK) || InputDirection(DIRECTION_RIGHT) || InputDirection(DIRECTION_LEFT))
	{
		//  ��
		if (InputDirection(DIRECTION_FORWARD))
		{
			m_Speed.z += ACCELERATION;
		}
		// ��O
		else if (InputDirection(DIRECTION_BACK))
		{
			m_Speed.z -= ACCELERATION;
		}
		// �E
		if (InputDirection(DIRECTION_RIGHT))
		{
			m_Speed.x += ACCELERATION;
		}
		// ��
		else if (InputDirection(DIRECTION_LEFT))
		{
			m_Speed.x -= ACCELERATION;
		}

		// �v���C���[�̊p�x���ړ��ʂ̑傫���Ōv�Z
		float Ang = atan2f(m_Speed.x,m_Speed.z);
		// �J�����̌����Ă�����Ɍ������Ă܂������ɂȂ�悤�ɃJ�����̌������A���O���ɑ���
		CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
		Ang = Ang + pCamera->GetRot().y;
		m_rot.y = Ang;

	}

	Inertia(m_Speed);								// ����
	SpeedAndRotLimit(m_Speed, m_rot, m_fMaxSpeed);	// ���x�Ɖ�]���E
	Quaternion();									// �N�H�[�^�j�I����]
}

//==========================================================================================================
// �N�H�[�^�j�I����]�����֐�
//==========================================================================================================
void CPlayer::Quaternion(void)
{
	D3DXVECTOR3 moveVec = { m_pos - m_Oldpos };			// �ړ��x�N�g��

	// ���x�N�g��(�ړ��x�N�g���Ɛ����̃x�N�g��)
	D3DXVECTOR3 Axis = {0.0f,0.0f,0.0f};
	Axis.x = -moveVec.z;
	Axis.z = moveVec.x;

	if (m_Speed.x != 0.0f || m_Speed.z != 0.0f)
	{
		D3DXQUATERNION QuaAns = { 0.0f,0.0f,0.0f,1.0f };						// �P�ʃN�H�[�^�j�I��
		D3DXQuaternionRotationAxis(&QuaAns, &Axis, D3DXToDegree(m_fAngle));		// ��]�p�x�Ǝ������]�N�H�[�^�j�I�������߂�
		D3DXMatrixRotationQuaternion(&m_mtxRot, &QuaAns);						// �N�H�[�^�j�I�������]�s����쐬����
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
// �����^�ԏ���
//==========================================================================================================
bool CPlayer::Carry(void)
{
	//--------------------------------------------------------------
	// �v���C���[�����ԋ߂����e���擾����
	//--------------------------------------------------------------
	CScene *pSaveScene = nullptr;
	CScene *pScene = CScene::GetScene(OBJTYPE_BOMB);

	float fSaveDist;		// �v���C���[�Ɣ��e�̋�����ۑ�����ϐ�
	while (pScene)
	{
		// ���̃V�[�����擾
		CScene *pSceneNext = CScene::GetSceneNext(pScene);

		if (pScene->GetModelType() == MODTYPE_BOMB)
		{
			float fDist;							// �v���C���[�Ɣ��e�̋��������ϐ�
			D3DXVECTOR3 BombPos = pScene->GetPos();	// �I�u�W�F�N�g�̈ʒu�擾

			// �����𑪂�
			fDist = ((BombPos.x - m_pos.x) * (BombPos.x - m_pos.x)) + ((BombPos.y - m_pos.y) * (BombPos.y - m_pos.y)) + ((BombPos.z - m_pos.z) * (BombPos.z - m_pos.z));

			// �v���C���[�̔��e�擾�͈͓��ɂ���
			if (fDist < CARRY_RANGE_DIST)
			{
				CBomb *pBomb = (CBomb*)pScene;
				// �擾�����I�u�W�F�N�g�̋����ƌ��ݕۑ�����Ă��鋗���𑪂�
				if ((!pSaveScene || fDist < fSaveDist) && !pBomb->GetThrow())
				{
					fSaveDist = fDist;
					pSaveScene = pScene;
				}

			}
		}

		// ���̃V�[�������݂̃V�[���ɂ���
		pScene = pSceneNext;
	}


	//--------------------------------------------------------------
	// �͈͓��ɃI�u�W�F�N�g(���e)�����݂��Ă���ꍇ
	//--------------------------------------------------------------
	if (pSaveScene)
	{
		m_nFrame++;	// �t���[���J�E���g

		// �ʒu�ƍ����̍쐬
		D3DXVECTOR3 pos = pSaveScene->GetPos();
		float fHeight = pSaveScene->GetSize().y * 1.5f;

		// ���ݑI������Ă��锚�e�ɖڈ�(�r���{�[�h)������
		if (!m_pBillboard)
		{
			m_pBillboard = CBillboard::Create({ pos.x,pos.y + fHeight ,pos.z }, MARK_SIZE, { 1.0f,1.0f,1.0f,1.0f });	// �r���{�[�h�̐���
			m_pBillboard->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_ALLOW"));				// �e�N�X�`���̎擾
			m_pBillboard->SetTex(0, MAX_PATTERN);																		// �e�N�X�`���̐ݒ�
		}
		else
		{
			m_pBillboard->CScene::SetPos({ pos.x,pos.y + fHeight ,pos.z });	// �ڈ�̈ʒu�ݒ�
		}

		// ���e������������
		if (CInput::PressAnyAction(CInput::ACTION_ATTACK))
		{
			m_pBomb = (CBomb*)pSaveScene;
			if (m_pBillboard)
			{
				m_pBillboard->Uninit();
				m_pBillboard = nullptr;
			}
			return true;
		}
	}
	else
	{
		// �͈͂ɔ��e���Ȃ��Ȃ�ڈ������
		if (m_pBillboard)
		{
			m_pBillboard->Uninit();
			m_pBillboard = nullptr;
		}
	}
	return false;
}

//==========================================================================================================
// �v���C���[�A�N�V����
//==========================================================================================================
void CPlayer::Action(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//--------------------------------------------------------------
	// ��ԕʏ���
	//--------------------------------------------------------------
	switch (m_state)
	{
	case STATE_NORMAL:	// �ʏ�
		if (Carry())
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CARRYUP);	// �����グ�鉹
			m_state = STATE_HOLD;
		}

		break;

	case STATE_HOLD:	// �ێ�
		if (m_pBomb)
		{
			m_pBomb->SetPos({ m_pos.x,m_pos.y + m_size.y,m_pos.z });

			// �U���{�^���̓���
			if (CInput::PressAnyAction(CInput::ACTION_ATTACK))
			{	
				pSound->PlaySound(CSound::SOUND_LABEL_SE_THROW);	// �����鉹	

				D3DXVECTOR3 moveVec = m_pos - m_Oldpos;				// �ړ��x�N�g�������
				m_pBomb->Throw(moveVec);							// �v���C���[�̈ړ������ɓ�����
				m_pBomb = nullptr;									// ���e��j��(���������͔̂j�����Ȃ�)
				m_state = STATE_NORMAL;								// �v���C���[�̏�Ԃ�ʏ�ɖ߂�
			}
		}
		break;
	}

	// �W�����v
	if (CInput::PressAnyAction(CInput::ACTION_SPACE) && !m_bJump)	// A�{�^��
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_JUMP);			// �W�����v��

		m_fGravity = JUMP;										// �����W�����v�ړ���
		m_bJump = true;											// �W�����v����ON
	}
}

//==========================================================================================================
// �d�͂̏���(�v���C���[�ɏd�͂����炷�l(�����l�͌��炵�������̐�)���������āA��O�����ɓ����)
//==========================================================================================================
void CPlayer::Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool&bJump)
{
	// �d�͉��Z
	if (fGravitySpeed > 0.0f)
	{
		fGravity -= fGravitySpeed;
		pos.y += m_fGravity;
	}

	// ���S����ɂ������ǂ���
	if (pos.y <= DEATH_POSY)
	{
		// �T�E���h�N���X�̎擾
		CSound *pSound = CManager::GetInstance()->GetSound();
		pSound->PlaySound(CSound::SOUND_LABEL_SE_FALL);

		// �ʒu�������ʒu�ɖ߂�
		pos = m_posInit;
		fGravity = 0.0f;
		bJump = false;
	}
}

//==========================================================================================================
// �ړ��������͊֐�
//==========================================================================================================
bool CPlayer::InputDirection(const MOVE_DIRECTION &moveDir)
{
	// ���͌n�N���X�̎擾
	CDInput *pDInput = CManager::GetInstance()->GetDInput();
	CXInput *pXInput = CManager::GetInstance()->GetXInput();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();

	switch (moveDir)
	{
	case DIRECTION_FORWARD:
		return pKey->GetPress(DIK_W) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lY <= -DINPUT_STICK_RANGE;

	case DIRECTION_BACK:
		return pKey->GetPress(DIK_S) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lY >= DINPUT_STICK_RANGE;

	case DIRECTION_LEFT:
		return pKey->GetPress(DIK_A ) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lX <= -DINPUT_STICK_RANGE;

	case DIRECTION_RIGHT:
		return pKey->GetPress(DIK_D) || pXInput->GetGamePad()->m_state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || pDInput->GetGamepad().lX >= DINPUT_STICK_RANGE;

	default:
		assert(moveDir <= -1 || moveDir >= DIRECTION_MAX);
		return false;
	}
}
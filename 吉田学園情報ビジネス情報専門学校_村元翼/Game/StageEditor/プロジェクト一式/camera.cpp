//=============================================================================
// �J�����̍쐬
// Author:������
//=============================================================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "mouse.h"
#include "game.h"
#include "meshfield.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define CAMERA_DISTANCE			(500.0f)	// �J��������
#define CAMERA_DISTANCE_MIN		(10.0f)		// �J���������ŏ�
#define CAMERA_DISTANCE_MAX		(10000.0f)	// �J���������ő�
#define CAMERA_SPEED			(0.04f)		// �J�����ړ����x
#define MOUSE_ROT_SPEED			(0.01f)		// �J������]���x(MOUSE)
#define LONG_SPEED				(10.0f)		// �J�������߈ړ����x
#define POSR_Y_MOVE				(2.0f)		// �J���������_�ړ����x
#define POSR_Y_MOVEHEIGHT		(150.0f)	// �J���������_�������n�߂鍂��
#define _OX_EPSILON_			(0.000001f) // �덷

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
const float CCamera::m_fMoveAmt[AMOUNT_MAX] =
{
	1.0f,
	2.0f,
	5.0f,
	10.0f,
	20.0f,
	50.0f,
	100.0f
};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}
//=============================================================================
// �J�����̏���������
//=============================================================================
HRESULT CCamera::Init(void)
{
	m_nAmtID = 0;
	m_fSpeed = m_fMoveAmt[3];					// �����ړ���(���f���I����)
	m_fSpeedX = 0.0f;								// �����ړ���X(�t�B�[���h�I����)
	m_fSpeedY = 50.0f;								// �����ړ���Y(�t�B�[���h�I����)
	m_fSpeedZ = 0.0f;								// �����ړ���Z(�t�B�[���h�I����)
	m_fLong = CAMERA_DISTANCE;						// ���_�����_�̋���
	m_posV = D3DXVECTOR3(0.0f, 100.0f, -m_fLong);	// �J�������_
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �J���������_
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			// �J�����x�N�g��
	m_rot = D3DXVECTOR3(2.0f, 0.0f, 0.0f);			// �J������]
	m_bPosR_Move = false;
	m_bFieldMove = false;
	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void CCamera::Update(void)
{
	// �J�����̈ړ�
	MoveCamera();
}

//================================================
// �J�����̐�������
//================================================
CCamera *CCamera::Create(D3DXVECTOR3 m_pos, D3DXVECTOR3 rot, float viewportX, float viewportY, float viewportWidth, float viewporHeight, int nID)
{
	//�C���X�^���X�̐���
	CCamera *pCamera = nullptr;
	if (!pCamera)
	{
		pCamera = new CCamera;
		if (pCamera)
		{
			pCamera->Init();	// ������
		}
	}
	return pCamera;
}

//================================================
// �J�����̐ݒ�
//================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�擾

	//�r���[�|�[�g�̐ݒ�
	//pDevice->SetViewport(&m_viewport);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(50.0f),							//�J�����̉�p
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,		//��ʔ䗦!!!�r���[�|�[�g���g���Ƃ��r���[�|�[�g�̕��������g��
								10.0f,											//��O�̕`�搧��
								10000.0f);										//���̕`�搧��

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH
	(
		&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU
	);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//===============================================================
// �J�����̈ړ�
//===============================================================
void CCamera::MoveCamera(void)
{
	// ���̓f�o�C�X�̎擾
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CDInput *pDInput = CManager::GetInstance()->GetDInput();

	//++++++++++++++++++++++++++++++//
	//			���_�̉�]			//
	//++++++++++++++++++++++++++++++//
	if (CInput::PressAnyAction(CInput::ACTION_R_LEFT))
	{	// ���։�]
		m_rot.y += CAMERA_SPEED;
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y = -D3DX_PI;
		}
	}
	if (CInput::PressAnyAction(CInput::ACTION_R_RIGHT))
	{	// �E�։�]
		m_rot.y += -CAMERA_SPEED;
		if (m_rot.y < -D3DX_PI)
		{
			m_rot.y = D3DX_PI;
		}
	}

	if (CInput::PressAnyAction(CInput::ACTION_R_DOWN))
	{	// ���։�]
		m_rot.x += -CAMERA_SPEED;
		if (m_rot.x < 0.0f + D3DX_PI / 2)
		{
			m_rot.x = 0.0f + D3DX_PI / 2 + _OX_EPSILON_;
		}
	}
	if (CInput::PressAnyAction(CInput::ACTION_R_UP))
	{	// ��։�]
		m_rot.x += CAMERA_SPEED;
		if (m_rot.x > D3DX_PI)
		{
			m_rot.x = D3DX_PI - _OX_EPSILON_;
		}
	}

	//****************************//
	//	�@	GamePad�ł̃Y�[��	  //
	//****************************//
	// �J�������߂Â���
	if (pDInput->GetPress(pDInput->BUTTON_R2))
	{
		m_fLong -= LONG_SPEED;
		if (m_fLong <= CAMERA_DISTANCE_MIN)
		{
			m_fLong = CAMERA_DISTANCE_MIN;
		}
	}

	// �J��������������
	else if (pDInput->GetPress(pDInput->BUTTON_L2))
	{
		m_fLong += LONG_SPEED;
		if (m_fLong >= CAMERA_DISTANCE_MAX)
		{
			m_fLong = CAMERA_DISTANCE_MAX;
		}
	}

	//****************************//
	//	�@	�}�E�X�ł̉�]		  //
	//****************************//
	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	float CameraPosRX = (float)pMouse->GetMousePos().lX;	// �}�E�X�J�[�\���̉���
	float CameraPosRZ = (float)pMouse->GetMousePos().lY;	// �}�E�X�J�[�\���̏c��
	float MouseWheel = (float)pMouse->GetMousePos().lZ;		// �}�E�X�z�C�[���̉�]

	if (pMouse->GetPress(CMouse::MOUSE_RIGHT))
	{
		// �}�E�X�̈ʒu�Œ�&�J�[�\�����\���ɂ���
		SetCursorPos(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		//ShowCursor(false);

		// ����]
		if (CameraPosRZ > 0.0f || CameraPosRZ < -0.0f)
		{
			m_rot.x += (MOUSE_ROT_SPEED / 3) * CameraPosRZ;
		}

		// �c��]
		if (CameraPosRX > 0.0f || CameraPosRX < -0.0f)
		{
			m_rot.y += (MOUSE_ROT_SPEED / 3) * CameraPosRX;
		}
	}

	//****************************//
	//	�@	�}�E�X�ł̃Y�[��	  //
	//****************************//
	// �J�������߂Â���
	if (MouseWheel > 0.0f)
	{
		m_fLong -= LONG_SPEED * LONG_SPEED;
		if (m_fLong <= CAMERA_DISTANCE_MIN)
		{
			m_fLong = CAMERA_DISTANCE_MIN;
		}
	}

	// �J��������������
	else if (MouseWheel < 0.0f)
	{
		m_fLong += LONG_SPEED * LONG_SPEED;
		if (m_fLong >= CAMERA_DISTANCE_MAX)
		{
			m_fLong = CAMERA_DISTANCE_MAX;
		}
	}

	//++++++++++++++++++++++++++++//
	//		�J������]���E		  //
	//++++++++++++++++++++++++++++//
	// ����]
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}

	// �c��]
	if (m_rot.x <  D3DXToRadian(90.0f))
	{
		m_rot.x = D3DXToRadian(90.0f);
	}
	else if (m_rot.x > D3DXToRadian(170.0f))
	{
		m_rot.x = D3DXToRadian(170.0f);
	}

	//++++++++++++++++++++++++++++//
	//		�J�����ړ��ʕύX	  //
	//++++++++++++++++++++++++++++//
	// �t�B�[���h��u���Ƃ��̈ړ�
	CMeshField *pMeshField = CManager::GetGame()->GetFieldPrev();
	if (pMeshField)
	{
		if (!m_bFieldMove)
		{
			m_bFieldMove = true;

			// ����2���ȉ����l�̌ܓ����ăL���̗ǂ����l�ɂ���
			m_posR.x = roundf(m_posR.x / 100) * 100;	
			m_posR.z = roundf(m_posR.z / 100) * 100;
		}

		m_fSpeedX = pMeshField->GetSize().x;
		m_fSpeedZ = pMeshField->GetSize().z;

		//++++++++++++++++++++++++++++//
		//		  �J�����̈ړ�	      //
		//++++++++++++++++++++++++++++//
		// ���ړ�
		if (pKey->GetTrigger(DIK_A))
		{
			m_posV.x -= m_fSpeedX;
			m_posR.x -= m_fSpeedX;
		}

		// �E�ړ�
		else if (pKey->GetTrigger(DIK_D))
		{
			m_posV.x += m_fSpeedX;
			m_posR.x += m_fSpeedX;
		}

		// ���ړ�
		if (pKey->GetTrigger(DIK_W))
		{
			m_posV.z += m_fSpeedZ;
			m_posR.z += m_fSpeedZ;
		}

		// ��O�ړ�
		else if (pKey->GetTrigger(DIK_S))
		{
			m_posV.z -= m_fSpeedZ;
			m_posR.z -= m_fSpeedZ;
		}

		// ��ړ�
		if (pKey->GetTrigger(DIK_T))
		{
			m_posR.y += m_fSpeed;
			m_posV.y += m_fSpeed;
		}

		// ���ړ�
		else if (pKey->GetTrigger(DIK_G))
		{
			m_posR.y -= m_fSpeed;
			m_posV.y -= m_fSpeed;
		}
	}

	// ���f����u���Ƃ��̈ړ�
	else
	{
		if (m_bFieldMove)
		{
			m_bFieldMove = false;
			m_fSpeed = m_fMoveAmt[0];
		}

		//++++++++++++++++++++++++++++//
		//		  �J�����̈ړ�	      //
		//++++++++++++++++++++++++++++//
		// ���ړ�
		if (pKey->GetPress(DIK_A))
		{
			m_posV.x -= m_fSpeed;
			m_posR.x -= m_fSpeed;
		}

		// �E�ړ�
		else if (pKey->GetPress(DIK_D))
		{
			m_posV.x += m_fSpeed;
			m_posR.x += m_fSpeed;
		}

		// ���ړ�
		if (pKey->GetPress(DIK_W))
		{
			m_posV.z += m_fSpeed;
			m_posR.z += m_fSpeed;
		}

		// ��O�ړ�
		else if (pKey->GetPress(DIK_S))
		{
			m_posV.z -= m_fSpeed;
			m_posR.z -= m_fSpeed;
		}

		// ��ړ�
		if (pKey->GetPress(DIK_T))
		{
			m_posR.y += m_fSpeed;
			m_posV.y += m_fSpeed;
		}

		// ���ړ�
		else if (pKey->GetPress(DIK_G))
		{
			m_posR.y -= m_fSpeed;
			m_posV.y -= m_fSpeed;
		}


		if ((pKey->GetTrigger(DIK_L) || pKey->GetTrigger(DIK_O)))
		{
			if (pKey->GetTrigger(DIK_L))
			{
				m_nAmtID++;
				if (m_nAmtID >= AMOUNT_MAX)
				{
					m_nAmtID = 0;
				}
			}

			else if (pKey->GetTrigger(DIK_O))
			{
				m_nAmtID--;
				if (m_nAmtID < AMOUNT_00)
				{
					m_nAmtID = AMOUNT_MAX - 1;
				}
			}

			// �ړ��ʕύX
			m_fSpeed = m_fMoveAmt[m_nAmtID];
		}
	}

	// ���_�̏ꏊ�𒍎��_�����Ɉړ�(�ɍ��W��])
	m_posV.x = m_posR.x - m_fLong * sinf(m_rot.x) * sinf(m_rot.y);
	m_posV.z = m_posR.z - m_fLong * sinf(m_rot.x) * cosf(m_rot.y);
	m_posV.y = m_posR.y - m_fLong * cosf(m_rot.x);
}
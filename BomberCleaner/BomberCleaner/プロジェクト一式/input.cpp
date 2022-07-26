//====================================
// ���͏���
//====================================
#include "input.h"
#include "manager.h"
#include "keyboard.h"
#include "XInput.h"
#include "mouse.h"
#include "DirectInput.h"

//====================================
// �}�N����`
//====================================

//====================================
// �ÓI�����o�ϐ��̏�����
//====================================
LPDIRECTINPUT8 CInput::m_pInput = nullptr;	// DirectInput�I�u�W�F�N�g�̃|�C���^
LPDIRECTINPUTDEVICE8 CInput::m_pJoystick = nullptr;

//====================================
// �R���X�g���N�^
//====================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//====================================
// �f�X�g���N�^
//====================================
CInput::~CInput()
{

}

//====================================
// ����������
//====================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���͂̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//====================================
// �I������
//====================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X�̊J��
	if (m_pDevice)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	// ���̓f�o�C�X�̊J��
	if (m_pJoystick)
	{
		m_pJoystick->Unacquire();
		m_pJoystick->Release();
		m_pJoystick = nullptr;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}

bool CInput::PressAnyAction(const ACTION &action)
{
	// ���͌n�N���X�̎擾
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CXInput *pXPad = CManager::GetInstance()->GetXInput();
	CDInput *pDPad = CManager::GetInstance()->GetDInput();
	CMouse *pMouse = CManager::GetInstance()->GetMouse();

	switch (action)
	{
	case ACTION_ENTER:	// ����E�|�[�Y�ȂǂɎg��
		return pKey->GetTrigger(DIK_RETURN) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_START) || pDPad->GetTrigger(pDPad->BUTTON_START);

	case ACTION_UP:		// �㏸�Ȃ�
		return pKey->GetTrigger(DIK_UP) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) || pDPad->GetGamepad().lY <= -DINPUT_STICK_RANGE ||
			pKey->GetTrigger(DIK_W) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ACTION_DOWN:	// ���~�Ȃ�
		return pKey->GetTrigger(DIK_DOWN) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN) || pDPad->GetGamepad().lY >= DINPUT_STICK_RANGE ||
			pKey->GetTrigger(DIK_S) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		
	case ACTION_SPACE:	// �L�����Z���E�W�����v�Ȃ�
		return (pKey->GetTrigger(DIK_SPACE) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_A) || pDPad->GetTrigger(CDInput::BUTTON_A));

	case ACTION_ATTACK:	// �U���Ȃ�
		return (pKey->GetTrigger(DIK_K) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_B) || pDPad->GetTrigger(CDInput::BUTTON_B)|| pMouse->GetTrigger(CMouse::MOUSE_LEFT));

	case ACTION_LEFT:	// ���s��
		return (pKey->GetTrigger(DIK_A) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT) || pDPad->GetGamepad().lX <= -DINPUT_STICK_RANGE) ||
				pKey->GetTrigger(DIK_LEFT) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ACTION_RIGHT:	// �E�s��
		return (pKey->GetTrigger(DIK_D) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) || pDPad->GetGamepad().lX >= DINPUT_STICK_RANGE) ||
				pKey->GetTrigger(DIK_RIGHT) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case ACTION_R_UP:	// ���]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRy >= DINPUT_STICK_RANGE;

	case ACTION_R_DOWN:	// ����]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRy <= -DINPUT_STICK_RANGE;

	case ACTION_R_RIGHT: // �E��]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRx <= -DINPUT_STICK_RANGE;

	case ACTION_R_LEFT:	// ����]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRx >= DINPUT_STICK_RANGE;
	}

	return false;
}



//==================================================
// ���͌n�f�o�C�X�̊Ǘ�
// Author:������
//==================================================
#include "InputManager.h"
#include "manager.h"
#include "keyboard.h"
#include "XInput.h"
#include "DirectInput.h"
#include "mouse.h"
#include <assert.h>

//==================================================
// �R���X�g���N�^
//==================================================
CInputManager::CInputManager()
{

}

//==================================================
// �f�X�g���N�^
//==================================================
CInputManager::~CInputManager()
{

}

//==================================================
// ������
//==================================================
HRESULT CInputManager::Init()
{
	return S_OK;
}

//==================================================
// ���͌n�N���X�̃v���X���
//==================================================
bool CInputManager::PressAnyAction(const PRESS &press)
{
	// ���̓f�o�C�X�̎擾
	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CXInput *pXPad = CManager::GetInstance()->GetXInput();
	CDInput *pDPad = CManager::GetInstance()->GetDInput();

	switch (press)
	{
	case PRESS_ENTER:	// ����E�|�[�Y�ȂǂɎg��
		return	pKey->GetPress(DIK_RETURN) || pXPad->GetButtonPress(XINPUT_GAMEPAD_START) || pDPad->GetPress(pDPad->BUTTON_START);

	case PRESS_SPACE:	// �L�����Z���E�W�����v�Ȃ�
		return pMouse->GetPress(CMouse::MOUSE_LEFT) || pKey->GetPress(DIK_SPACE) || pXPad->GetButtonPress(XINPUT_GAMEPAD_A) || pDPad->GetPress(CDInput::BUTTON_A);

	case PRESS_ATTACK:	// �U���Ȃ�
		return pKey->GetPress(DIK_K) || pXPad->GetButtonPress(XINPUT_GAMEPAD_B) || pDPad->GetPress(CDInput::BUTTON_B);

	case PRESS_UP:		// �㏸�Ȃ�
		return	pKey->GetPress(DIK_UP) || pXPad->GetButtonPress(XINPUT_GAMEPAD_DPAD_UP) || pDPad->GetGamepad().lY <= -DINPUT_STICK_RANGE ||
				pKey->GetPress(DIK_W)  || pXPad->GetGamePad()->m_state.Gamepad.sThumbLY >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case PRESS_DOWN:	// ���~�Ȃ�
		return	pKey->GetPress(DIK_DOWN) || pXPad->GetButtonPress(XINPUT_GAMEPAD_DPAD_DOWN) || pDPad->GetGamepad().lY >= DINPUT_STICK_RANGE ||
				pKey->GetPress(DIK_S)	 || pXPad->GetGamePad()->m_state.Gamepad.sThumbLY <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case PRESS_LEFT:	// ���s��
		return  pKey->GetPress(DIK_A)	 || pXPad->GetButtonPress(XINPUT_GAMEPAD_DPAD_LEFT) || pDPad->GetGamepad().lX <= -DINPUT_STICK_RANGE ||
				pKey->GetPress(DIK_LEFT) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case PRESS_RIGHT:	// �E�s��
		return  pKey->GetPress(DIK_D)	  || pXPad->GetButtonPress(XINPUT_GAMEPAD_DPAD_RIGHT) || pDPad->GetGamepad().lX >= DINPUT_STICK_RANGE ||
				pKey->GetPress(DIK_RIGHT) || pXPad->GetGamePad()->m_state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	case PRESS_R_UP:	// ���]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRY <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRy >= DINPUT_STICK_RANGE;

	case PRESS_R_DOWN:	// ����]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRY >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRy <= -DINPUT_STICK_RANGE;

	case PRESS_R_RIGHT: // �E��]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRx <= -DINPUT_STICK_RANGE;

	case PRESS_R_LEFT:	// ����]�Ȃ�
		return pXPad->GetGamePad()->m_state.Gamepad.sThumbRX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE || pDPad->GetGamepad().lRx >= DINPUT_STICK_RANGE;

	default:
		assert(press < PRESS_ENTER || press > PRESS_MAX);	// �\���̂ɑ��݂��Ȃ��ԍ���I�������ꍇ�G���[
		break;
	}

	return false;
}

//==================================================
// ���͌n�N���X�̃g���K�[���
//==================================================
bool CInputManager::TriggerAnyAction(const TRIGGER &trigger)
{

	CMouse *pMouse = CManager::GetInstance()->GetMouse();
	CInputkeyboard *pKey = CManager::GetInstance()->GetKeyboard();
	CXInput *pXPad = CManager::GetInstance()->GetXInput();
	CDInput *pDPad = CManager::GetInstance()->GetDInput();

	switch (trigger)
	{
	case TRIGGER_ENTER:	// ����E�|�[�Y�ȂǂɎg��
		return	pKey->GetTrigger(DIK_RETURN) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_START) || pDPad->GetTrigger(pDPad->BUTTON_START);

	case TRIGGER_SPACE:	// �L�����Z���E�W�����v�Ȃ�
		return  pKey->GetTrigger(DIK_SPACE) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_A) || pDPad->GetTrigger(CDInput::BUTTON_A) || pMouse->GetTrigger(CMouse::MOUSE_LEFT);

	case TRIGGER_ATTACK:// �U���Ȃ�
		return	pKey->GetTrigger(DIK_K) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_B) || pDPad->GetTrigger(CDInput::BUTTON_B);

	case TRIGGER_UP:	// �㏸�Ȃ�
		return	pKey->GetTrigger(DIK_UP) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_UP) ||
				pKey->GetTrigger(DIK_W);

	case TRIGGER_DOWN:	// ���~�Ȃ�
		return	pKey->GetTrigger(DIK_DOWN) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_DOWN)||
				pKey->GetTrigger(DIK_S);

	case TRIGGER_LEFT:	// ���s��
		return  pKey->GetTrigger(DIK_A) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_LEFT) ||
				pKey->GetTrigger(DIK_LEFT);

	case TRIGGER_RIGHT:	// �E�s��
		return  pKey->GetTrigger(DIK_D) || pXPad->GetButtonTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) ||
				pKey->GetTrigger(DIK_RIGHT);

	default:
		assert(trigger < TRIGGER_ENTER || trigger > TRIGGER_MAX);	// �\���̂ɑ��݂��Ȃ��ԍ���I�������ꍇ�G���[
		break;
	}

	return false;
}
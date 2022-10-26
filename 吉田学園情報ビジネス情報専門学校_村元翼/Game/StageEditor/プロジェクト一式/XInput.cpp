//=============================================================================
// �Q�[���p�b�h����̏���
// Author:������
//=============================================================================
#include <Windows.h>
#include "XInput.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CXInput::CXInput()
{
	// ���͏��E�o�C�u���[�V����������
	ZeroMemory(&m_GamePad.m_state, sizeof(XINPUT_STATE));
	ZeroMemory(&m_GamePad.m_stateLast, sizeof(XINPUT_STATE));
	ZeroMemory(&m_GamePad.m_vibration, sizeof(XINPUT_VIBRATION));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CXInput::~CXInput()
{
	// Xinput���I��
	XInputEnable(false); // BOOL enable
}

//=============================================================================
// �X�V����
//=============================================================================
void CXInput::UpdateGamepad(void)
{
	XINPUT_STATE state;

	DWORD dwResult = XInputGetState(0, &state);

	if (dwResult == ERROR_SUCCESS)
	{
		m_GamePad.m_stateLast = m_GamePad.m_state;
		m_GamePad.m_state = state;
	}
}

//=============================================================================
// �{�^���̃g���K�[���擾
//=============================================================================
bool CXInput::GetButtonTrigger(int nButton)
{
	return (m_GamePad.m_stateLast.Gamepad.wButtons & nButton) == false && (m_GamePad.m_state.Gamepad.wButtons & nButton) ? true : false;
}

//=============================================================================
// �{�^���̃v���X���擾
//=============================================================================
bool CXInput::GetGamepadPress(int nButton)
{
	return (m_GamePad.m_state.Gamepad.wButtons & nButton);
}

//=============================================================================
// XInput�̎擾
//=============================================================================
CXInput::GAMEPAD *CXInput::GetGamePad(void)
{
	return &m_GamePad;
}
//===============================================
// DirectInput�̏���
// Author:������
//===============================================
#include "DirectInput.h"

//===============================================
// �ÓI�����o�ϐ��̏�����
//===============================================
const char *CDInput::m_aStrR = nullptr;
const char *CDInput::m_aStrL = nullptr;

//===============================================
// �R���X�g���N�^
//===============================================
CDInput::CDInput()
{

}

//===============================================
// �f�X�g���N�^
//===============================================
CDInput::~CDInput()
{

}

//===============================================
// ������
//===============================================
HRESULT CDInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏�����
	CInput::Init(hInstance, hWnd);

	// �f�o�C�X�̗�(�Q�[���p�b�h)
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, &DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	if (m_pJoystick != nullptr)
	{
		// �������[�h��ݒ�
		if (FAILED(m_pJoystick->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		m_pJoystick->GetCapabilities(&m_pJoystickCaps);

		// ���̐ݒ�
		SetAxis(hWnd);

		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_pJoystick->Acquire();

		// �|�[�����O(����I�Ƀf�o�C�X�̒l���擾����֐�)
		if (FAILED(m_pJoystick->Poll()))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//===============================================
// �I��
//===============================================
void CDInput::Uninit()
{
	CInput::Uninit();
}

//===============================================
// �X�V
//===============================================
void CDInput::Update()
{
	// �Q�[���p�b�h�̓��͏��̍\����
	DIJOYSTATE state;
	ZeroMemory(&state, sizeof(state));

	if (m_pJoystick != nullptr)
	{
		// �|�[�����O
		m_pJoystick->Poll();

		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pJoystick->GetDeviceState(sizeof(state), &state)))
		{
			PrintState();

			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				m_aStateTrigger.rgbButtons[nCntKey] = (m_aState.rgbButtons[nCntKey] ^ state.rgbButtons[nCntKey]) & state.rgbButtons[nCntKey];
				m_aStateRelease.rgbButtons[nCntKey] = (m_aState.rgbButtons[nCntKey] ^ state.rgbButtons[nCntKey]) & ~state.rgbButtons[nCntKey];
			}
			m_aState = state;
		}

		else
		{
			m_pJoystick->Acquire();
		}
	}
}

//=============================================================================
// �{�^���̃g���K�[���擾
//=============================================================================
bool CDInput::GetTrigger(int nButton)
{
	return (m_aStateTrigger.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �{�^���̃v���X���擾
//=============================================================================
bool CDInput::GetPress(int nButton)
{
	return (m_aState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// �{�^���̃����[�X���擾
//=============================================================================
bool CDInput::GetRelease(int nButton)
{
	return (m_aStateRelease.rgbButtons[nButton] & 0x80) ? true : false;
}

//===============================================
// �R�[���o�b�N�֐�
//===============================================
BOOL CALLBACK CDInput::DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	// �f�o�C�X����
	if (FAILED(m_pInput->CreateDevice(ipddi->guidInstance, &m_pJoystick, NULL)))
	{
		m_pJoystick->Release();
		return DIENUM_STOP;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pJoystick->SetDataFormat(&c_dfDIJoystick)))
	{
		m_pJoystick->Release();
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;	// �R���g���[���[��2�ȏ�̏ꍇ��DIENUM_CONTINUE
}

BOOL CALLBACK CDInput::EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef)
{
	//------------------------------------
	// ���͈͐ݒ�
	//------------------------------------
	DIPROPRANGE diprg;								// �錾
	ZeroMemory(&diprg, sizeof(diprg));				// ������
	diprg.diph.dwSize = sizeof(diprg);				// �T�C�Y�ݒ�
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);	// �T�C�Y�ݒ�
	diprg.diph.dwHow = DIPH_BYID;					// �Ώ�
	diprg.diph.dwObj = lpddoi->dwType;				// �ΏۂɂȂ鎲
	diprg.lMin = -DINPUT_STICK_RANGE;				// �ŏ��l
	diprg.lMax = DINPUT_STICK_RANGE;				// �ő�l
	if (FAILED(m_pJoystick->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}


//===============================================
// ���ݒ�֐�
//===============================================
void CDInput::SetAxis(HWND hWnd)
{
	//------------------------------------
	// �����[�h�ݒ�
	//------------------------------------
	DIPROPDWORD diprop;								// �錾
	ZeroMemory(&diprop, sizeof(diprop));			// �T�C�Y������
	diprop.diph.dwSize = sizeof(diprop);			// �g�p����\���̂̃T�C�Y��ݒ�
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);	// �g�p����\���̂̃T�C�Y��ݒ�
	diprop.diph.dwHow = DIPH_DEVICE;				// dwObj�ɂȂ�̒l�������Ă邩�̔���
	diprop.diph.dwObj = 0;							// DIPH_DIVICE�Ȃ�0����
	diprop.dwData = DIPROPAXISMODE_ABS;				// ��Βl���[�h�̎w��(DIPROPAXISMODE_REL�ɂ����瑊�Βl)

	// �����[�h��ύX(�ݒ�)
	if (FAILED(m_pJoystick->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		throw std::runtime_error("�����[�h�̐ݒ�Ɏ��s");
	}

	// �I�u�W�F�N�g�̗�
	m_pJoystick->EnumObjects(EnumAxesCallback, (void*)hWnd, DIDFT_AXIS);
}


//===============================================
// ���͕����m�F
//===============================================
void CDInput::PrintState(void)
{
	if (m_aState.lX >= DINPUT_STICK_RANGE)
	{
		PrintLeftState("��");
	}
	else if (m_aState.lX <= -DINPUT_STICK_RANGE)
	{
		PrintLeftState("��");
	}
	else if (m_aState.lY >= DINPUT_STICK_RANGE)
	{
		PrintLeftState("��");
	}
	else if (m_aState.lY <= -DINPUT_STICK_RANGE)
	{
		PrintLeftState("��");
	}
	else
	{
		PrintLeftState("�Z");
	}

	if (m_aState.lRx >= DINPUT_STICK_RANGE)
	{
		PrintRightState("��");
	}
	else if (m_aState.lRx <= -DINPUT_STICK_RANGE)
	{
		PrintRightState("��");
	}
	else if (m_aState.lRy >= DINPUT_STICK_RANGE)
	{
		PrintRightState("��");
	}
	else if (m_aState.lRy <= -DINPUT_STICK_RANGE)
	{
		PrintRightState("��");
	}
	else
	{
		PrintRightState("�Z");
	}
}



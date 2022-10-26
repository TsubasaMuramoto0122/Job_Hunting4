//====================================
// ���͏���
//====================================
#include "input.h"

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



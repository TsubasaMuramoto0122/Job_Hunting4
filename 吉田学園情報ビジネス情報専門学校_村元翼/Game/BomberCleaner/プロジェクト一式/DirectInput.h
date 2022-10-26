//===============================================
// DirectInput�̏���
// Author:������
//===============================================
#ifndef _DIRECTINPUT_H_
#define _DIRECTINPUT_H_

//===============================================
// �C���N���[�h�t�@�C��
//===============================================
#include "main.h"
#include "input.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_GAMEPAD (1)				// �Q�[���p�b�h�̐�
#define DINPUT_STICK_RANGE (1000)	// �X�e�B�b�N�̌X���͈�

//===============================================
// �O���錾
//===============================================

//===============================================
// DirectInput�N���X�̒�`
//===============================================
class CDInput : public CInput
{
public:
	//------------------------------
	// �{�^����
	//------------------------------
	typedef enum
	{
		BUTTON_A = 0,			// A�{�^��
		BUTTON_B,				// B�{�^��
		BUTTON_Y,				// Y�{�^��
		BUTTON_X,				// X�{�^��
		BUTTON_L2,				// L2�{�^��
		BUTTON_R2,				// R2�{�^��
		BUTTON_L,				// L�{�^��
		BUTTON_R,				// R�{�^��
		BUTTON_START,			// START�{�^��
		BUTTON_SELECT,			// SELECT�{�^��
		BUTTON_L3,				// L3�{�^��
		BUTTON_R3,				// R3�{�^��
		BUTTON_BUTTOM,			// BUTTOM�{�^��
		BUTTON_HOME,			// HOME�{�^��
		BUTTON_MAX				// �{�^����

	}DINPUT_BUTTON;

	CDInput();												// �R���X�g���N�^
	~CDInput();												// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);			// ������
	void Uninit(void);										// �I��
	void Update(void);										// �X�V

	DIJOYSTATE GetGamepad(void)		{ return m_aState; }	// �W���C�p�b�h��Ԃ̎擾
	bool GetPress(int nButton);								// ���������͏��̎擾
	bool GetTrigger(int nButton);							// �P�������͏��̎擾
	bool GetRelease(int nButton);							// �������͏��̎擾

	void SetAxis(HWND hWnd);								// ���ݒ�֐�

	static BOOL CALLBACK DeviceFindCallBack(const LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);			// DInput�񋓃R�[���o�b�N
	static BOOL CALLBACK EnumAxesCallback(const LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);	// DInput���񋓃R�[���o�b�N

	void PrintState(void);
	static void PrintLeftState(const char *aStr)	{ m_aStrL = aStr; }
	static void PrintRightState(const char *aStr)	{ m_aStrR = aStr; }
	static const char *GetStateR()					{ return m_aStrR; }
	static const char *GetStateL()					{ return m_aStrL; }

private:
	DIDEVCAPS m_pJoystickCaps;		// 
	DIJOYSTATE m_aState;			// �Q�[���p�b�h�̓��͏��i�v���X���j
	DIJOYSTATE m_aStateTrigger;		// �Q�[���p�b�h�̓��͏�� (�g���K�[)
	DIJOYSTATE m_aStateRelease;		// �Q�[���p�b�h�̓��͏�� (�����[�X)
	const static char *m_aStrR;
	const static char *m_aStrL;

};
#endif //_DIRECTINPUT_H_
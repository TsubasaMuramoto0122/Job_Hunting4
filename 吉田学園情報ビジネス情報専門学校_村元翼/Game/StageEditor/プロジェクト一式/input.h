//---------------------
// ���͏���
//---------------------
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

class CInput
{
public:
	//------------------------------------
	// ���͏���
	//------------------------------------
	typedef enum
	{
		ACTION_ENTER = 0,
		ACTION_SPACE,
		ACTION_ATTACK,
		ACTION_UP,
		ACTION_DOWN,
		ACTION_RIGHT,
		ACTION_LEFT,
		ACTION_R_UP,
		ACTION_R_DOWN,
		ACTION_R_RIGHT,
		ACTION_R_LEFT,
		ACTION_MAX

	}ACTION;

	static bool PressAnyAction(const ACTION &action);

	CInput();
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:
	static LPDIRECTINPUT8			m_pInput;		// DirectInput�I�u�W�F�N�g�̃|�C���^
	static LPDIRECTINPUTDEVICE8		m_pJoystick;	// �W���C�X�e�B�b�N�p�f�o�C�X
	LPDIRECTINPUTDEVICE8			m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^
};

#endif //_INPUT_H_

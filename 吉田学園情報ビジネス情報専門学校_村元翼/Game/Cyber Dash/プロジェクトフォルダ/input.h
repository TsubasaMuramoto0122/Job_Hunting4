//======================================================
// ���͏���
// Author:������
//======================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//======================================================
// ���͏����̃N���X
//======================================================
class CInput
{
public:
	CInput();												   // �R���X�g���N�^
	virtual ~CInput();										   // �f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	   // ������
	virtual void Uninit(void);								   // �I��
	virtual void Update(void) = 0;							   // �X�V

protected:
	static LPDIRECTINPUT8			m_pInput;		// DirectInput�I�u�W�F�N�g�̃|�C���^
	static LPDIRECTINPUTDEVICE8		m_pJoystick;	// �W���C�X�e�B�b�N�p�f�o�C�X
	LPDIRECTINPUTDEVICE8			m_pDevice;		// ���̓f�o�C�X�ւ̃|�C���^
};

#endif

//==================================================
// ���͌n�f�o�C�X�̊Ǘ�
// Author:������
//==================================================
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_
#include "main.h"

//==================================================
// �N���X��`
//==================================================
class CInputManager
{
public:
	//------------------------------------
	// ���͏���
	//------------------------------------
	typedef enum
	{
		PRESS_ENTER = 0,
		PRESS_SPACE,
		PRESS_ATTACK,
		PRESS_UP,
		PRESS_DOWN,
		PRESS_RIGHT,
		PRESS_LEFT,
		PRESS_R_UP,
		PRESS_R_DOWN,
		PRESS_R_RIGHT,
		PRESS_R_LEFT,
		PRESS_MAX

	}PRESS;

	typedef enum
	{
		TRIGGER_ENTER = 0,
		TRIGGER_SPACE,
		TRIGGER_ATTACK,
		TRIGGER_UP,
		TRIGGER_DOWN,
		TRIGGER_RIGHT,
		TRIGGER_LEFT,
		TRIGGER_MAX

	}TRIGGER;

	CInputManager();	// �R���X�g���N�^
	~CInputManager();	// �f�X�g���N�^
	HRESULT Init(void);	// ������

	static bool PressAnyAction(const PRESS &press);			// �v���X���
	static bool TriggerAnyAction(const TRIGGER &trigger);	// �g���K�[���

};

#endif
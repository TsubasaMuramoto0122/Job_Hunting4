//========================================================
// �L�[�{�[�h�̓��͏���
// Author:������
//========================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"
#include "input.h"

//========================================================
// �}�N����`
//========================================================
#define NUM_KEY_MAX (256)		// �L�[�̍ő吔

//========================================================
// �L�[�{�[�h�̃N���X
//========================================================
class CInputkeyboard : public CInput
{
public:
	CInputkeyboard();									// �R���X�g���N�^
	~CInputkeyboard();									// �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V

	bool GetPress(int nKey);							// �v���X���擾
	bool GetTrigger(int Key);							// �g���K�[���擾

private:
	BYTE m_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̓��͏��(�v���X���)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̓��͏��(�g���K�[���)

};
#endif

//*****************************************************************************
// manager.h�̍쐬
// Author:������
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C��
//*****************************************************************************
#include "main.h"
#include "camera.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CLight;
class CTexture;
class CInputkeyboard;
class CGame;
class CFade;
class CXInput;
class CDInput;
class CLoadX;
class CMouse;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIGHT		(4)

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{
public:
	//----------------------------------
	// ���[�h�ύX�̃X�R�[�v�t����
	//----------------------------------
	enum class MODE : int
	{
		TITLE = 0,
		GAME,
		RESULT,
		MAX
	};

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bReturn);	// ������
	void Uninit(void);											// �I��
	void Update(void);											// �X�V
	void Draw(void);											// �`��

	static bool				SetModeBool(MODE modeNext);								// ���[�h�ݒ蔻��
	static void				SetMode(MODE mode);										// ���[�h�ݒ�
	static CManager			*GetInstance(void);										// �C���X�^���X�擾
	static CRenderer		*GetRenderer()			{ return m_pRenderer; }			// �����_���[�̎擾
	static CInputkeyboard	*GetKeyboard()			{ return m_pInputKeyboard; }	// �L�[�{�[�h�̎擾
	static CXInput			*GetXInput()			{ return m_pXInput; }			// �p�b�h(X)�擾����
	static CDInput			*GetDInput()			{ return m_pDInput; }			// �p�b�h(D)�擾����
	static CTexture			*GetTexture()			{ return m_pTexture; }			// �e�N�X�`���̎擾
	static CCamera			*GetCamera(int nNum)	{ return m_apCamera[nNum]; }	// �J�����̎擾
	static CLoadX			*GetLoadX()				{ return m_pLoadX; }			// X�ǂݍ��݃N���X�̎擾

	static CFade			*GetFade()				{ return m_pFade; }				// �t�F�[�h�̎擾
	static CGame			*GetGame()				{ return m_pGame; }				// �Q�[���N���X�̎擾
	static MODE				Getmode()				{ return m_mode; }				// ���[�h�̎擾
	static CMouse			*GetMouse()				{ return m_pMouse; }			// �}�E�X�̎擾
					
	static HWND GethWnd()							{ return m_hWnd; }							// �E�B���h�E�n���h���̎擾

private:
	CManager();												// �R���X�g���N�^
	~CManager();											// �f�X�g���N�^

	static CManager			*s_pManager;					// �V���O���g���}�l�[�W���[�̃N���X
	static CRenderer		*m_pRenderer;					// �����_���[�̃N���X
	static CTexture			*m_pTexture;					// �e�N�X�`���̃N���X
	static CInputkeyboard	*m_pInputKeyboard;				// �L�[�{�[�h�̃N���X
	static CXInput			*m_pXInput;						// �p�b�h(X)�̃N���X
	static CDInput			*m_pDInput;						// �p�b�h(D)�̃N���X
	static CCamera			*m_apCamera[MAX_CAMERA];		// �J�����̃N���X
	static CLight			*m_pLight[MAX_LIGHT];			// ���C�g�̃N���X
	static CFade			*m_pFade;						// �t�F�[�h�N���X
	static CGame			*m_pGame;						// �Q�[���N���X
	static MODE				m_mode;							// ���[�h�̍\����
	static CLoadX			*m_pLoadX;						// X�ǂݍ��݃N���X
	static CMouse			*m_pMouse;						// �}�E�X�N���X
	static bool				m_bOnlyOnce;					// �����[�h�J�ڂ̍ۂ̘A���t�F�[�h�h�~
	static HWND				m_hWnd;							// �E�B���h�E�n���h��
};
#endif

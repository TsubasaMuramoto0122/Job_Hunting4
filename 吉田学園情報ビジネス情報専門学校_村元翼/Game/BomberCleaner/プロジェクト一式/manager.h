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
class CTitle;
class CGame;
class CResult;
class CFade;
class CPause;
class CXInput;
class CDInput;
class CPlayData;
class CLoadX;
class CMouse;
class CSound;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIGHT		(4)
#define MOVELIMIT		(1450.0f)	// �ړ����E

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
	static CPlayData		*GetPlayData()			{ return m_pPlayData; }			// �v���C�f�[�^�̎擾
	static CPause			*GetPause(void)			{ return m_pPause; }			// �|�[�Y�̎擾
	static CFade			*GetFade()				{ return m_pFade; }				// �t�F�[�h�̎擾
	static CGame			*GetGame()				{ return m_pGame; }				// �Q�[���N���X�̎擾
	static MODE				Getmode()				{ return m_mode; }				// ���[�h�̎擾
	static CMouse			*GetMouse()				{ return m_pMouse; }			// �}�E�X�̎擾
	static CSound			*GetSound()				{ return m_pSound; }			// �T�E���h�̎擾

	static void SetPause(bool bPause)	{ m_bPause = bPause; }						// �|�[�Y�̐ݒ�
	static void SetStop(bool bStop)		{ m_bStop = bStop; }						// �X�g�b�v�̐ݒ�
	static bool GetStop()				{ return m_bStop; }							// �X�g�b�v�̎擾
	static HWND GethWnd()				{ return m_hWnd; }							// �E�B���h�E�n���h���̎擾

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
	static CTitle			*m_pTitle;						// �^�C�g���N���X
	static CGame			*m_pGame;						// �Q�[���N���X
	static CResult			*m_pResult;						// ���U���g�N���X
	static MODE				m_mode;							// ���[�h�̍\����
	static CPause			*m_pPause;						// �|�[�Y�N���X
	static CPlayData		*m_pPlayData;					// �v���C�f�[�^�N���X
	static CLoadX			*m_pLoadX;						// X�ǂݍ��݃N���X
	static CMouse			*m_pMouse;						// �}�E�X�N���X
	static CSound			*m_pSound;						// �T�E���h�N���X
	static bool				m_bOnlyOnce;					// �����[�h�J�ڂ̍ۂ̘A���t�F�[�h�h�~
	static bool				m_bStop;						// �X�g�b�v���邩
	static bool				m_bPause;						// �|�[�Y�ɂ��邩
	static HWND				m_hWnd;							// �E�B���h�E�n���h��
};
#endif

//===============================================
// �^�C�g���̏���
// Author:������
//===============================================
#ifndef _TITLE_H_
#define _TITLE_H_

//===============================================
// �C���N���[�h�t�@�C��
//===============================================
#include "main.h"
#include "scene.h"
#include "Ui.h"

//===============================================
// �}�N����`
//===============================================
#define MAX_SCROLL_UI		(2)		// �X�N���[��UI�̍ő吔
#define MAX_TUTO			(3)		// �`���[�g���A��UI�̍ő吔
#define KEY_EXP_FONTSIZE	(36)	// �L�[�����̃t�H���g�T�C�Y

//===============================================
// �O���錾
//===============================================
class CTexture;
class CStringManager;

//===============================================
// �^�C�g���N���X�̒�`
//===============================================
class CTitle
{
public:
	//------------------------------------
	// �^�C�g����ʂ̃y�[�W
	//------------------------------------
	typedef enum
	{
		PAGE_01 = 0,
		PAGE_02,
		PAGE_03,
		PAGE_MAX

	}TITLE_PAGE;

	//------------------------------------
	// �`���[�g���A���̃y�[�W
	//------------------------------------
	typedef enum
	{
		TUTORIAL_PAGE_01 = 0,
		TUTORIAL_PAGE_02,
		TUTORIAL_PAGE_03,
		TUTORIAL_PAGE_MAX

	}TUTORIAL_PAGE;

	//------------------------------------
	// ��������̃y�[�W
	//------------------------------------
	typedef enum
	{
		OPR_INSTR_01 = 0,
		OPR_INSTR_02,
		OPR_INSTR_03,
		OPR_INSTR_04,
		OPR_INSTR_05,
		OPR_INSTR_06,
		OPR_INSTR_07,
		OPR_INSTR_08,
		OPR_INSTR_09,
		OPR_INSTR_10,
		OPR_INSTR_11,
		OPR_INSTR_12,
		OPR_INSTR_MAX

	}OPR_INSTR;

	//------------------------------------
	// �Q�[�����[�h�I���̎��ʔԍ�
	//------------------------------------
	typedef enum
	{
		TITLE_SELECT_01 = 0,
		TITLE_SELECT_02,
		TITLE_SELECT_03,
		TITLE_SELECT_MAX

	}TITLE_SELECT;

	CTitle();						// �R���X�g���N�^
	~CTitle();						// �f�X�g���N�^
	HRESULT Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��

	void TutorialDisplay(CUi **pTutoUI,CUi **pScr,const CTexture *pTex);									// �`���[�g���A���\�������֐�
	void CursorMove(int &nNowSel, const int &nFirst, const int &nLast, const bool &bMinus);					// �J�[�\���ړ��֐�
	void KeyExpCreate(CStringManager **ppKeyExp, const char *cStr,const int& nFontSize = KEY_EXP_FONTSIZE);	// �L�[���������񐶐��֐�

private:
	//--------------------------------------
	// �Q�[���I��UI�p�����[�^
	//--------------------------------------
	typedef struct
	{
		D3DXVECTOR3		pos, size;	// �ʒu�E�T�C�Y
		string			Tex;		// �e�N�X�`����
		CUi::UI_TYPE	UiType;		// UI�̃^�C�v

	}SELECT_PARAM;
	
	const static SELECT_PARAM	m_SelectParam[TITLE_SELECT_MAX];// �Q�[���I��UI�p�����[�^
	CStringManager				*m_pKeyExp;						// �L�[����
	CUi							*m_pScrollUI[MAX_SCROLL_UI];	// ���[��UI	
	CUi							*m_pPressEnter;					// PressEnterUI�N���X
	CUi							*m_pTitleLogo;					// �^�C�g�����SUI�N���X
	CUi							*m_pTutorial;					// �`���[�g���A�����UI�N���X
	CUi							*m_pSelectUI[TITLE_SELECT_MAX];	// �I����UI�N���X
	TITLE_PAGE					m_Page;							// �^�C�g���y�[�W�\����
	TUTORIAL_PAGE				m_TutoPage;						// �`���[�g���A���y�[�W�\����
	int							m_nSelect;						// ���[�h�I���Ɏg��
	int							m_nSelectKey;					// �L�[�����̑I���Ɏg��
	bool						m_bOPRLock;						// ���샍�b�N����
};
#endif
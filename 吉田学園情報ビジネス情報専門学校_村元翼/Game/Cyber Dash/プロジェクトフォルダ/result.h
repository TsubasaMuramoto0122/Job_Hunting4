//--------------------------------------
// ���U���g�̏���
// Author:������
//--------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"
#include "scene.h"
#include "Ui.h"

//--------------------------------------
// �}�N����`
//--------------------------------------
#define MAX_RESULT					(4)
#define RESULT_RANKING_POSY			(80.0f)				//�����L���O��posY

//--------------------------------------
// �O���錾
//--------------------------------------
class CBg;
class CRanking;

//--------------------------------------
// ���U���g�N���X�̐���
//--------------------------------------
class CResult
{
public:

	//------------------------------------
	// �Q�[�����[�h�I���̎��ʔԍ�
	//------------------------------------
	typedef enum
	{
		RESULT_SELECT_01 = 0,
		RESULT_SELECT_02,
		RESULT_SELECT_03,
		RESULT_SELECT_MAX

	}RESULT_SELECT;

	//--------------------------------------
	// �I�����\����
	//--------------------------------------
	typedef enum
	{
		GAME_RETRY = 0,
		TITLE,
		MAX,
	}TYPE;
										
	CResult();							// �R���X�g���N�^
	~CResult();							// �f�X�g���N�^
	HRESULT Init(void);					// ������
	void Uninit(void);					// �I��
	void Update(void);					// �X�V
	void Draw(void);					// �`��

	// �J�[�\���ړ�
	void CursorMove(int &nNowSel, const int &nFirst, const int &nLast, const bool &bMinus);

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

	const static SELECT_PARAM	m_SelectParam[RESULT_SELECT_MAX];	// �Q�[���I��UI�p�����[�^
	CRanking					*m_pRanking;						// �����L���O�N���X
	CUi							*m_pUi;								// UI
	CUi							*m_pReturnTitle;					// �^�C�g���ɖ߂�UI
	CUi							*m_pSelectUI[RESULT_SELECT_MAX];	// �I����UI�N���X
	CBg							*m_pBgResult;						// ���U���g�w�i
	int							m_nSelectType;						// ���݂̑I��
	int							m_nFrame;							// �t���[����
	int							m_nSelect;							// ���[�h�I���Ɏg��
	bool						m_bOPRLock;							// ���샍�b�N����
	bool						m_bUnlock;							// �A�����b�N����

};
#endif
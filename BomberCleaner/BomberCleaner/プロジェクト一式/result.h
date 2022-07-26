//--------------------------------------
// ���U���g�̏���
//--------------------------------------
#ifndef _RESULT_H_
#define _RESULT_H_
#include "main.h"

//--------------------------------------
// �}�N����`
//--------------------------------------			
#define RESULT_RANKING_POSY			(80.0f)				// �����L���O��posY

//--------------------------------------
// �O���錾
//--------------------------------------
class CBg;
class CUi;
class CRanking;

//--------------------------------------
// ���U���g�N���X�̐���
//--------------------------------------
class CResult
{
public:
	CResult();				// �R���X�g���N�^
	~CResult();				// �f�X�g���N�^
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

private:
	CRanking	*m_pRanking;		// �����L���O�N���X
	CUi			*m_pTitleLogo;		// �^�C�g�����SUI
	CUi			*m_pReturnTitle;	// �^�C�g���ɖ߂�UI
	CBg			*m_pBgResult;		// ���U���g�w�i
	int			m_nSelectType;		// ���݂̑I��
	int			m_nFrame;			// �t���[����

};
#endif
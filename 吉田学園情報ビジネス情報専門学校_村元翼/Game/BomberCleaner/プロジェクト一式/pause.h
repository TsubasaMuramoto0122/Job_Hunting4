//--------------------------------------
// PAUSE�̏���
// Author:������
//--------------------------------------
#ifndef _PAUSE_H
#define _PAUSE_H
#include "main.h"
#include "scene.h"

//-------------------------------------------
// �O���錾
//-------------------------------------------
class CScene2D;
class CBg;

//---------------------------------------------
// �N���X��`
//---------------------------------------------
class CPause : public CScene
{
public:
	//-----------------------------------------
	// �|�[�Y�t�F�[�h�̏�ԍ\����
	//-----------------------------------------
	typedef enum
	{
		FADE_IN = 0,	// �t�F�[�h�C��
		FADE_OUT,		// �t�F�[�h�A�E�g
	}PAUSE_FADE;

	//-----------------------------------------
	// �|�[�Y�̑I����ԗ�
	//-----------------------------------------
	typedef enum
	{
		SELECT_RETURNGAME = 0,		// �Q�[���ɖ߂�
		SELECT_RETRY,				// ���g���C
		SELECT_RETURNTITLE,			// �^�C�g���ɖ߂�
		SELECT_MAX
	}PAUSE_SELECT;

	CPause(OBJTYPE nPriority = OBJTYPE_PAUSE);			// �R���X�g���N�^
	~CPause();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos,D3DXVECTOR3 size);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��
	static CPause *Create(D3DXVECTOR3 pos);				// �C���X�^���X��������

private:
	D3DXVECTOR3		m_pos;							// �ʒu
	D3DXVECTOR3		m_Speed;						// ���x
	float			m_fAlpha;						// �|�[�Y�w�i�̓����x
	int				m_nPauseType;					// �|�[�Y�I��
	bool			m_bUninit;						// pause���I�������邩
	bool			m_bNextMode;					// ���̃��[�h�ɂ�����
	bool			m_bMoveCursor;					// �J�[�\���������Ă��邩
	CScene2D		*m_pSelect[SELECT_MAX];			// �|�[�Y�I���̃|���S��
	CScene2D		*m_pCursor;						// �I���J�[�\���̃|���S��
	CScene2D		*m_pTutorialUI;					// �������UI
	static CBg		*m_pBg;							// �|�[�Y�w�i
};
#endif
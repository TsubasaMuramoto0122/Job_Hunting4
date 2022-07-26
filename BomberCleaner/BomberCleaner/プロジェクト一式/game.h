//============================================================
// �Q�[���V�[���̏���
// Author:������
//============================================================
#ifndef _GAME_H_
#define _GAME_H_

//============================================================
// �C���N���[�h�t�@�C��
//============================================================
#include "main.h"

//============================================================
// �O���錾
//============================================================
class CPlayer;
class CTime;
class CScore;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CStage;
class CUi;
class CTimerCount;

//============================================================
// �N���X�̐���
//============================================================
class CGame
{
public:

	CGame();				// �R���X�g���N�^
	~CGame();				// �f�X�g���N�^
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	CScore *GetScore() { return m_pScore; }									// �X�R�A�̎擾
	void StartGame(void);													// �Q�[���X�^�[�g
	void EndGame(void);														// �Q�[���I��
	void SetUiCreate(const char* TexStr = "",float fSizeMulti = 1);	// UI�̐���
	void SetUiDelete(void);													// UI�̍폜

private:
	CTimerCount			*m_pTimer;			// �^�C�}�[�N���X
	CUi					*m_pUI;				// UI�N���X
	CStage				*m_pStage;			// �X�e�[�W�N���X
	CMeshField			*m_pMeshField;		// ���b�V���t�B�[���h�N���X
	CMeshField			*m_pMeshWall;		// ���b�V���E�H�[���N���X
	CMeshCylinder		*m_pMeshCylinder;	// ���b�V���V�����_�[�N���X
	CMeshSphere			*m_pMeshSphere;		// ���b�V���X�t�B�A�N���X
	CScore				*m_pScore;			// �X�R�A�N���X
	CScore				*m_pAddScore;		// �ǉ��X�R�A�N���X
	bool				m_bOnce;			// ��x�̂ݒʂ鏈���Ɏg���ϐ�
	bool				m_bStart;			// �X�^�[�g����
	bool				m_bEnd;				// �G���h����
	bool				m_bFrameCount;		// �t���[���J�E���g����
	int					m_nFrame;			// �t���[���J�E���g�ϐ�
};
#endif
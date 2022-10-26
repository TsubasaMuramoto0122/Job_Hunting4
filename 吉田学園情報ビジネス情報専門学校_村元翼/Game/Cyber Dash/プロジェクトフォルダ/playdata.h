//===========================================
// �v���C�f�[�^�Ǘ�����
// Author:������
//===========================================
#ifndef _PLAY_DATA_H_
#define _PLAY_DATA_H_
#include "main.h"

//================================================
// �N���X�̒�`
//================================================
class CPlayData
{
public:
	CPlayData();			//�R���X�g���N�^
	~CPlayData();			//�f�X�g���N�^

	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��

	static void SetScore(int nScore);		// �X�R�A�̐ݒ�
	static int GetScore(void);				// �X�R�A�̎擾
	static void SetWinNum(int nWinNum);		// �������̎��ʔԍ��̐ݒ�
	static int GetWinNum(void);				// �������̎��ʔԍ��̎擾

private:
	static int m_nScore;					// �X�R�A
	static int m_nWinNum;					// �������̎��ʔԍ�
};
#endif
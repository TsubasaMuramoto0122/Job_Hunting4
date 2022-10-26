//=============================================================================
//
// �c�@�J�E���g�̃N���X [RemainCount.h]
// Author : ������
//
//=============================================================================
#ifndef _REMAINCOUNT_H_
#define _REMAINCOUNT_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "scene.h"
#include "manager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_REMAIN_DIGIT	(3)									// ���S�񐔂̐�����
#define REMAIN_SIZE			(D3DXVECTOR3(100.0f,50.0f,0.0f))	// �傫��

//=============================================================================
// �O���錾
//=============================================================================
class CNumber;
class CUi;

//=============================================================================
// �N���X��`
//=============================================================================
class CRemainCount : public CScene
{
public:

	CRemainCount(OBJTYPE nPriority = OBJTYPE_NUMBER);	// �R���X�g���N�^
	~CRemainCount();									// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	void RemainChange(void);
	void SumAndDiffRemain(int nValue)	{ m_nRemain += nValue; }		// �c�@�̉��Z���Z
	static void SetRemainChange()		{ m_bChange = true; }			// ���g���C�̐ݒ�
	static void ResetRemain()			{ m_nRemain = 0; }				// ���S�J�E���g�̃��Z�b�g
	static int GetRemain()				{ return m_nRemain; }			// ���S�J�E���g�̎擾

	// �I�u�W�F�N�g����
	static CRemainCount *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
	CNumber				*m_apNumber[MAX_REMAIN_DIGIT];	// �����̃C���X�^���X
	static int			m_nRemain;						// �c�@
	static bool			m_bChange;						// �c�@�ω�����
	int					m_nFrame;						// �t���[��
	D3DXVECTOR3			m_size;							// �傫��
	D3DXVECTOR3			m_pos;							// �ʒu
	D3DXCOLOR			m_col;							// �F
	bool				m_bFade;						// �t�F�[�h����
};

#endif
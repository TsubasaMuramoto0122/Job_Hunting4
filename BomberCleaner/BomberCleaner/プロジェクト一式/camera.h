//======================================
// �J�����̍쐬
// Author:������
//======================================
#ifndef _CAMERA_H
#define _CAMERA_H
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_CAMERA		(1)		// �J�����̍ő吔

//==============================================================================
// �O���錾
//==============================================================================
class CPlayer;

//==============================================================================
// �N���X�̒�`
//==============================================================================
class CCamera
{
public:
	CCamera();						// �R���X�g���N�^
	~CCamera();						// �f�X�g���N�^

	HRESULT Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void SetCamera(void);			// �ݒ�

	// ����
	static CCamera *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float viewportX, float viewportY, float viewportWidth, float viewporHeight, int nID);

	//--------------------------------------------------
	// �ݒ�E�擾�֐�
	//--------------------------------------------------
	void SetRot(D3DXVECTOR3 rot)	{ m_rot = rot; }		// ��]�Z�b�g
	D3DXVECTOR3 GetPosV(void)		{ return m_posV; }		// ���_
	D3DXVECTOR3 GetPosR(void)		{ return m_posR; }		// �����_
	D3DXVECTOR3 GetRot(void)		{ return m_rot; }		// ��]
	float		GetLong(void)		{ return m_fLong; }		// ���_�����_�̋���

	void SetPlayerCamera(CPlayer *pPlayer);					// �J�������v���C���[�ɒǏ]����֐�
	void MoveCamera(void);									// �ړ��֐�


private:
	D3DXVECTOR3 m_posV;				// ���݂̎��_
	D3DXVECTOR3 m_posR;				// ���݂̒����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// �J�����̌���
	float		m_fLong;			// ����
	bool		m_bPosR_Move;		// �����_�������������Ȃ�������
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
};

#endif

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
// �N���X�̒�`
//==============================================================================
class CCamera
{
public:
	//--------------------------------------------------
	// �ړ��ʂ̗�
	//--------------------------------------------------
	typedef enum 
	{
		AMOUNT_00 = 0,
		AMOUNT_01,
		AMOUNT_02,
		AMOUNT_03,
		AMOUNT_04,
		AMOUNT_05,
		AMOUNT_06,

		AMOUNT_MAX

	}MOVE_AMOUNT;

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
	void SetRot(D3DXVECTOR3 rot)	{ m_rot = rot; }								// ��]�ݒ�
	void SetPosR(D3DXVECTOR3 posR)	{ m_posR = posR; }								// �����_�̈ʒu�ݒ�
	D3DXVECTOR3 GetPosV(void)		{ return m_posV; }								// ���_
	D3DXVECTOR3 GetPosR(void)		{ return m_posR; }								// �����_
	D3DXVECTOR3 GetRot(void)		{ return m_rot; }								// ��]
	float		GetLong(void)		{ return m_fLong; }								// ���_�����_�̋���
	float		GetSpeed()			{ return m_fSpeed; }							// �ړ��X�s�[�h
	D3DXVECTOR3	GetSpeed2()			{ return{ m_fSpeedX,m_fSpeedY,m_fSpeedZ }; }	// �ړ���

	void MoveCamera(void);									// �ړ��֐�

private:
	const static float m_fMoveAmt[AMOUNT_MAX];		// �J�����ړ��ʂ̕ύX
	D3DXVECTOR3 m_posV;								// ���݂̎��_
	D3DXVECTOR3 m_posR;								// ���݂̒����_
	D3DXVECTOR3 m_vecU;								// ������x�N�g��
	D3DXVECTOR3 m_rot;								// �J�����̌���
	int			m_nAmtID;							// �ړ��ʂ̔ԍ�
	float		m_fLong;							// ���_�ƒ����_�̊Ԃ̋���
	float		m_fSpeedX;							// �J�����ړ����xX
	float		m_fSpeedY;							// �J�����ړ����xY
	float		m_fSpeedZ;							// �J�����ړ����xZ
	float		m_fSpeed;							// �J�����ړ����x

	bool		m_bPosR_Move;						// �����_�������������Ȃ�������
	bool		m_bFieldMove;						// �J�����ړ�����(���b�V���t�B�[���h���[�h)
	D3DXMATRIX m_mtxProjection;						// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;							// �r���[�}�g���b�N�X
};

#endif

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
#define MAX_CAMERA				(1)								// �J�����̍ő吔
#define CAMERA_DISTANCE			(800.0f)						// �Q�[���̃J�������� 
#define CAMERA_ROT				(D3DXVECTOR3(1.8f,0.0f,0.0f))	// �Q�[���̃J������]

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
	void SetPosVY(float posVY) { m_posV.y = posVY; }
	void SetPosRY(float posRY) { m_posR.y = posRY; }
	void SetRot(D3DXVECTOR3 rot)	{ m_rot = rot; }		// ��]�̐ݒ�
	void SetLong(float fLong)		{ m_fLong = fLong; }	// ���_�����_�̋����̐ݒ�
	D3DXVECTOR3 GetPosV(void)		{ return m_posV; }		// ���_�̎擾
	D3DXVECTOR3 GetPosR(void)		{ return m_posR; }		// �����_�̎擾
	D3DXVECTOR3 GetRot(void)		{ return m_rot; }		// ��]�̎擾
	float		GetLong(void)		{ return m_fLong; }		// ���_�����_�̋����̎擾

	void SetPlayerCamera(CPlayer *pPlayer);					// �J�������v���C���[�ɒǏ]����֐�
	void SetPlayerCamera2(CPlayer *pPlayer);				// �J�������v���C���[�ɒǏ]����֐�
	void NoFollowingPlayer(void);							// �J�������v���C���[�ɒǏ]���Ă��Ȃ����̏���
	void MoveCamera(void);									// �ړ��֐�
	void RotLimit(void);									// ��]���E


private:
	D3DXVECTOR3 m_posV;				// ���݂̎��_
	D3DXVECTOR3 m_posR;				// ���݂̒����_
	D3DXVECTOR3 m_vecU;				// ������x�N�g��
	D3DXVECTOR3 m_rot;				// �J�����̌���
	float		m_fLong;			// ����
	D3DXMATRIX m_mtxProjection;		// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			// �r���[�}�g���b�N�X
};

#endif

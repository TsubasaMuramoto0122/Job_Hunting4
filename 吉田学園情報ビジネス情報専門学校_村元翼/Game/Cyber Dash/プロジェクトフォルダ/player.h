//=============================================================
// �v���C���[�̍쐬
// Author:������
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "scene.h"

//=============================================================
// �O���錾
//=============================================================
class CModel;
class CShadow;
class CBillboard;

//=============================================================
// �N���X��`
//=============================================================
class CPlayer : public CScene
{
public:
	//------------------------------------
	// �v���C���[��ԍ\����
	//------------------------------------
	enum class PLAYER_STATE
	{
		BALL = 0,		// �{�[��
		CUBE,			// �L���[�u
		AIRSHIP,		// ��s�@
		UFO,			// UFO
		MAX
	};

	CPlayer(OBJTYPE nPriority = CScene::OBJTYPE_PLAYER);		// �R���X�g���N�^
	~CPlayer();													// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// ������
	void Uninit(void);											// �I��
	void Update(void);											// �X�V
	void Draw(void);											// �`��

	// �I�u�W�F�N�g����
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, PLAYER_STATE nType, bool bMove = true, bool bCameraMove = true);

	//---------------------------------------------------
	// �ړ��n�֐�
	//---------------------------------------------------
	void Move(void);																				// �ړ�
	void Inertia(D3DXVECTOR3 &speed);																// ����
	void Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool &bJump);		// �d��
	void Action(void);																				// �A�N�V����
	void JumpAction(const float& fJump);															// �W�����v�A�N�V����
	void SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot, const float fMaxSpeed);				// �ړ��E��]���E																			// ���e�����グ�֐�
	void Quaternion(void);																			// �N�H�[�^�j�I����]�֐�
	void ModelDelete(void);																			// ���f���j���֐�
	void PosYLimit(float& PosY);																	// �ʒuY���E�֐�
	void SetReverse(void);																			// �d�͔��]�֐�
	void Fall(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld);									// ��������֐�

	//---------------------------------------------------
	// �ݒ�E�擾�֐�
	//---------------------------------------------------
	D3DXVECTOR3 GetPos(void)					{ return m_pos; }
	D3DXVECTOR3 GetRot(void)					{ return m_rot; }
	D3DXVECTOR3 GetSpeed(void)					{ return m_Speed; }
	bool GetJump()								{ return m_bJump; }
	bool GetLock()								{ return m_bJumpLock; }
	float GetBuoyancy()							{ return m_fBuoyancy; }
	PLAYER_STATE GetState()						{ return m_state; }
	bool GetReverse()							{ return m_bReverse; }
	void SetUninit()							{ m_bUninit = true; }
	void SetSpeed(D3DXVECTOR3 speed)			{ m_Speed = speed; }
	void SetJump(bool bJump)					{ m_bJump = bJump; }
	void SetGravity(float fGravity,bool bJump)	{ m_fGravity = fGravity , m_bJump = bJump; }
	void SetPlayerHit(bool bHit)				{ m_bHit = bHit; }
	void SetLock(bool bLock)					{ m_bJumpLock = bLock; }
	void SetMoveBool(bool bMove)				{ m_bMove = bMove; }
	void SetMoveCameraBool(bool bCameraMove)	{ m_bCameraMove = bCameraMove; }
	void SetState(PLAYER_STATE state);

private:
	const static float	m_fMaxSpeed;					// �ő�X�s�[�h
	const static float	m_fMaxGravity;					// �d�͍ō����x
	static float		m_fFallSpeed;					// �������x
	static float		m_fGravitySpeed;				// �d�͉����x
	static float		m_fJump;						// �W�����v��
	static float		m_fHop;							// �W�����v��(UFO��)
	static float		m_fCubeRotate;					// ��]��(�L���[�u��)
	static float		m_fBallRotate;					// ��]��(�{�[����)
	static float		m_fBuoyancy;					// ����

	CBillboard			*m_pBillboard;					// �r���{�[�h�N���X
	CShadow				*m_pShadow;						// �V���h�E�N���X
	CModel				*m_pModel;						// ���f���N���X
	D3DXVECTOR3			m_pos,m_posOld;					// �ʒu�E�Ō�̈ʒu
	D3DXVECTOR3			m_rot;							// ��]
	D3DXVECTOR3			m_size;							// �T�C�Y
	D3DXVECTOR3			m_Speed;						// ���x			
	float				m_fGravity;						// �d��
	float				m_fAngle;						// ��]����p�x
	int					m_nType;						// �^�C�v
	bool				m_bReverse;						// �d�͔��]������
	bool				m_bJumpLock;					// �W�����v���b�N����
	bool				m_bJump;						// �W�����v����
	bool				m_bSwitch;						// �J�����Ǐ]�؂�ւ�
	bool				m_bUninit;						// �I������
	bool				m_bHit;							// �v���C���[���Ώۂɓ������Ă��邩
	bool				m_bMove;						// �ړ��\��Ԃ̔���
	bool				m_bCameraMove;					// �J�����ړ��\��
	D3DXMATRIX			m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxRot;						// �N�H�[�^�j�I���̌v�Z�Ɏg���}�g���b�N�X
	PLAYER_STATE		m_state;						// �v���C���[�̏��
};
#endif 
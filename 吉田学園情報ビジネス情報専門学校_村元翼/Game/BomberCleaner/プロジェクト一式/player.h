//=============================================================
// �v���C���[�̍쐬
// Author:������
//=============================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================
// �C���N���[�h�t�@�C��
//=============================================================
#include "scene.h"

//=============================================================
// �}�N����`
//=============================================================
#define MAX_SPEED			(6.0f)		// �ő呬�x
#define INERTIA				(0.1f)		// ����
#define ACCELERATION		(0.2f)		// �����x
#define GRAVITY_SPEED		(0.8f)		// �d�͂̋���
#define ROTATING_VELOCITY	(0.001f)	// �v���C���[�̉�]���x
#define JUMP				(13.0f)		// �d��

//=============================================================
// �O���錾
//=============================================================
class CModel;
class CBomb;
class CShadow;
class CBillboard;

//=============================================================
// �N���X��`
//=============================================================
class CPlayer : public CScene
{
public:
	//------------------------------------
	// ���͏��\����
	//------------------------------------
	typedef enum
	{
		DIRECTION_FORWARD = 0,	// �O
		DIRECTION_BACK,			// ��
		DIRECTION_RIGHT,		// �E
		DIRECTION_LEFT,			// ��
		DIRECTION_MAX

	}MOVE_DIRECTION;

	//------------------------------------
	// �v���C���[��ԍ\����
	//------------------------------------
	typedef enum
	{
		STATE_NORMAL = 0,	// �m�[�}��
		STATE_HOLD,			// �����������Ă���
		STATE_MAX

	}PLAYER_STATE;

	CPlayer(OBJTYPE nPriority = CScene::OBJTYPE_PLAYER);		// �R���X�g���N�^
	~CPlayer();													// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);				// ������
	void Uninit(void);											// �I��
	void Update(void);											// �X�V
	void Draw(void);											// �`��

	// ����
	static CPlayer *Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot, int nType);

	// �ړ��n�֐�
	void Move(void);																				// �ړ�
	void Inertia(D3DXVECTOR3 &speed);																// ����
	void Gravity(D3DXVECTOR3& pos, float& fGravity, const float& fGravitySpeed, bool & bJump);		// �d��
	void Action(void);																				// �A�N�V����
	bool InputDirection(const MOVE_DIRECTION &moveDir);												// �ړ��������͊֐�
	void SpeedAndRotLimit(D3DXVECTOR3 &speed, D3DXVECTOR3 &rot, const float fMaxSpeed);				// �ړ��E��]���E
	bool Carry(void);																				// ���e�����グ�֐�
	void Quaternion(void);

	//---------------------------------------------------
	// �ݒ�E�擾�֐�
	//---------------------------------------------------
	D3DXVECTOR3 GetPos(void)					{ return m_pos; }									// �ʒu�̎擾
	D3DXVECTOR3 GetRot(void)					{ return m_rot; }									// ��]�̎擾
	D3DXVECTOR3 GetSpeed(void)					{ return m_Speed; }									// ���x�̎擾
	bool GetJump(void)							{ return m_bJump; }									// �W�����v����̎擾
	void SetSpeed(D3DXVECTOR3 speed)			{ m_Speed = speed; }								// ���x�ݒ�
	void SetJump(bool bJump)					{ m_bJump = bJump; }								// �W�����v����̐ݒ�
	void SetGravity(float fGravity,bool bJump)	{ m_fGravity = fGravity , m_bJump = bJump; }		// �d�͂̐ݒ�

private:
	CBillboard		*m_pBillboard;					// �r���{�[�h�N���X
	CShadow			*m_pShadow;						// �V���h�E�N���X
	CBomb			*m_pBomb;						// ���e�N���X
	CModel			*m_pModel;						// ���f���N���X
	D3DXVECTOR3		m_pos,m_Oldpos;					// �ʒu�E�Ō�̈ʒu
	D3DXVECTOR3		m_posInit;						// �����ʒu
	D3DXVECTOR3		m_rot;							// ��]
	D3DXVECTOR3		m_size;							// �T�C�Y
	D3DXVECTOR3		m_Speed, m_Direction;			// ���x�E����			
	float			m_fGravitySpeed, m_fGravity;	// �d�͉����x�E�d��
	float			m_fMaxSpeed;					// �ő呬�x
	float			m_fAngle;						// ��]����p�x
	int				m_nType;						// �^�C�v
	int				m_nFrame;						// �t���[��
	int				m_nPattern;						// �A�j���[�V�����p�^�[��
	bool			m_bJump;						// �W�����v����
	D3DXMATRIX		m_mtxWorld;						// ���[���h�}�g���b�N�X
	D3DXMATRIX		m_mtxRot;						// �N�H�[�^�j�I���̌v�Z�Ɏg���}�g���b�N�X
	PLAYER_STATE	m_state;						// �v���C���[�̏��
};
#endif 

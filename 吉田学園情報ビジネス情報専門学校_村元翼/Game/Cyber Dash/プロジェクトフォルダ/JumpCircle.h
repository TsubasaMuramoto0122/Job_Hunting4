//===========================================
// �W�����v�T�[�N���̍쐬
// Author;������
//===========================================
#ifndef _JUMPCIRCLE_H_
#define _JUMPCIRCLE_H_
#include "main.h"
#include "billboard.h"

//===========================================
// �N���X��`
//===========================================
class CJumpCircle : public CBillboard
{
public:
	//----------------------------------
	// ��ލ\����
	//----------------------------------
	enum class CIRCLE_TYPE
	{
		JUMP = 0,	// �W�����v
		QUICK_REV,	// �}�ɔ��]
		SLOW_REV,	// ������蔽�]
		FALL,		// �}�ɗ�����
		MAX
	};

	CJumpCircle(OBJTYPE nPriority = OBJTYPE_BILLBOARD);	// �R���X�g���N�^
	~CJumpCircle();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	// �I�u�W�F�N�g����
	static CJumpCircle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CIRCLE_TYPE type);

	// 2D�~�̓����蔻��
	void CircleCollision(void);

	void Jump(CPlayer *pPlayer);		// �W�����v�֐�
	void Reverse(CPlayer *pPlayer);		// �d�͔��]�֐�
	void SlowRev(CPlayer *pPlayer);		// �������d�͔��]�֐�
	void Fall(CPlayer *pPlayer);		// ������֐�

private:
	static float m_fJumpPower;		// �W�����v��
	CIRCLE_TYPE m_type;				// �W�����v�T�[�N���^�C�v
};
#endif
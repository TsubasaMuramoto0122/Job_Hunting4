//===========================================
// ���e�̍쐬
// Author : ������
//===========================================
#ifndef _BOMB_H_
#define _BOMB_H_
#include "main.h"
#include "scene.h"

//===========================================
// �O���錾
//===========================================
class CModel;
class CControl;
class CShadow;

//===========================================
// �N���X��`
//===========================================
class CBomb : public CScene
{
public:

	CBomb(OBJTYPE nPriority = OBJTYPE_BOMB);	// �R���X�g���N�^
	~CBomb();									// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	static CBomb *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);	// �I�u�W�F�N�g����
	void Ignition(void);					// ���Ώ�Ԃ̏���
	void Explosion(void);					// ������Ԃ̏���
	void Throw(D3DXVECTOR3 ThrowDir);		// ���������̏���

	// �ݒ�E�擾����
	void SetThrow(bool bThrow)	{ m_bThrow = bThrow; }	// �����Ă��锻���ݒ�
	bool GetThrow()				{ return m_bThrow; }	// �����Ă��锻����擾
	D3DXVECTOR3 GetThrowDir()	{ return m_ThrowDir; }	// �����Ă���������擾

private:
	CShadow			*m_pShadow;			// �e�N���X
	CControl		*m_pControl;		// �R���g���[���N���X
	CModel			*m_pModel;			// ���f���N���X(���e�̃��f��)
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_scale;			// �X�P�[��
	D3DXVECTOR3		m_ThrowDir;			// ���e�̔�Ԍ���
	D3DXCOLOR		m_Emissive;			// �����F
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
	int				m_nBombTime;		// �����܂ł̃^�C��
	bool			m_bIgnition;		// ���΂�����
	bool			m_bShrink;			// �k�ނ�
	bool			m_bBlink;			// �_�ł��邩
	bool			m_bThrow;			// �������Ă����Ԃ�
	float			m_fOriginScaleX;	// �������̃X�P�[��X(���̃X�P�[��)��ۑ�����ϐ�
	float			m_fSphereSize;		// ���T�C�Y
	float			m_fSphereRot;		// ����]
	float			m_fSphereAlpha;		// �������x
};

#endif
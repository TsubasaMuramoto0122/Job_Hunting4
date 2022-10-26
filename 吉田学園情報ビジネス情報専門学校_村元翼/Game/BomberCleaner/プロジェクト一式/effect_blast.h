//===========================================
// �����G�t�F�N�g(���b�V����)�̍쐬
// Author : ������
//===========================================
#ifndef _EFFECT_BLAST_H_
#define _EFFECT_BLAST_H_
#include "main.h"
#include "scene.h"

//===========================================
// �}�N����`
//===========================================
#define EXPLOSION_SPHERE_SIZE	(D3DXVECTOR3(100.0f,100.0f,100.0f))	// �������T�C�Y
#define BLAST_SCALE_MAX			(5.0f)								// �������X�P�[���̍ő�

//===========================================
// �O���錾
//===========================================
class CMeshSphere;

//===========================================
// �N���X��`
//===========================================
class CBlast : public CScene
{
public:
	CBlast(OBJTYPE nPriority = OBJTYPE_EFFECTMESH);	// �R���X�g���N�^ 
	~CBlast();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	static CBlast *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, int nLine, int nVertical);	// �I�u�W�F�N�g����
	void Spread(void);																					// �����L����

private:
	CMeshSphere		*m_pSphere;			// ���b�V�����N���X(�����G�t�F�N�g�p)
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_size;				// �T�C�Y
	int				m_nFrame;			// �t���[��
	float			m_scale;			// �X�P�[��
	float			m_fScalingSpeed;	// �X�P�[���ύX���x
	D3DXCOLOR		m_col;				// �F
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
};
#endif
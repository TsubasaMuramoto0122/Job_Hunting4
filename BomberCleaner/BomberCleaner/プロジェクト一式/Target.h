//===========================================
// �j�󂷂�Ώۂ̏����̍쐬
// Author : ������
//===========================================
#ifndef _TARGET_H_
#define _TARGET_H_
#include "main.h"
#include "scene.h"

//===========================================
// �}�N����`
//===========================================

//===========================================
// �O���錾
//===========================================
class CModel;

//===========================================
// �N���X��`
//===========================================
class CTarget : public CScene
{
public:

	//---------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//---------------------------------
	CTarget(OBJTYPE nPriority = OBJTYPE_MODEL);
	~CTarget();

	//---------------------------------
	// ���\�b�h
	//---------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	static CTarget *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType);	// �I�u�W�F�N�g����

private:
	CModel			*m_pModel;			// ���f���N���X
	D3DXVECTOR3		m_pos;				// �ʒu
	D3DXVECTOR3		m_rot;				// ��]
	D3DXVECTOR3		m_scale;			// �X�P�[��
	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���b�N�X
};

#endif
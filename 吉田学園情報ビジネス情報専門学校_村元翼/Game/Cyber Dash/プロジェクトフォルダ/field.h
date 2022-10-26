//==================================
// �t�B�[���h�̍쐬
// Author:������
//==================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "scene3D.h"
#include "texture.h"

//==============================================================================
// �}�N����`
//==============================================================================
//#define _OX_EPSILON_		(0.000001f) // �덷
//#define ALLOWABLE_ERROR		(5.0f)	// ���e�덷(�����Ζʂł������Ȃ��悤�ɂ���)

//==============================================================================
// �O���錾
//==============================================================================

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CField : public CScene3D
{
public:
	CField();																			// �R���X�g���N�^
	~CField();																			// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);									// ������
	void Uninit(void);																	// �I��
	void Update(void);																	// �X�V
	void Draw(void);																	// �`��

	// �I�u�W�F�N�g�̐����֐�
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);

	// �����蔻��n
	bool DotCollisionField(CScene *pScene);
	bool LineCollisionField(CScene *pScene);

	D3DXMATRIX *GetVtxMtx(void)	{ return &m_vtxMtxWorld[0]; }	// ���_���[���h�}�g���b�N�X�̎擾
	void SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx);		// ���_���[���h�}�g���b�N�X�̐ݒ�

private:
	D3DXMATRIX m_vtxMtxWorld[FIELD_VTX];						// ���_���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtx[FIELD_VTX];								// ���_
	D3DXVECTOR3 m_vtxRot[FIELD_VTX];							// ��]�p���_
	CScene3D	*m_pScene3D;									// 3D�|���S���N���X
};
#endif
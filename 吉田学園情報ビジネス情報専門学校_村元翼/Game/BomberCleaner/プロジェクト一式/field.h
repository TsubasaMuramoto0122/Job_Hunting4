//==================================
// �t�B�[���h�̍쐬
// Author:������
//==================================
#ifndef _FIELD_H_
#define _FIELD_H_
#include "main.h"
#include "scene3D.h"

//==============================================================================
// �O���錾
//==============================================================================

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CField : public CScene3D
{
public:
	CField();											// �R���X�g���N�^
	~CField();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// ����
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);

	bool DotCollisionField(CScene *pScene);							// �_�Ə��̓����蔻��
	bool LineCollisionField(CScene *pScene);						// ���Ə��̓����蔻��
	D3DXMATRIX *GetVtxMtx(void)	{ return &m_vtxMtxWorld[0]; }		// ���_���[���h�}�g���b�N�X�̎擾
	void SetVtxMtxWorld(D3DXVECTOR3 vtxPos, int nCntVtx);			// ���_���[���h�}�g���b�N�X�̐ݒ�

private:
	D3DXMATRIX m_vtxMtxWorld[FIELD_VTX];	// ���̒��_���[���h�}�g���b�N�X
	D3DXVECTOR3 m_vtx[FIELD_VTX];			// ���̒��_���W���
	D3DXVECTOR3 m_vtxRot[FIELD_VTX];		// ���̒��_��]���
	CScene3D	*m_pScene3D;				// �V�[��3D�N���X
};


#endif // !_FIELD_H_

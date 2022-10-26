//==================================
// �V�[��3D�̍쐬
// Author:
//==================================
#ifndef _SCENE3D_H
#define _SCENE3D_H
#include "main.h"
#include "scene.h"
#include "texture.h"

//==============================================
// �}�N����`
//==============================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define ALLOWABLE_ERROR		(5.0f)		// ���e�덷(�����Ζʂł������Ȃ��悤�ɂ���)
#define PERCENT_EXTRA		(3.0f)		// ���葝���덷
#define FIELD_VTX			(4)
#define MESH_VTX			(4)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	D3DXVECTOR3 nor;//�@���x�N�g��
	D3DCOLOR	col;//���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W

} VERTEX_3D;

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CScene3D : public CScene
{
public:

	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CScene3D(OBJTYPE nPriority = OBJTYPE_POLYGON3D);
	~CScene3D();

	//--------------------------------------------
	// ���\�b�h
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// �ݒ�E�擾�֐�
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }			// �e�N�X�`���ݒ菈��
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);									// ���_���W�̐ݒ�
	void SetTex(int nAnim, int nPartU);													// �e�N�X�`���̐ݒ�
	void SetTex(float fSpeedX, float fSpeedY);											// �e�N�X�`���̐ݒ�
	void SetCol(D3DXCOLOR col);															// �J���[�̐ݒ�
	D3DXMATRIX	GetMatrix()						{ return m_mtxWorld; }					// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetLocalVtx(int nID)			{ return m_vtxLocal[nID]; }				// ���[�J�����_���W�̎擾

	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);		// �V�[��3D�̐���

private:
	D3DXVECTOR3 m_vtxLocal[4];								// ���[�J�����_���W�ۑ�

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;
	D3DXVECTOR3 m_pos;										// �ʒu
	D3DXVECTOR3 m_rot;										// ��]
	D3DXVECTOR3 m_size;										// �T�C�Y
};
#endif
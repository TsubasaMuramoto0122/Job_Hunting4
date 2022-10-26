//==================================
// ���b�V���N���X�̍쐬
// Author: ������
//==================================
#ifndef _MESH_H_
#define _MESH_H_
#include "main.h"

//==============================================
// �O���錾
//==============================================
class CScene;

//==============================================
// �N���X��`
//==============================================
class CMesh
{
public:
	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMesh();
	~CMesh();

	//--------------------------------------------
	// �ݒ�E�擾�֐�
	//--------------------------------------------
	void SetPos(CScene *pScene,D3DXVECTOR3 pos, D3DXVECTOR3 size);					// ���_���W�̐ݒ�
	void SetTex(float fSpeedX, float fSpeedY);										// �e�N�X�`���̐ݒ�
	void SetCol(CScene *pScene, D3DXCOLOR col);										// �J���[�̐ݒ�
	void AddRot(D3DXVECTOR3 rot); 														// ��]�̉��Z

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture; }		// �e�N�X�`���ݒ菈��
	D3DXMATRIX	GetMatrix()							{ return m_mtxWorld; }			// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetLocalVtx(int nID)				{ return m_vtxLocal[nID]; }		// ���[�J�����_���W�̎擾

protected:
	vector<D3DXVECTOR3>			m_vtxLocal;								// ���[�J�����_���W
	vector<D3DXVECTOR3>			m_vtxWorld;								// ���[���h���_���W
	vector<D3DXMATRIX>			m_mtxVec;								// ���[���h���_�}�g���b�N�X
	D3DXVECTOR3					m_posMS;								// �ʒu
	D3DXVECTOR3					m_rotMS;								// ��]
	D3DXVECTOR3					m_sizeMS;								// �T�C�Y
	int							m_nLine;								// ���̃|���S����
	int							m_nVertical;							// �c�̃|���S����

	LPDIRECT3DTEXTURE9		m_pTexture;									// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff;							// ���_�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;									// ���[���h�}�g���b�N�X
};

#endif


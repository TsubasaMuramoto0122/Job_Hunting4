//==================================
// ���b�V���t�B�[���h�̍쐬
// Author: ������
//==================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_
#include "main.h"
#include "scene.h"
#include "Scene3D.h"
#include "texture.h"

//------------------------------------------------------------------------------
//�N���X�̒�`
//------------------------------------------------------------------------------
class CMeshCylinder : public CScene
{
public:
	//--------------------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//--------------------------------------------
	CMeshCylinder(OBJTYPE nPriority = OBJTYPE_MESH);
	~CMeshCylinder();

	//--------------------------------------------
	// ���\�b�h
	//--------------------------------------------
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//--------------------------------------------
	// �ݒ�E�擾�֐�
	//--------------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }		// �e�N�X�`���ݒ菈��
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);								// ���_���W�̐ݒ�
	void SetTex(int nAnim, int nPartU);												// �e�N�X�`���̐ݒ�
	void SetTex(float fSpeedX, float fSpeedY);										// �e�N�X�`���̐ݒ�
	void SetCol(D3DXCOLOR col);														// �J���[�̐ݒ�
	D3DXMATRIX	GetMatrix()				{ return m_mtxWorld; }						// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetLocalVtx(int nID)	{ return m_vtxLocal[nID]; }					// ���[�J�����_���W�̎擾

	static CMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, int nLine, int nVertical);	// ����
	bool LineCollisionMesh(CScene *pScene, const int *nVtx);													// ���ƃ��b�V���̓����蔻��

private:
	vector<D3DXVECTOR3> m_vtxLocal;								// ���[�J�����_���W
	vector<D3DXVECTOR3> m_vtxWorld;								// ���[���h���_���W
	vector<D3DXMATRIX>  m_mtxVec;								// ���[���h���_�}�g���b�N�X
	D3DXVECTOR3 m_pos;											// �ʒu
	D3DXVECTOR3 m_rot;											// ��]
	D3DXVECTOR3 m_size;											// �T�C�Y
	int m_nLine;												// ���̃|���S����
	int m_nVertical;											// �c�̃|���S����

protected:

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9	m_pVtxIndexBuff = nullptr;		// ���_�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX				m_mtxWorld;
};


#endif


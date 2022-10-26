//=============================================================================
//
// �r���{�[�h�N���X [billboard.h]
// Author : ������
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#include "main.h"
#include "scene.h"
#include "manager.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	// ���_�t�H�[�}�b�g
#define NUM_VERTEX		(4)											// ���_��
#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)							// �X�N���[��X���W�̒��S
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)							// �X�N���[��Y���W�̒��S

//=============================================================================
// �N���X�̐���
//=============================================================================
class CBillboard : public CScene
{
public:

	CBillboard(OBJTYPE nPriority = OBJTYPE_BILLBOARD);	// �R���X�g���N�^
	~CBillboard();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	// ����
	static CBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col);

	//---------------------------------------
	// �ݒ�E�擾�֐�
	//---------------------------------------
	void SetTex(int nAnim, int nPartU);
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void SetColVal(D3DCOLORVALUE colval);
	D3DXVECTOR3 GetPos(void)						{ return m_pos; }
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture)	{ m_pTexture = pTexture; }

protected:
	LPDIRECT3DTEXTURE9		m_pTexture;				// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;				// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;					// �ʒu
	D3DXVECTOR3				m_size;					// �T�C�Y
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	D3DCOLORVALUE			m_col;					// �J���[
};
#endif
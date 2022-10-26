//*****************************************************************************
// �|���S���̏���
// Author:������
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float		rhw;	// 1.0f�ŌŒ�
	D3DCOLOR	col;	// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

//*****************************************************************************
// �N���X�̒�`
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(OBJTYPE nPriority = OBJTYPE_POLYGON2D);		// �R���X�g���N�^
	~CScene2D();											// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);		// ������
	void Uninit(void);										// �I��
	void Update(void);										// �X�V
	void Draw(void);										// �`��
	
	// �I�u�W�F�N�g����
	static CScene2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size,OBJTYPE objtype = OBJTYPE_POLYGON2D);

	//------------------------------------------------------------------------------
	// �ݒ�E�擾�֐�
	//------------------------------------------------------------------------------
	// �ʒu�ݒ�
	void SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale);
	void SetPos(D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3);	
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool bScroll = true);
	void SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 Rotvtx0, D3DXVECTOR3 Rotvtx1, D3DXVECTOR3 Rotvtx2, D3DXVECTOR3 Rotvtx3);
	// �e�N�X�`���ݒ�
	void SetTex(D3DXVECTOR2 Start, D3DXVECTOR2 End);
	void SetTex(int nAnim,int nPartU);
	void SetTex(float fSpeedX, float fSpeedY);
	void SetTex(float nPatternX, float nPatternY, float fSizeX,float fSizeY);
	// �J���[�ݒ�
	void SetCol(D3DXCOLOR col);
	// �e�N�X�`���擾
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }
	// ��]�ݒ�
	void SetRot(D3DXVECTOR3 pos, D3DXVECTOR3 size,float angle);	

private:
	D3DXVECTOR2 m_Scroll;						// �X�N���[���p�ϐ�
	D3DXVECTOR2 m_SaveTex[4];					// �e�N�X�`�����W�ۑ��p��

protected:
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@�ւ̃|�C���^
};
#endif

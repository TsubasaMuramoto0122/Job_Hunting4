//========================================
// �w�i
// Author:������
//========================================
#ifndef _BG_H
#define _BG_H
#include "scene2D.h"

//========================================
// �O���錾
//========================================
class CScene;

//========================================
// �N���X����
//========================================
class CBg : public CScene2D
{
public:
	CBg(OBJTYPE nPriority = OBJTYPE_BG);
	~CBg();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// ��������
	static CBg *Create
	(	
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		bool bScrol = true,
		CScene::OBJTYPE objtype = CScene::OBJTYPE_BG
	); 


	//-------------------------------------
	// �ݒ�E�擾�֐�
	//-------------------------------------
	void BindTexture(LPDIRECT3DTEXTURE9 pTex)	{ m_pTexture = pTex; }	// �e�N�X�`���擾
	D3DXVECTOR3 GetTex(void)					{ return m_Tex; }		// �e�N�X�`���ʒu�擾
	D3DXVECTOR3 GetPos(void)					{ return m_pos; }		// �ʒu�擾

private:
	LPDIRECT3DTEXTURE9			m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_Tex;			// �e�N�X�`�����X�N���[������ϐ�
	D3DXVECTOR3					m_pos;			// �ʒu
	bool						m_bScroll;		// �X�N���[���؂�ւ�
};
#endif 
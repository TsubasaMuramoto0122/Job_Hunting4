//=============================================================================
//
// �e�L�X�g�\���̃N���X [TextureFont.h]
// Author : ������
//
//=============================================================================
#ifndef _TEXTUREFONT_H_
#define _TEXTUREFONT_H_

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FONT_POS (D3DXVECTOR3(50.0f,600.0f,0.0f))

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFont : public CScene2D
{
public:
	CFont(const OBJTYPE nPriority = OBJTYPE_FONT);				// �R���X�g���N�^
	~CFont();													// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	static CFont *Create(D3DXVECTOR3 pos, int nfontSize,const char* str, const char* font = "�l�r ����");	// �����֐�

private:
	int m_nFontSize;
	const char* m_str;
	const char* m_font;
	D3DXVECTOR3 m_pos;
	
};
#endif

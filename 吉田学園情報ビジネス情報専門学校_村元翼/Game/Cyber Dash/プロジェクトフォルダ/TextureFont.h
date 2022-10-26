//=============================================================================
//
// �e�L�X�g�\���̃N���X [TextureFont.h]
// Author : ������
//
//=============================================================================
#ifndef _TEXTUREFONT_H_
#define _TEXTUREFONT_H_
#include "scene.h"
#include "scene2D.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define FONT_POS (D3DXVECTOR3(50.0f,600.0f,0.0f))		// �t�H���g�̈ʒu

//=============================================================================
// �N���X��`
//=============================================================================
class CFont : public CScene2D
{
public:
	CFont(OBJTYPE nPriority = OBJTYPE_NUMBER);					// �R���X�g���N�^
	~CFont();													// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��

	// �I�u�W�F�N�g����
	static CFont *Create(D3DXVECTOR3 pos, int nfontSize,const char* str, const char* font = "�l�r ����");

private:
	int m_nFontSize;		// �t�H���g�T�C�Y
	const char* m_str;		// ����
	const char* m_font;     // �t�H���g��
	D3DXVECTOR3 m_pos;		// �ʒu
};
#endif

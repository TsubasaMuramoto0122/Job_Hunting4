//=============================================================================
//�e�N�X�`������
//Author:������
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//=============================================================================
// �w�b�_�t�@�C���̃C���N���[�h
//=============================================================================
#include "main.h"

//=============================================================================
// �N���X��`
//=============================================================================
class CTexture
{
public:
	CTexture();						//�R���X�g���N�^
	~CTexture();					//�f�X�g���N�^

	static void Init(void);															// �e�N�X�`���̐���
	static void Uninit(void);														// �I��
	static LPDIRECT3DTEXTURE9 GetTexture(std::string texType);						// �e�N�X�`���̊��蓖��
	static LPDIRECT3DTEXTURE9 GetTexture(int nCnt)	{ return m_apTexture[nCnt]; }	// �e�N�X�`���̊��蓖��
	static std::string GetPas(int nCntTex)			{ return m_aPas[nCntTex]; }		// �e�N�X�`���̃p�X�擾����
	static int GetNum(void)							{ return m_nNumTex; }			// �e�N�X�`���̑����擾����

private:
	static std::vector<LPDIRECT3DTEXTURE9> m_apTexture;		// �e�N�X�`��
	static std::vector<std::string>	m_aPas;					// �e�N�X�`���̃p�X
	static map<string, int> m_texType;						// �e�N�X�`���̎��
	static int m_nNumTex;									// �e�N�X�`���̑���
};

#endif
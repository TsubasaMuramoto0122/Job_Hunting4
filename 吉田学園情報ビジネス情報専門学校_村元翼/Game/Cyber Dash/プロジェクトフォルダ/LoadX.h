//======================================================
// X�t�@�C���̓ǂݍ���
// Author:������
//======================================================
#ifndef _LOADX_H_
#define _LOADX_H_
#include "main.h"

//======================================================
// �N���X����
//======================================================
class CLoadX
{
public:
	typedef struct
	{
		LPD3DXMESH					m_pMesh;		// ���b�V���i���_���j�ւ̃|�C���^
		LPD3DXBUFFER				m_pBuffMat;		// �}�e���A���i�ގ����j�ւ̃|�C���^
		DWORD						m_nNumMat;		// �}�e���A���̐�
		D3DXMATERIAL				*m_pMat;		// �}�e���A���ւ̃|�C���^
		vector<LPDIRECT3DTEXTURE9>	m_pTex;			// �e�N�X�`��
		string						m_XFilePas;		// �t�@�C���p�X

	}XData;

	//-------------------------------
	// �R���X�g���N�^�E�f�X�g���N�^
	//-------------------------------
	CLoadX();
	~CLoadX();

	//-------------------------------
	// ���\�b�h
	//-------------------------------
	HRESULT Init(void);
	void Uninit(void);

	//-------------------------------
	// �擾�֐�
	//-------------------------------
	static XData GetXData(string xName);						// X�t�@�C���𕶎��񌟍��Ŏ擾
	static XData GetXData(int nNum)	{ return m_vDataX[nNum]; }	// X�t�@�C���f�[�^�擾
	static int GetNumAll()			{ return m_nNum; }			// X�t�@�C�������擾
	static int GetNum(string xName)	{ return m_NameX[xName]; }	// X�t�@�C���w��ԍ��擾

private:
	static vector<XData>		m_vDataX;		// X�t�@�C���f�[�^�\����
	static map<string, int>		m_NameX;		// X�t�@�C���̎��
	static int m_nNum;							// X�t�@�C���̑���
};
#endif _LOADX_H_

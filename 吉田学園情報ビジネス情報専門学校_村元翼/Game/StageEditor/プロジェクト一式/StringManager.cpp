//=============================================================================
//
// ������Ǘ��̃N���X [StringManager.cpp]
// Author : ������
//
//=============================================================================
#include "StringManager.h"
#include "TextureFont.h"
#include <stdlib.h>
#include <mbstring.h>

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CStringManager::CStringManager()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStringManager::~CStringManager()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CStringManager::Init(D3DXVECTOR3 pos, float fSpace, int nfontSize, const char* str, const char* font)
{
	if (m_vFont.empty())
	{
		CFont *pFont = nullptr;

		// 2�o�C�g������nCnt��2�i�߂�̂ŁA���̕����܂ł�fSpace��2���ƂȂ�̂ŁA�󔒂�1�������ł��Ă��܂��̂𖄂߂�ϐ������
		int nFill = 0;
		int nLen = strlen(str);
		for (int nCnt = 0; nCnt < nLen; nCnt++)
		{
			// �t�H���g����
			pFont = CFont::Create({ pos.x + (fSpace*(nCnt - nFill)),pos.y,pos.z }, nfontSize, &str[nCnt], font);
			m_vFont.emplace_back(pFont);
	
			// 2�o�C�g����(�S�p����)��������s��
			if (_mbclen((BYTE*)&str[nCnt]) == 2)
			{
				nCnt++;		// �z���2�i�߂�
				nFill++;	// �󂢂����Ԃ𖄂߂邽�߂̕ϐ�
			}
		}
	}

	else
	{
		return S_FALSE;
	}

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CStringManager::Uninit()
{
	// �󔻒�
	if (!m_vFont.empty())
	{
		// �R���e�i�Ɋi�[����Ă���t�H���g�N���X��S�Ĕj��
		for (CFont *pDeleteFont : m_vFont)
		{
			if (pDeleteFont)
			{
				pDeleteFont->Uninit();
				pDeleteFont = nullptr;
			}
		}

		// �t�H���g�N���X�R���e�i��j��
		m_vFont.clear();
	}
}

//=============================================================================
// �X�V
//=============================================================================
void CStringManager::Update()
{

}

//=============================================================================
// ����
//=============================================================================
CStringManager *CStringManager::Create(D3DXVECTOR3 pos, float fSpace, int nfontSize, const char* str, const char* font)
{
	// �C���X�^���X����
	CStringManager *pStrMng = nullptr;
	if (!pStrMng)
	{
		pStrMng = new CStringManager;
		if (pStrMng)
		{
			// ������
			pStrMng->Init(pos, fSpace, nfontSize, str, font);
		}
	}

	return pStrMng;
}
//=============================================================================
//
// ������Ǘ��̃N���X [StringManager.h]
// Author : ������
//
//=============================================================================
#ifndef _STRINGMANAGER_H_
#define _STRINGMANAGER_H_

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"


//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFont;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CStringManager
{
public:
	CStringManager();		// �R���X�g���N�^
	~CStringManager();		// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSpace, int nfontSize, const char* str, const char* font);		// ������
	void Uninit(void);																					// �I��
	void Update(void);																					// �X�V

	static CStringManager *Create(D3DXVECTOR3 pos,float fSpace, int nfontSize, const char* str, const char* font = "�l�r ����");	// �����֐�

private:
	vector<CFont*> m_vFont;		// �t�H���g�N���X�̓��I�z��
};
#endif


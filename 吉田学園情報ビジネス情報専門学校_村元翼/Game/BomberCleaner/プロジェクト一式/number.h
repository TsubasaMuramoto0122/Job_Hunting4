//=============================================================================
//
// �����\���̃N���X [number.h]
// Author : ������
//
//=============================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "manager.h"
#include "scene.h"
#include "scene2D.h"

//*****************************************************************************
//�N���X��`
//*****************************************************************************
class CNumber : public CScene2D
{
public:
	CNumber(OBJTYPE nPriority = OBJTYPE_NUMBER);				// �R���X�g���N�^
	~CNumber();													// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);			// ����������
	void Uninit(void);											// �I������
	void Update(void);											// �X�V����
	void Draw(void);											// �`�揈��
	void SetColor(D3DXCOLOR col);								// �F�̐ݒ�
	void SetNumber(int nNumber);								// �����ݒ�
	static CNumber *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// �����֐�

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;			// �o�b�t�@
	D3DXCOLOR					m_col;				// �F
};

#endif

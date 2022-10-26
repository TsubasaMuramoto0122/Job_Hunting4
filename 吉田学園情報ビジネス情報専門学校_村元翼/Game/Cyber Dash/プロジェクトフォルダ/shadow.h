//==================================
// �e�̍쐬
// Author:������
//==================================
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"
#include "scene3D.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define GRAVITY (1000.0f)

//------------------------------------------------------------------------------
// �N���X�̒�`
//------------------------------------------------------------------------------
class CShadow : public CScene3D
{
public:
	CShadow(OBJTYPE nPriority = OBJTYPE_SHADOW);									// �R���X�g���N�^
	~CShadow();																		// �f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);								// ������
	void Uninit(void);																// �I��
	void Update(void);																// �X�V
	void Draw(void);																// �`��
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot);		// ����

	void SetGround(const float &X, const float &Z)	{ m_pos.x = X, m_pos.z = Z; }	// �ʒu�ݒ�
	void SetHeight(const float &fHeight)			{ m_pos.y = fHeight; }			// �����ݒ�
	void SetGravity(float GravitySpeed);											// �d�͐ݒ�

private:
	D3DXVECTOR3 m_OriginSize;														// �������̃T�C�Y
};


#endif	
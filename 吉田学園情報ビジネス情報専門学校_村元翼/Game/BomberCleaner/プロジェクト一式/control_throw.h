//=============================================================================
// �R���g���[������ [control_throw.h]
//=============================================================================
#ifndef _CONTROL_THROW_H_
#define _CONTROL_THROW_H_
#include "main.h"
#include "control.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene;

//*****************************************************************************
// �R���g���[���N���X
//*****************************************************************************
class CControlThrow : public CControl
{
public:
	CControlThrow();						// �R���X�g���N�^
	~CControlThrow();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(CScene *pObject);			// �X�V����
	static CControlThrow *Create();			// ��������

private:
	const static float m_fGravity;			// �d��
	bool m_bDoOnce;							// ��x�̂ݒʂ鏈��
};

#endif
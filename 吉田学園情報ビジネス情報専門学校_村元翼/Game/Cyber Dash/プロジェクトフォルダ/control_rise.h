//=============================================================================
// �R���g���[������ [control_rise.h]
//=============================================================================
#ifndef _CONTROL_RISE_H_
#define _CONTROL_RISE_H_
#include "main.h"
#include "control.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScene;

//*****************************************************************************
// �R���g���[���N���X
//*****************************************************************************
class CControlRise : public CControl
{
public:
	CControlRise();										// �R���X�g���N�^
	~CControlRise();									// �f�X�g���N�^
	HRESULT Init(void);									// ����������
	void Uninit(void);									// �I������
	void Update(CScene *pObject);						// �X�V����
	static CControlRise *Create(bool bDown = false);	// ��������

private:
	bool m_bDown;							// ���~����^�C�v�ɂ��邩
};

#endif

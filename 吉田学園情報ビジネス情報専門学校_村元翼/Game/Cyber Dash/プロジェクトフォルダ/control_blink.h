//=============================================================================
// �R���g���[������ [control_blink.h]
//=============================================================================
#ifndef _CONTROL_BLINK_H_
#define _CONTROL_BLINK_H_
#include "main.h"
#include "control.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CScene;

//*****************************************************************************
// �R���g���[���N���X
//*****************************************************************************
class CControlBlink : public CControl
{
public:
	CControlBlink();						// �R���X�g���N�^
	~CControlBlink();						// �f�X�g���N�^
	HRESULT Init(void);						// ����������
	void Uninit(void);						// �I������
	void Update(CScene *pObject);			// �X�V����
	static CControlBlink *Create(void);		// ��������
	
	// �_�ő��x�ύX
	void SetInterval(int nInterval) { m_nInterval = nInterval; }

private:
	int m_nInterval;				// �_�ŊԊu
};

#endif

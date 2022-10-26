//=============================================================================
// �R���g���[������ [control_fade.cpp]
// Author : ������
//=============================================================================
#include "control_fade.h"
#include "billboard.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlFade::CControlFade()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlFade::~CControlFade()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlFade::Init()
{
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlFade::Uninit()
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlFade::Update(CScene *pObject)
{
	// �I�u�W�F�N�g����
	CBillboard *pBillboard = nullptr;

	// �I�u�W�F�N�g��Billboard�N���X�ɃL���X�g
	pBillboard = (CBillboard*)pObject;

	if (pBillboard)
	{
		// ���X�ɏ����鏈��
		D3DXCOLOR alpha = pBillboard->GetCol();
		D3DXVECTOR3 pos = pBillboard->GetPos();
		D3DXVECTOR3 size = pBillboard->GetSize();

		// �����x�̃t�F�[�h�A�E�g
		if (alpha.a > 0.0f)
		{
			alpha.a -= 0.01f;
			pBillboard->SetColVal(alpha);
		}

		// �T�C�Y�̏k��
		if (size.x > 0.0f && size.y > 0.0f)
		{
			size.x -= 0.1f;
			size.y -= 0.1f;
			pBillboard->SetPos(pos, size);
		}

		// �r���{�[�h�̏I��
		if (alpha.a <= 0.0f || (size.x <= 0.0f && size.y <= 0.0f))
		{
			pBillboard->Uninit();
		}
	}

}

//=============================================================================
// �I�u�W�F�N�g��������
//=============================================================================
CControlFade *CControlFade::Create()
{
	// �I�u�W�F�N�g����
	CControlFade *pControlFade = nullptr;
	if (pControlFade = new CControlFade())
	{
		pControlFade->Init();	// ������
	}

	return pControlFade;
}
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
	// �r���{�[�h�N���X
	CBillboard *pBillboard = nullptr;
	if (!pBillboard)
	{
		// �I�u�W�F�N�g��Billboard�N���X�ɃL���X�g
		pBillboard = (CBillboard*)pObject;

		if(pBillboard)
		{
			// �ϐ��擾
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
			if(size.x > 0.0f && size.y > 0.0f)
			{ 
				size.x -= 0.1f;
				size.y -= 0.1f;
				pBillboard->SetPos(pos,size);
			}

			// �r���{�[�h�̏I��
			if(alpha.a <= 0.0f || (size.x <= 0.0f && size.y <= 0.0f))
			{
				pBillboard->Uninit();
			}
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CControlFade *CControlFade::Create()
{
	// �C���X�^���X�̐���
	CControlFade *pControlFade = nullptr;
	if (!pControlFade)
	{
		pControlFade = new CControlFade();
		if (pControlFade)
		{
			pControlFade->Init();	// ������
		}
	}
	return pControlFade;
}
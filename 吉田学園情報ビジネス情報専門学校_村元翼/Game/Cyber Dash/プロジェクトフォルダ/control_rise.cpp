//=============================================================================
// �R���g���[������ [control_rise.cpp]
// Author : ������
//=============================================================================
#include "control_rise.h"
#include "effect.h"
#include <random>

//=============================================================================
// �}�N����`
//=============================================================================
#define MOVE_SPEED (1.5f)

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlRise::CControlRise()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlRise::~CControlRise()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlRise::Init()
{
	// ���˃G�t�F�N�g�̊p�x�̍ŏ��l�ƍő�l(0�`360)
	const float minRange = 0.0f;
	const float maxRange = D3DX_PI * 2;
	float fAngle;

	std::random_device Randseed;												// ����������ŃV�[�h�l�����S�����_���ɏ���������
	std::mt19937_64 mt(Randseed());												// �^�����������o��
	std::uniform_real_distribution<float> Dist(minRange, maxRange);				// �w�肵���͈͓��̐��l�𓙊m���ŕԂ�(�ŏ��l�ȏ�A�ő�l"����")
	fAngle = Dist(mt);

#ifdef _DEBUG
	float fDegree = D3DXToDegree(fAngle);										// �f�o�b�O���Ɋp�x���m�F���邽�߂̏���
#endif

	// �L������������߂�
	m_move.x = cosf(fAngle);
	m_move.z = sinf(fAngle);
	m_move.y = 1.0f;
	m_move *= MOVE_SPEED;
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlRise::Uninit()
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlRise::Update(CScene *pObject)
{
	// �I�u�W�F�N�g����
	CEffect *pEffect = nullptr;
	// �I�u�W�F�N�g���G�t�F�N�g�N���X�ɃL���X�g
	pEffect = (CEffect*)pObject;
	if (pEffect)
	{
		// ��ɂӂ�ӂ�㏸���鏈��
		D3DXVECTOR3 pos = pEffect->GetPos();

		if (m_bDown)
		{
			pos -= m_move;		// ���~���Ă���
		}
		else
		{
			pos += m_move;		// �㏸���Ă���

		}

		pEffect->SetPos(pos);
	}
}

//=============================================================================
// �I�u�W�F�N�g��������
//=============================================================================
CControlRise *CControlRise::Create(bool bDown)
{
	// �I�u�W�F�N�g����
	CControlRise *pControlRise = nullptr;
	if (pControlRise = new CControlRise())
	{
		pControlRise->m_bDown = bDown;
		pControlRise->Init();	// ������
	}

	return pControlRise;
}


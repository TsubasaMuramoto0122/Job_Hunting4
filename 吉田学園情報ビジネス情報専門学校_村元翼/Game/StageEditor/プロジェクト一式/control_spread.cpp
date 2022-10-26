//=============================================================================
// �R���g���[������ [control_spread.cpp]
// Author : ������
//=============================================================================
#include "control_spread.h"
#include "effect.h"
#include <random>

//=============================================================================
// �}�N����`
//=============================================================================
#define MOVE_SPEED (5.0f)	// �ړ��X�s�[�h

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CControlSpread::CControlSpread()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CControlSpread::~CControlSpread()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CControlSpread::Init()
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

	m_move.x = cosf(fAngle); //* sinf(fAngle);
	m_move.z = sinf(fAngle); //* sinf(fAngle);

	m_move *= MOVE_SPEED;	// �ړ��X�s�[�h���|����
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CControlSpread::Uninit()
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CControlSpread::Update(CScene *pObject)
{
	// �C���X�^���X
	CEffect *pEffect = nullptr;
	if (!pEffect)
	{
		// �I�u�W�F�N�g���G�t�F�N�g�N���X�ɃL���X�g
		pEffect = (CEffect*)pObject;
		if (pEffect)
		{
			// ��ɂӂ�ӂ�㏸���鏈��
			D3DXVECTOR3 pos = pEffect->GetPos();
			pos += m_move;		// �㏸���Ă���
			pEffect->SetPos(pos);
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CControlSpread *CControlSpread::Create()
{
	// �C���X�^���X�̐���
	CControlSpread *pControlSpread = nullptr;
	if (!pControlSpread)
	{
		pControlSpread = new CControlSpread();
		if (pControlSpread)
		{
			pControlSpread->Init();	// ������
		}
	}
	return pControlSpread;
}


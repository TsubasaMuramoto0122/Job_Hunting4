//========================================
// �~�X�g�̐���
// Author:������
//========================================
#include "Mist.h"
#include "FadePolygon.h"

//========================================
// �}�N����`
//========================================
#define MAX_INIT_FADE_SIZE (3.0f)	// �ő叉���t�F�[�h�|���S���T�C�Y

//========================================
// �R���X�g���N�^
//========================================
CMist::CMist(int nMaxFrame,int nMaxCount, OBJTYPE nPriority) : CScene(nPriority),
															   m_nMaxFrame(nMaxFrame),	// �ő�t���[��������
															   m_nMaxCount(nMaxCount)	// �ő�J�E���g������
{
	m_RangeMin = { 0.0f,0.0f,0.0f };		// �͈͍ŏ�
	m_RangeMax = { 0.0f,0.0f,0.0f };		// �͈͍ő�
	m_nFrame = m_nMaxFrame;					// �t���[��
	m_nCount = 0;							// �J�E���g
}

//========================================
// �f�X�g���N�^
//========================================
CMist::~CMist() 
{

}

//========================================
// �I�u�W�F�N�g����
//========================================
CMist *CMist::Create(D3DXVECTOR3 RangeMin, D3DXVECTOR3 RangeMax, int nMaxFrame, int nMaxCount,bool bInfinite)
{
	// �I�u�W�F�N�g����
	CMist *pMist = nullptr;
	if (pMist = new CMist(nMaxFrame, nMaxCount))	// �ő�l���R���X�g���N�^�ŏ�����
	{
		pMist->m_bInfinite = bInfinite;		// ������������
		pMist->Init(RangeMin, RangeMax);	// ������
	}

	return pMist;
}

//========================================
// ������
//========================================
HRESULT CMist::Init(D3DXVECTOR3 RangeMin, D3DXVECTOR3 RangeMax)
{
	m_RangeMin = RangeMin;	// �ŏ��͈͂̎擾
	m_RangeMax = RangeMax;	// �ő�͈͂̎擾
	return S_OK;
}

//========================================
// �I��
//========================================
void CMist::Uninit(void)
{
	// ���̃I�u�W�F�N�g��j��
	Release();
}

//========================================
// �X�V
//========================================
void CMist::Update(void)
{
	// �ő�J�E���g���܂Ő���������I��
	if (m_nCount > m_nMaxCount)
	{
		Uninit();
	}

	// �t���[���J�E���g
	m_nFrame++;

	// �t�F�[�h����|���S�����w��͈͓��ɐ���
	if (m_nFrame > m_nMaxFrame)
	{
		mt19937_64	  mt(m_Randseed());											// �^�����������o��
		uniform_real_distribution<float> DistX(m_RangeMin.x, m_RangeMax.x);		// �w�肵���͈͓��̐��l�𓙊m���ŕԂ�(�ŏ��l�ȏ�A�ő�l"����")
		uniform_real_distribution<float> DistY(m_RangeMin.y, m_RangeMax.y);		
		uniform_real_distribution<float> DistInitSize(1.0f, MAX_INIT_FADE_SIZE);

		CFadePolygon::Create({ DistX(mt),DistY(mt),0.0f }, { 1.0f,1.0f,0.0f }, DistInitSize(mt));
		m_nFrame = 0;	// �t���[�����Z�b�g

		// ��������off
		if (!m_bInfinite)
		{
			m_nCount++;	// ���������J�E���g
		}
	}
}

//========================================
// �`��
//========================================
void CMist::Draw(void)
{

}

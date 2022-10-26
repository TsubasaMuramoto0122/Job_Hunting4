//=============================================================================
// ���C�g�̍쐬
// Author:������
//=============================================================================
#include "light.h"
#include "Renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��̏�����
//=============================================================================
int CLight::m_nCntLight = 0;		// ���C�g

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	// �������̃J�E���g
	m_nId = m_nCntLight;
	// �J�E���g����
	m_nCntLight++;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{
	// ���C�g�̃J�E���g�����炷
	m_nCntLight--;
}

//=============================================================================
// �|���S���̏���������
//=============================================================================
HRESULT CLight::Init(D3DXVECTOR3 pos,D3DXCOLOR Diffuse, D3DXVECTOR3 vecDir)
{
	// ���C�g���N���A����
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));

	LPDIRECT3DDEVICE9 pDevice;											// �f�o�C�X�̃|�C���^
	D3DXVECTOR3 r_vecDir;												// �ݒ�p�̃x�N�g��
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		// �f�o�C�X�̎擾

	// ���C�g�̎�ނ�ݒ�
	m_light.Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U��
	m_light.Diffuse = Diffuse;

	// ���C�g�̕�����ݒ�
	r_vecDir = vecDir;

	// ���K������
	D3DXVec3Normalize(&r_vecDir, &r_vecDir);

	// ���C�g�̈ʒu�ݒ�
	m_light.Position = pos;

	m_light.Direction = r_vecDir;
	// ���C�g�̐ݒ�
	pDevice->SetLight(m_nId, &m_light);
	// ���C�g��L���ɂ���
	pDevice->LightEnable(m_nId, TRUE);

	return S_OK;
}


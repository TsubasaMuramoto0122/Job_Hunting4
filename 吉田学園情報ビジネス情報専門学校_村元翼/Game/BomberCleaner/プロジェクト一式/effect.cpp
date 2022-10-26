//=============================================================================
// �G�t�F�N�g [effect.cpp]
// Author : ������
//=============================================================================
#include "Scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "effect.h"
#include "control_fade.h"
#include "control_rise.h"
#include "control_spread.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect(OBJTYPE nPriority) : CScene(nPriority)
{
	// �����o�ϐ��̃N���A
	m_pos	= { 0.0f,0.0f,0.0f };				// �ʒu
	m_size	= { 0.0f,0.0f,0.0f };				// �傫��
	m_col	= { 0.0f,0.0f,0.0f,0.0f };			// �F
	m_fSub	= 0.0f;								// ������
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{
}

//=============================================================================
// ��������
//=============================================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, float fSub, int nType)
{
	// �C���X�^���X����
	CEffect *pEffect = nullptr;
	if (!pEffect)
	{
		pEffect = new CEffect(OBJTYPE_EFFECT);
		if (pEffect)
		{
			pEffect->CScene::SetCol(col);	// �J���[�ݒ�
			pEffect->m_fSub = fSub;			// ������
			pEffect->Init(pos ,size);		// ������

			// �r���{�[�h�̃^�C�v
			switch (nType)
			{
			case EFFECT_NONE:		// ����
				break;

			case EFFECT_FADE:		// �t�F�[�h�A�E�g
				//pEffect->m_pControl = CControlFade::Create();
				break;

			case EFFECT_BURN:		// ����
				pEffect->m_pControl = CControlRise::Create();
				break;

			case EFFECT_EXPLOSION:	// ����
				pEffect->m_pControl = CControlSpread::Create();
			}
		}
	}
	return pEffect;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �f�o�C�X�E�e�N�X�`���̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_EFFECT_MOVE"));

	// �ʒu�E�T�C�Y�ݒ�
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	// �@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	// �R���g���[���j��
	if (m_pControl)
	{
		m_pControl->Uninit();
		delete m_pControl;
		m_pControl = nullptr;
	}

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	// �^�C�v�ɍ��킹������������
	if (m_pControl)
	{
		m_pControl->Update(this);
	}

	m_pos = CScene::GetPos();
	m_size = CScene::GetSize();
	m_col = CScene::GetCol();

	// �T�C�Y�̏k��
	if (m_size.x > 0.0f && m_size.y > 0.0f)
	{
		m_size.x -= m_fSub;
		m_size.y -= m_fSub;
	}

	// �����x�̃t�F�[�h�A�E�g
	if (m_col.a > 0.0f)
	{
		m_col.a -= 0.001f;
	}

	CScene::SetPos(m_pos);
	CEffect::SetSize(m_size);
	CEffect::SetCol(m_col);

	// �r���{�[�h�̏I��
	if (m_col.a <= 0.0f || (m_size.x <= 0.0f && m_size.y <= 0.0f))
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxView, mtxTrans;											// �v�Z�p�}�g���b�N�X

	CManager::GetInstance()->GetRenderer()->SetZTest(true, pDevice);		// �r���{�[�h�����f���̑O�ɕ`�悳��Ȃ��悤�ɂ���
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(true, pDevice);	// �A���t�@�u�����h�ɂ����Z����
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);	// �A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);							// ���C�g�𖳌���

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);  //�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));  //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�

	pDevice->SetFVF(FVF_VERTEX_3D); //���_�t�H�[�}�b�g�̐ݒ�

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);							// ���C�e�B���O�L��
	CManager::GetInstance()->GetRenderer()->SetZTest(false, pDevice);		// Z�e�X�g����
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);	// ���e�X�g����
	CManager::GetInstance()->GetRenderer()->SetAddSynthesis(false, pDevice);// ���Z��������
}

//*****************************************************************************
// �ʒu�ݒ�
//*****************************************************************************
void CEffect::SetSize(D3DXVECTOR3 size)
{
	m_size = size;
	CScene::SetSize(m_size);

	VERTEX_3D *pVtx; //���_�o�b�t�@�ւ̃|�C���^

	 // ���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_size.x, +m_size.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_size.x, +m_size.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_size.x, -m_size.y, 0.0f);

	//���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock();
}

//*****************************************************************************
// �F�ݒ�
//*****************************************************************************
void CEffect::SetCol(D3DXCOLOR col)
{
	m_col = col;
	CScene::SetCol(m_col);

	// ���_���ւ̃|�C���^
	VERTEX_3D *pVtx;

	// ���_���W�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W���X�V
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
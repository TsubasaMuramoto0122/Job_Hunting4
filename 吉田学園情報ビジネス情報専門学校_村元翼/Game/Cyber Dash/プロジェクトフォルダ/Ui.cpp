//================================================
// UI����
// Author:������
//================================================
#include "ui.h"
#include "control_blink.h"
#include "control_zoom.h"
#include "manager.h"
#include "renderer.h"

//================================================
// �R���X�g���N�^
//================================================
CUi::CUi(OBJTYPE nPriority) : CScene2D(nPriority)
{
	m_pTexture = nullptr;					 // �e�N�X�`���ւ̃|�C���^
	m_pControl = nullptr;					 // �R���g���[���N���X
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // �ʒu
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 // �傫��
	m_UiType = UI_TYPE_NONE;				 // UI�^�C�v
}

//================================================
// �f�X�g���N�^
//================================================
CUi::~CUi()
{

}

//================================================
// ����������
//================================================
HRESULT CUi::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�E�傫���̐ݒ�
	m_pos = pos;
	m_size = size;
	CScene::SetPos(m_pos);
	CScene::SetSize(m_size);

	// �e�N�X�`���擾
	CScene2D::BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));

	// ������
	CScene2D::Init(m_pos, m_size);

	return S_OK;
}

//================================================
// �I������
//================================================
void CUi::Uninit(void)
{
	// �R���g���[���N���X�̔j��
	if (m_pControl)
	{
		m_pControl->Uninit();
		delete m_pControl;
		m_pControl = nullptr;
	}

	// �j��
	CScene2D::Uninit();
}

//================================================
// �X�V����
//================================================
void CUi::Update(void)
{
	// �ʒu�E�傫���E�J���[�擾
	m_pos = GetPos();
	m_size = GetSize();
	m_col = GetCol();

	// �ʒu�E�傫���ݒ�
	if (m_pVtxBuff)
	{
		CScene::SetPos(m_pos);
		CScene::SetSize(m_size);
	}

	// �R���g���[���N���X�̍X�V
	if (m_pControl)
	{
		m_pControl->Update(this);
	}
}


//================================================
// �`�揈��
//================================================
void CUi::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^
	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);				// ���e�X�g�L��

	CScene2D::Draw();																	// �`��

	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);				// ���e�X�g����
}


//================================================
// ��������
//================================================
CUi *CUi::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, UI_TYPE type)
{
	// �I�u�W�F�N�g����
	CUi *pUi = nullptr;
	if (pUi = new CUi)
	{
		// �^�C�v�ʏ���
		switch (type)
		{
		case UI_TYPE_BLINK:	// �_��
			pUi->m_pControl = CControlBlink::Create();
			break;

		case UI_TYPE_ZOOM:	// �Y�[��
			pUi->m_pControl = CControlZoom::Create();
			break;
		}

		pUi->m_UiType = type;								// UI�^�C�v
		pUi->Init(pos, size);								// ����������
		pUi->SetPos(pos, size, false);						// �ʒu�ݒ�
		pUi->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));		// �J���[�ݒ�
	}

	return pUi;
}

//================================================
// UI�̃^�C�v�擾
//================================================
CUi::UI_TYPE CUi::GetUiType(void)
{
	return UI_TYPE();
}
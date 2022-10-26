//===================================================
// �w�i�̏���
// Author:������
//===================================================
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"

//===================================================
// �R���X�g���N�^
//===================================================
CBg::CBg(OBJTYPE nPriority):CScene2D(nPriority)
{
	m_Tex = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �e�N�X�`���ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �w�i�ʒu
	m_bScroll = false;						// �X�N���[������
}

//===================================================
// �f�X�g���N�^
//===================================================
CBg::~CBg()
{

}

//===================================================
// ������
//===================================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �ʒu�擾
	m_pos = pos;

	// �e�N�X�`���擾
	CScene2D::BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));
	
	CScene2D::Init(m_pos,size);		// ������
	CScene2D::SetPos				// �ʒu�ݒ�
	(
		D3DXVECTOR3(0.0f,0.0f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH,0.0f, 0.0f),
		D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f)
	);

	CScene2D::SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �J���[�ݒ�
	return S_OK;
}

//===================================================
// �I��
//===================================================
void CBg::Uninit(void)
{
	CScene2D::Uninit();
}

//===================================================
// �X�V
//===================================================
void CBg::Update(void)
{
	CScene2D::Update();

	if (m_bScroll)
	{
		CScene2D::SetTex(m_Tex.x, m_Tex.y);			// �e�N�X�`���X�V
	}	
}

//===================================================
// �`��
//===================================================
void CBg::Draw(void)
{
	CScene2D::Draw();
}

//===================================================
//�C���X�^���X��������
//===================================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, bool bScrol, CScene::OBJTYPE objtype)
{
	// �C���X�^���X����
	CBg *pBg = nullptr;

	if (pBg = new CBg(objtype))
	{
		pBg->m_bScroll = bScrol;	// ���̎擾
		pBg->Init(pos, size);		// ����������
	}
	return pBg;
}
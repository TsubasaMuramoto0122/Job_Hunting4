//========================================================================
// �|���S���̏���
// Author:������
//========================================================================
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(OBJTYPE nPriority) :CScene(nPriority)
{
	m_pTexture = nullptr;				// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = nullptr;				// ���_�o�b�t�@�̃|�C���^
	m_Scroll = D3DXVECTOR2(0.0f,0.0f);	// �X�N���[���ϐ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CScene::OBJTYPE objtype)
{
	// �C���X�^���X����
	CScene2D *pScene2D = nullptr;

	if (pScene2D = new CScene2D(objtype))
	{
		pScene2D->Init(pos, size);																	// ������
		pScene2D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("TEX_TYPE_FLOOR"));	// �e�N�X�`���擾
		pScene2D->SetPos(pos, size);																// �ʒu�ݒ�
	}
	return pScene2D;
}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// �ʒu�E�T�C�Y�ݒ�
	CScene::SetPos(pos);
	CScene::SetSize(size);

	 // ���_����ݒ�
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL
	);

	// ���_�o�b�t�@�̃|�C���^
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �o�b�t�@�̐���(�ʒu)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`��
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�ۑ�
	for (int nCnt = 0; nCnt < 4 ; nCnt++)
	{
		m_SaveTex[nCnt] = pVtx[nCnt].tex;
	}

	// �A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �|���S���̏I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �|���S���̍X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
// �|���S���̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X�̃|�C���^

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));		
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,																// �`����J�n����ŏ��̒��_�C���f�b�N�X
		2																// �|���S���̐�
	);	
}

//------------------------------------------------------------
// ���_���W�̐ݒ�(�s��`�|���S��ver.)
//------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 vtx0, D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2, D3DXVECTOR3 vtx3)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �o�b�t�@�̐���
	pVtx[0].pos = vtx0;
	pVtx[1].pos = vtx1;
	pVtx[2].pos = vtx2;
	pVtx[3].pos = vtx3;

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// �����`�|���S��(true�ŃX�N���[������)
//------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, bool bScroll)
{
	D3DXVECTOR3 Pos = { 0.0f,0.0f,0.0f };
	Pos = pos;

	CScene::SetPos(Pos);
	CScene::SetSize(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(Pos.x - (scale.x / 2), Pos.y - (scale.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(Pos.x + (scale.x / 2), Pos.y - (scale.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(Pos.x - (scale.x / 2), Pos.y + (scale.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(Pos.x + (scale.x / 2), Pos.y + (scale.y / 2), 0.0f);

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// �����`�|���S��(��]����)
//------------------------------------------------------------
void CScene2D::SetPos(D3DXVECTOR3 pos, D3DXVECTOR3 scale, D3DXVECTOR3 Rotvtx0, D3DXVECTOR3 Rotvtx1, D3DXVECTOR3 Rotvtx2, D3DXVECTOR3 Rotvtx3)
{
	CScene::SetPos(pos);
	CScene::SetSize(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(pos.x + Rotvtx0.x, pos.y + Rotvtx0.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + Rotvtx1.x, pos.y + Rotvtx1.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x + Rotvtx2.x, pos.y + Rotvtx2.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + Rotvtx3.x, pos.y + Rotvtx3.y, 0.0f);

	m_pVtxBuff->Unlock();
}

//--------------------------------------------------------------------
// pos���_���[
//--------------------------------------------------------------------
void CScene2D::SetScalePos(D3DXVECTOR3 pos, D3DXVECTOR3 scale)
{
	// �ʒu�E�T�C�Y�ݒ�
	CScene::SetPos(pos);
	CScene::SetSize(scale);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(pos.x,			pos.y - scale.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + scale.x,	pos.y - scale.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x,			pos.y + scale.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + scale.x,	pos.y + scale.y, 0.0f);

	m_pVtxBuff->Unlock();

}

//------------------------------------------------------------
// �e�N�X�`���̐ݒ�
//------------------------------------------------------------
void CScene2D::SetTex(D3DXVECTOR2 Start, D3DXVECTOR2 End)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(Start.x, Start.y);
	pVtx[1].tex = D3DXVECTOR2(End.x, Start.y);
	pVtx[2].tex = D3DXVECTOR2(Start.x, End.y);
	pVtx[3].tex = D3DXVECTOR2(End.x, End.y);

	// �e�N�X�`�����W�ۑ�
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		m_SaveTex[nCnt] = pVtx[nCnt].tex;
	}

	m_pVtxBuff->Unlock();
}

void CScene2D::SetTex(int nAnim, int nPartU)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)* nAnim,	  0.0f);
	pVtx[1].tex = D3DXVECTOR2(		 (1.0f / nPartU)*(nAnim + 1), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (1.0f / nPartU)* nAnim,	  1.0f);
	pVtx[3].tex = D3DXVECTOR2(		 (1.0f / nPartU)*(nAnim + 1), 1.0f);

	m_pVtxBuff->Unlock();
}

void CScene2D::SetTex(float nPatternX,float nPatternY, float fSizeX, float fSizeY)
{
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPatternX * fSizeX		 , nPatternY * fSizeY);
	pVtx[1].tex = D3DXVECTOR2(nPatternX * fSizeX + fSizeX, nPatternY * fSizeY);
	pVtx[2].tex = D3DXVECTOR2(nPatternX * fSizeX		 , nPatternY * fSizeY + fSizeY);
	pVtx[3].tex = D3DXVECTOR2(nPatternX * fSizeX + fSizeX, nPatternY * fSizeY + fSizeY);

	m_pVtxBuff->Unlock();

}
void CScene2D::SetTex(float fSpeedX, float fSpeedY)
{
	VERTEX_2D *pVtx;

	m_Scroll.x += fSpeedX;
	m_Scroll.y += fSpeedY;

	CScene::SetTex(m_Scroll);

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(m_SaveTex[0].x + m_Scroll.x, m_SaveTex[0].y + m_Scroll.y);
	pVtx[1].tex = D3DXVECTOR2(m_SaveTex[1].x + m_Scroll.x, m_SaveTex[1].y + m_Scroll.y);
	pVtx[2].tex = D3DXVECTOR2(m_SaveTex[2].x + m_Scroll.x, m_SaveTex[2].y + m_Scroll.y);
	pVtx[3].tex = D3DXVECTOR2(m_SaveTex[3].x + m_Scroll.x, m_SaveTex[3].y + m_Scroll.y);

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// �J���[�̐ݒ�
//------------------------------------------------------------
void CScene2D::SetCol(D3DXCOLOR col)
{
	CScene::SetCol(col);

	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[1].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[2].col = D3DXCOLOR(col.r, col.g, col.b, col.a);
	pVtx[3].col = D3DXCOLOR(col.r, col.g, col.b, col.a);

	m_pVtxBuff->Unlock();
}

//------------------------------------------------------------
// �|���S���̈ړ��v�Z
//------------------------------------------------------------
D3DXVECTOR3 CScene2D::Move(D3DXVECTOR3 *pPos, float angle, float speed)
{
	D3DXVECTOR3 move;
	float fAngle = angle;
	float fSpeed = speed;

	move.x = sinf(-fAngle) * fSpeed;
	move.y = cosf(-fAngle) * fSpeed;

	*pPos -= move;

	return move;
}

//=============================================================================
//
// �����̏��� [TextureFont.cpp]
// Author : ������
//
//=============================================================================
#include "TextureFont.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFont::CFont(OBJTYPE nPriority) : CScene2D(nPriority)
{
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CFont::~CFont()
{
}

//=============================================================================
//�@�����̐���
//=============================================================================
CFont *CFont::Create(D3DXVECTOR3 pos, int nfontSize, const char* str, const char* font)
{
	// �C���X�^���X����
	CFont *pFont = new CFont;

	// �����̏���
	if (pFont)
	{
		pFont->m_font = font;					// �t�H���g��
		pFont->m_str = str;						// ����
		pFont->m_pos = pos;						// �ʒu
		pFont->m_nFontSize = nfontSize;			// �t�H���g�T�C�Y
		pFont->Init(pos, { 0.0f,0.0f,0.0f });	// ������
	}

	return pFont;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFont::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	// �V�[��2D������
	CScene2D::Init(pos, size);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//------------------------------------
	// �t�H���g���̍쐬
	//------------------------------------
	HFONT hFont = CreateFont
	(
		m_nFontSize,				// �����̍���(�t�H���g�T�C�Y)
		0,							// �����̉���
		0,							// ��������̕����Ɗp�x�w��(10����1�p�x)
		0,							// �e�����̉�]�p�x
		0,							// �����̑���(FW_BOLD�ő����ɂȂ�)
		FALSE,						// �C�^���b�N��(�Α�)�ɂ��邩
		TRUE,						// ���������邩
		FALSE,						// 
		SHIFTJIS_CHARSET,			// �L�����N�^�Z�b�g
		OUT_TT_ONLY_PRECIS,			// �o�͐��x
		CLIP_DEFAULT_PRECIS,		// �N���b�s���O���x
		PROOF_QUALITY,				// �o�͕i��
		FIXED_PITCH | FF_MODERN,	// �t�H���g�̃s�b�`�E�t�@�~��
		m_font						// �t�H���g��
	);

	//------------------------------------
	// �f�o�C�X�R���e�L�X�g�擾
	//------------------------------------
	// �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
	HDC hdc = GetDC(CManager::GetInstance()->GethWnd());
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	//------------------------------------
	// �����R�[�h�擾
	//------------------------------------
	UINT code = 0;
	if (IsDBCSLeadByte(*m_str))
	{
		code = (BYTE)m_str[0] << 8 | (BYTE)m_str[1];
	}
	else
	{
		code = m_str[0];
	}

	//------------------------------------
	// �r�b�g�}�b�v�擾
	//------------------------------------
	const MAT2 Mat2 = { { 0,1 },{ 0,0 },{ 0,0 },{ 0,1 } };
	GLYPHMETRICS GM;
	TEXTMETRIC TM;
	GetTextMetrics(hdc, &TM);

	DWORD BitMap;	// �t�H���g�̃r�b�g�}�b�v�����i�[����ϐ�
	BitMap = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat2);

	BYTE *ptr = new BYTE[BitMap];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, BitMap, ptr, &Mat2);

	// �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̉��
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	//------------------------------------
	// �r�b�g�}�b�v�擾�I��
	//------------------------------------
	// �e�N�X�`���쐬
	pDevice->CreateTexture
	(
		GM.gmCellIncX,
		TM.tmHeight,
		1, 
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture,
		NULL
	);
	
	// �t�H���g�̃r�b�g�}�b�v�̕��ƍ���
	int iBmp_w, iBmp_h;
	iBmp_w = GM.gmBlackBoxX + (4 - (GM.gmBlackBoxX % 4)) % 4;
	iBmp_h = GM.gmBlackBoxY;

	// �]�����܂ރt�H���g�̈�
	int iFnt_w, iFnt_h;
	iFnt_w = GM.gmCellIncX;
	iFnt_h = TM.tmHeight;

	// �t�H���g�̏����o���ʒu(����)
	int iOfs_x, iOfs_y;
	iOfs_x = GM.gmptGlyphOrigin.x;
	iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;

	// ���l�̒i�K
	int level = 17;
	DWORD Alpha, Color;

	// ���_���̐ݒ�
	VERTEX_2D *pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �o�b�t�@�̐���
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - iFnt_w, m_pos.y + iFnt_h,	0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x - iFnt_w, m_pos.y,			0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x,			m_pos.y + iFnt_h,	0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x,			m_pos.y,			0.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	m_pVtxBuff->Unlock();

	// �e�N�X�`���T�[�t�F�C�X�����b�N
	D3DLOCKED_RECT LockRect;
	m_pTexture->LockRect(0, &LockRect, NULL, D3DLOCK_DISCARD);

	// �e�N�X�`���T�[�t�F�C�X�̏�����
	FillMemory(LockRect.pBits, LockRect.Pitch * iFnt_h, 0);

	// �t�H���g���̏�������
	for (int y = iOfs_y; y < iOfs_y + iBmp_h; y++)
	{
		for (int x = iOfs_x; x < iOfs_x + iBmp_w; x++)
		{
			Alpha = (255 * ptr[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (level - 1);
			Color = 0x00ffffff | (Alpha << 24);
			memcpy((BYTE*)LockRect.pBits + LockRect.Pitch*y + 4 * x, &Color, sizeof(DWORD));
		}
	}

	// �e�N�X�`���T�[�t�F�C�X�A�����b�N
	m_pTexture->UnlockRect(0);
	delete[] ptr;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFont::Uninit(void)
{
	// �V�[��2D�I��
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFont::Update(void)
{
	// �V�[��2D�X�V
	CScene2D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFont::Draw(void)
{
	// �f�o�C�X���擾����
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);

	CManager::GetInstance()->GetRenderer()->SetAlphaTest(true, pDevice);

	// �e�N�X�`���Z�b�g
	pDevice->SetTexture(0, m_pTexture);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); // �|���̃��l�𗘗p

	// �|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,																// �`����J�n����ŏ��̒��_�C���f�b�N�X
		2																// �|���S���̐�
	);

	CManager::GetInstance()->GetRenderer()->SetAlphaTest(false, pDevice);

}
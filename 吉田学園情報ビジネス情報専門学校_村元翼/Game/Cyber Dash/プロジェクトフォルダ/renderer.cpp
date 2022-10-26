//=============================================================================
// renderer.cpp�̍쐬
// Author:������
//=============================================================================
#include "renderer.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "stage.h"
#include "player.h"
#include "DirectInput.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pD3D)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O���s��Ȃ�
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��(�����x���\�ɂ��邩�̐ݒ�)
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 20, 10, 0, 0, TRUE, SHIFTJIS_CHARSET,
	OUT_STRING_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void)
{

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif

	// �f�o�C�X�̔j��
	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	//---------------------------------
	// �S�Ă̍X�V����
	//---------------------------------
	CScene::UpdateAll();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0
	);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//---------------------------------
		// �S�Ă̕`�揈��
		//---------------------------------
		CScene::DrawAll();

#ifdef _DEBUG
		// FPS�\��
		DrawFPS();

		if (CManager::Getmode() == CManager::MODE::GAME)
		{
			DrawTXT();
		}
#endif
		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
//=============================================================================
// Z�e�X�g�̏���
//=============================================================================
void CRenderer::SetZTest(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	else
	{
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//=============================================================================
// �A���t�@�e�X�g�̏���
//=============================================================================
void CRenderer::SetAlphaTest(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	}

	else
	{
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ALPHAREF, 0x00);
	}
}

//=============================================================================
// ���Z����(���ߔ�)�̏���
//=============================================================================
void CRenderer::SetAddSynthesis(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		// ���u�����f�B���O�����Z�����ɐݒ�
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	}
	else
	{
		// ���u�����f�B���O��ʏ퍇���ɐݒ�
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//=============================================================================
// ���Z�����̏���
//=============================================================================
void CRenderer::SetSubSynthesis(bool bUse, LPDIRECT3DDEVICE9 device)
{
	if (bUse)
	{
		//���Z�����̐ݒ�
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		//�ʏ퍇���ɖ߂�
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void CRenderer::DrawFPS(void)
{
	//main����FPS���擾
	int nCntFPS = GetFPS();

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[32];

	wsprintf(str, "FPS:%d\n", nCntFPS);

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, str, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// �e�L�X�g�\��
//=============================================================================
void CRenderer::DrawTXT(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[1024];

	CCamera *pCamera = CManager::GetInstance()->GetCamera(0);
	CPlayer *pPlayer = (CPlayer*)CScene::GetScene(CScene::OBJTYPE_PLAYER);

	//sprintf��<stdio.h>���K�v&wsprintf�ƈ����float��\���ł���
	int nNum = sprintf(&aStr[0], "[�J�����̎��_       :(%.1f �F%.1f �F%.1f)]\n", pCamera->GetPosV().x, pCamera->GetPosV().y, pCamera->GetPosV().z);

	//�g��ꂽ�������̎���aStr�̃A�h���X��n��
	nNum += sprintf(&aStr[nNum], "[�J�����̒����_     :(%.1f �F%.1f : %.1f)]\n", pCamera->GetPosR().x, pCamera->GetPosR().y,pCamera->GetPosR().z);
	nNum += sprintf(&aStr[nNum], "[�J�����̌���       :(%.2f : %.2f : %.2f)]\n", pCamera->GetRot().x, pCamera->GetRot().y, pCamera->GetRot().z);
	nNum += sprintf(&aStr[nNum], "[���_�ƒ����_�̋��� :(%.1f)]\n\n", pCamera->GetLong());
	nNum += sprintf(&aStr[nNum], "*** �J�����ړ� ***\n(�ړ� : W / A / S / D)\n\n");
	nNum += sprintf(&aStr[nNum], "*** ���_���� ***\n(�ړ� : Y / N)(���� : Z / C)\n\n");
	nNum += sprintf(&aStr[nNum], "*** �����_���� ***\n(�ړ� : T / B)(���� : Q / E)\n\n");
	nNum += sprintf(&aStr[nNum], "*** ���Z�b�g ***\nR\n\n");
	nNum += sprintf(&aStr[nNum], "***  �㎋�_  ***\n5\n\n");

	if (pPlayer)
	{
		nNum += sprintf(&aStr[nNum], "[�v���C���[�̈ʒu     :(%.1f, %.1f, %.1f)]\n", pPlayer->GetPos().x, pPlayer->GetPos().y, pPlayer->GetPos().z);
		nNum += sprintf(&aStr[nNum], "[�v���C���[�̉�]     :(%.2f, %.2f, %.2f)]\n", pPlayer->GetRot().x, pPlayer->GetRot().y, pPlayer->GetRot().z);
		nNum += sprintf(&aStr[nNum], "[�v���C���[�̑��x     :(%.1f, %.1f, %.1f)]\n", pPlayer->GetSpeed().x, pPlayer->GetSpeed().y, pPlayer->GetSpeed().z);
		nNum += sprintf(&aStr[nNum], "[�W�����v���Ă��邩   :(%s)]\n\n", pPlayer->GetJump() ? "true" : "false");
	}

	nNum += sprintf(&aStr[nNum], "[�G�t�F�N�g��   :(%d)]\n", CScene::GetNumObj(CScene::OBJTYPE_EFFECT));
	nNum += sprintf(&aStr[nNum], "[�r���{�[�h��   :(%d)]\n", CScene::GetNumObj(CScene::OBJTYPE_BILLBOARD));
	nNum += sprintf(&aStr[nNum], "[���f����   :(%d)]\n\n", CScene::GetNumObj(CScene::OBJTYPE_MODEL));
	nNum += sprintf(&aStr[nNum], "[DInput���X�e�B�b�N���͏��   :(%s)]\n", CDInput::GetStateL());
	nNum += sprintf(&aStr[nNum], "[DInput�E�X�e�B�b�N���͏��   :(%s)]\n\n", CDInput::GetStateR());

	nNum += sprintf(&aStr[nNum], "[B�{�^����K�L�[��F�L�[�Ŕ��e���o��]\n\n");

	m_pFont->DrawText(NULL, aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

#endif
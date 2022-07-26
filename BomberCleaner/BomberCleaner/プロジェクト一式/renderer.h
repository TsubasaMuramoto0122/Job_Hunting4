//=================================================
// renderer.h�̍쐬
// Author:������
//=================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//=================================================
// �C���N���[�h�t�@�C��
//=================================================
#include "main.h"

//=================================================
// �����_���[�N���X�̐���
//=================================================
class CRenderer
{
public:
	CRenderer();												// �R���X�g���N�^
	~CRenderer();												// �f�X�g���N�^

	HRESULT Init(HWND hWnd, bool bWindow);						// �S�̂̏�����
	void Uninit(void);											// �S�̂̏I��
	void Update(void);											// �S�̂̍X�V
	void Draw(void);											// �S�̂̕`��

	void SetAddSynthesis(bool bUse, LPDIRECT3DDEVICE9 device);	// ���Z����
	void SetSubSynthesis(bool bUse, LPDIRECT3DDEVICE9 device);	// ���Z����
	void SetAlphaTest(bool bUse, LPDIRECT3DDEVICE9 device);		// �A���t�@�e�X�g
	void SetZTest(bool bUse, LPDIRECT3DDEVICE9 device);			// Z�e�X�g

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// �f�o�C�X�擾

private:
#ifdef _DEBUG
	void DrawFPS(void);											// FPS�̕`��
	void DrawTXT(void);											// �f�o�b�O�����̕`��
	LPD3DXFONT	m_pFont = nullptr;								// �t�H���g�ւ̃|�C���^
#endif

	LPDIRECT3D9				m_pD3D = nullptr;					// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice = nullptr;				// Device�I�u�W�F�N�g(�`��ɕK�v)
};

#endif
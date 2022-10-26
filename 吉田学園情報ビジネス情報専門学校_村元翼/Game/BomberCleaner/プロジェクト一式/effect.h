//=============================================================================
//
// �G�t�F�N�g [effect.h]
// Author : ������
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define EXPLOSION_NUM	(128)								// �����G�t�F�N�g��
#define EXPLOSION_SUB	(0.5f)								// �����G�t�F�N�g�k����
#define EXPLOSION_SIZE	(D3DXVECTOR3(50.0f,50.0f,0.0f))		// �����G�t�F�N�g�T�C�Y
#define EXPLOSION_COL	(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))	// �����G�t�F�N�g�J���[

#define IGNITION_SUB	(0.2f)								// ���΃G�t�F�N�g�k����
#define IGNITION_SIZE	(D3DXVECTOR3(5.0f, 5.0f, 0.0f))		// ���΃G�t�F�N�g�T�C�Y
#define IGNITION_COL	(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))	// ���΃G�t�F�N�g�J���[

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CControl;

//*****************************************************************************
// �G�t�F�N�g�N���X�̒�`
//*****************************************************************************
class CEffect : public CScene
{
public:
	//---------------------------------------
	// �G�t�F�N�g�^�C�v�̗񋓌^�\����
	//---------------------------------------
	typedef enum
	{
		EFFECT_NONE = 0,
		EFFECT_FADE,
		EFFECT_BURN,
		EFFECT_EXPLOSION,
		EFFECT_MAX

	}EFFECT_TYPE;

	CEffect(OBJTYPE nPriority = OBJTYPE_EFFECT);	// �R���X�g���N�^
	~CEffect();										// �f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);	// ����������
	void Uninit(void);									// �I������
	void Update(void);									// �X�V����
	void Draw(void);									// �`�揈��

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, float fSub,int nType);		// ����

	//---------------------------------------
	// Setter & Getter
	//---------------------------------------
	void SetSize(D3DXVECTOR3 size);
	void SetCol(D3DXCOLOR col);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }

private:
	LPDIRECT3DTEXTURE9		m_pTexture = nullptr;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = nullptr;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3				m_pos;					// �ʒu
	D3DXVECTOR3				m_move;					// �ړ���
	D3DXVECTOR3				m_size;					// �T�C�Y
	D3DXCOLOR				m_col;					// �F
	D3DXMATRIX				m_mtxWorld;				// ���[���h�}�g���b�N�X
	CControl				*m_pControl;			// �R���g���[���N���X
	float					m_fSub;					// ������

};
#endif //!_EFFECT_H_

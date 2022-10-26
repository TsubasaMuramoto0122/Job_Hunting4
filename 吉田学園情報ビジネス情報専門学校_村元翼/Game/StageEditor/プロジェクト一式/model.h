//========================================================
// ���f���̏���
// �쐬�ҁF������
//========================================================
#ifndef _MODEL_H
#define _MODEL_H

//========================================================
// �C���N���[�h�t�@�C��
//========================================================
#include "main.h"
#include "scene.h"

//========================================================
// �}�N����`
//========================================================
#define MAX_MODEL_VTX		(8)		// ���f���̒��_��
#define SURFACE_NUM			(6)		// �L���[�u�̖̂ʂ̐�

//========================================================
// �O���錾
//========================================================

//========================================================
// �N���X�̒�`
//========================================================
class CModel
{
public:
	//---------------------------------------
	// �ǂ�����Փ˂��Ă��邩
	//---------------------------------------
	typedef enum
	{
		FROM_UP = 0,
		FROM_DOWN,
		FROM_RIGHT,
		FROM_LEFT,
		FROM_FRONT,
		FROM_BACK,
		FROM_MAX

	}HIT_FROM;

	//---------------------------------------
	// �Փ˔���̎��
	//---------------------------------------
	typedef enum
	{
		TYPE_DOT = 0,
		TYPE_LINE,
		TYPE_SPHERE,
		TYPE_CUBE,
		TYPE_MAX

	}HIT_TYPE;

	//---------------------------------------
	// ���f���̊e���_�̕ۑ��p�\����
	//---------------------------------------
	typedef struct
	{
		D3DXVECTOR3 vtx,vtxWorld, rot;					// ���_�E��]
		D3DXMATRIX mtxWorld;							// ���[���h�}�g���b�N�X
	}ModelVtx;

	CModel();											// �R���X�g���N�^
	~CModel();											// �f�X�g���N�^
	void Init(int nXType);								// ������
	void Uninit(void);									// �I��
	void Update(void);									// �X�V
	void Draw(void);									// �`��

	// ����
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nXType,bool bCollision = true);

	//---------------------------------------
	// �ݒ�E�擾�֐�
	//---------------------------------------
	void SetParent(CModel *pModel)				{ m_pParent = pModel; }			// �e�̃��f������
	void SetPos(D3DXVECTOR3 pos)				{ m_pos = pos; }				// �ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot)				{ m_rot = rot; }				// ��]�ݒ�
	void SetScale(D3DXVECTOR3 scale)			{ m_scale = scale; }			// �X�P�[���ݒ�
	void SetLayerPos(D3DXVECTOR3 pos)			{ m_layerpos = pos; }			// ���C���[�ʒu�ݒ�
	void SetLayerRot(D3DXVECTOR3 rot)			{ m_layerrot = rot; }			// ���C���[��]�ݒ�
	void SetEmissive(D3DCOLORVALUE Emissive)	{ m_SaveEmissive = Emissive; }	// �g�U�ݒ�
	void SetDraw(bool bDraw)					{ m_bDraw = bDraw; }			// �`�攻��
	void SetNumParent(int Parent)				{ m_nNumParent = Parent; }		// �e�ԍ��ݒ�

	int			GetXType()				{ return m_nXType; }				// X�^�C�v�擾			
	D3DXMATRIX	GetMatrix()				{ return m_mtxWorld; }				// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetPos()				{ return m_pos; }					// �ʒu�擾
	D3DXVECTOR3 GetRot()				{ return m_rot; }					// ��]�擾
	D3DXVECTOR3 GetLayerPos()			{ return m_layerpos; }				// ���C���[�ʒu�擾
	D3DXVECTOR3 GetLayerRot()			{ return m_layerrot; }				// ���C���[��]�擾
	D3DXVECTOR3 GetMinPos()				{ return m_vtxMin; }				// ���_�ŏ��ʒu�擾
	D3DXVECTOR3 GetMaxPos()				{ return m_vtxMax; }				// ���_�ő�ʒu�擾
	D3DXVECTOR3 GetSize()				{ return m_size; }					// �T�C�Y�擾
	D3DXVECTOR3 GetSizeHalf()			{ return m_size / 2; }				// �T�C�Y���a�擾
	D3DXVECTOR3 GetScale()				{ return m_scale; }					// �X�P�[���擾
	int			GetNumParent(void)		{ return m_nNumParent; }			// �e�ԍ��擾
	ModelVtx	GetVtx(int nNumVtx)		{ return m_vtx[nNumVtx]; }			// �e���f���̒��_���擾
	D3DXMATRIX	GetMtxWorld(void)		{ return m_mtxWorld; }				// ���[���h�}�g���b�N�X�擾
	D3DXMATRIX *GetVtxMtxWorld(void)	{ return &m_vtx[0].mtxWorld; }		// ���_���[���h�}�g���b�N�X�擾
	bool		GetHitBool()			{ return m_bHit; }					// �������������擾

private:
	LPD3DXMESH					m_pMesh;					// ���b�V���i���_���j�ւ̃|�C���^
	LPD3DXBUFFER				m_pBuffMat;					// �}�e���A���i�ގ����j�ւ̃|�C���^
	vector<LPDIRECT3DTEXTURE9>	m_pTexture;					// �e�N�X�`��
	DWORD						m_nNumMat;					// �}�e���A���̐�
	D3DXVECTOR3					m_pos,m_layerpos;			// ���f���̈ʒu�i�I�t�Z�b�g�j
	D3DXVECTOR3					m_scale, m_size;			// ���f���̑傫���A�K��(�䗦)
	D3DXVECTOR3					m_rot, m_layerrot;			// ����
	ModelVtx					m_vtx[MAX_MODEL_VTX];		// �e���_�̏��i�[�\����
	D3DXVECTOR3					m_vtxMin, m_vtxMax;			// ���f���̍ŏ��l�A�ő�l
	D3DXMATRIX					m_mtxWorld;					// ���[���h�}�g���b�N�X
	D3DXMATERIAL				*m_pMat;					// �}�e���A���ւ̃|�C���^
	D3DXCOLOR					m_SaveEmissive;				// �����F�̕ۑ��p�ϐ�
	DWORD						m_sizeFVF;					// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE						*m_pVtxBuff;				// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxData;					// ���_�o�b�t�@�ւ̃|�C���^�̏��

	int							m_nXType;					// X�t�@�C���̎��
	int							m_nNumVtx;					// ���_�̐�
	int							m_nNumParent;				// ���݂̐e���f���̔ԍ�
	CModel						*m_pParent;					// �e���f���̃|�C���^
	bool						m_bDraw;					// �`�悷�邩
	bool						m_bDoOnce;					// ��񂾂��ʂ�
	bool						m_bCollision;				// �����蔻����s����
	bool						m_bHit;						// ����������
};


#endif

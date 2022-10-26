//==================================================
// scene.h�̍쐬
// Author:������
//==================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

//==================================================
// �I�u�W�F�N�g�N���X�̐���
//==================================================
class CScene
{
public:
	//-----------------------------
	// �I�u�W�F�N�g�^�C�v�\����
	//-----------------------------
	typedef enum
	{
		OBJTYPE_NONE = 0,		// ����
		OBJTYPE_BG,				// �w�i
		OBJTYPE_NUMBER,			// �i���o�[
		OBJTYPE_POLYGON2D,		// 2D�|���S��
		OBJTYPE_POLYGON3D,		// 3D�|���S��
		OBJTYPE_MESH,			// ���b�V��
		OBJTYPE_MODEL,			// ���f��
		OBJTYPE_BOMB,			// ���f�����e
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_EFFECTMESH,		// �G�t�F�N�g���b�V��
		OBJTYPE_BILLBOARD,		// �r���{�[�h
		OBJTYPE_SHADOW,			// �V���h�E
		OBJTYPE_UI,				// UI
		OBJTYPE_FONT,			// �t�H���g
		OBJTYPE_PAUSE,			// �|�[�Y
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_MAX				

	}OBJTYPE;

	//-----------------------------
	// ���f���^�C�v�\����
	//-----------------------------
	typedef enum
	{
		MODTYPE_NORMAL = 0,			// �ʏ�
		MODTYPE_BOMB,				// ���e
		MODTYPE_BOMBSPAWNER,		// ���e�X�|�i�[
		MODTYPE_TARGET,				// �Ώ�
		MODTYPE_PLAYER,				// �v���C���[
		MODTYPE_MAX					

	}MODTYPE;

	//------------------------------------
	// ���b�V���^�C�v��
	//------------------------------------
	typedef enum
	{
		MESH_FIELD = 0,				// �t�B�[���h
		MESH_WALL,					// �E�H�[��
		MESH_CYLINDER,				// �V�����_�[
		MESH_SPHERE,				// �X�t�B�A
		MESH_MAX

	}MESH_TYPE;

	CScene();														// �f�t�H���g�R���X�g���N�^
	explicit CScene(OBJTYPE nPriority);								// �I�[�o�[���[�h�R���X�g���N�^
	virtual ~CScene();												// �f�X�g���N�^

	// �������z�֐�
	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size) = 0;	// �|���S���̏�����
	virtual void Uninit(void) = 0;									// �|���S���̏I��
	virtual void Update(void) = 0;									// �|���S���̍X�V
	virtual void Draw(void) = 0;									// �|���S���̕`��

	static void UpdateAll();										// �S�Ẵ|���S���̍X�V
	static void DrawAll();											// �S�Ẵ|���S���̕`��
	static void ReleaseAll();										// �S�Ẵ|���S���̉��

	//-----------------------------
	// �ݒ�E�擾�֐�
	//-----------------------------
	void SetPos(D3DXVECTOR3 pos)			{ m_pos = pos; }					// �ʒu�̐ݒ�
	void SetPosOld(D3DXVECTOR3 posOld)		{ m_posOld = posOld; }				// �Ō�̈ʒu�̐ݒ�
	void SetRot(D3DXVECTOR3 rot)			{ m_rot = rot; }					// ��]�̐ݒ�
	void SetSize(D3DXVECTOR3 size)			{ m_size = size; }					// �傫���̐ݒ�
	void SetObjType(OBJTYPE objType)		{ m_objType = objType; }			// �I�u�W�F�N�g�^�C�v�̐ݒ�
	void SetTex(D3DXVECTOR2 tex)			{ m_Tex = tex; }					// �e�N�X�`���̑傫���ݒ�
	void SetCol(D3DXCOLOR col)				{ m_col = col; }					// �J���[�̐ݒ�
	void SetModelType(MODTYPE ModTp)		{ m_ModelType = ModTp; }			// ���f���^�C�v�ݒ�

	static int	GetNumObj(int nPriority)	{ return m_nNumAll[nPriority]; }		// �I�u�W�F�N�g���̎擾
	D3DXVECTOR3 GetPos(void)				{ return m_pos; }						// �ʒu�擾
	D3DXVECTOR3 GetPosOld(void)				{ return m_posOld; }					// �Ō�̈ʒu�擾
	D3DXVECTOR3 GetRot(void)				{ return m_rot; }
	D3DXVECTOR3 GetSize(void)				{ return m_size; }						// �傫���̎擾
	D3DXVECTOR2 GetTex(void)				{ return m_Tex; }						// �e�N�X�`���̑傫���擾
	OBJTYPE		GetObjType(void)			{ return m_objType; }					// �I�u�W�F�N�g�^�C�v�̎擾
	D3DXCOLOR	GetCol(void)				{ return m_col; }						// �J���[�̎擾
	MODTYPE		GetModelType()				{ return m_ModelType; }					// ���f���^�C�v�̎擾
	bool		GetDeath()					{ return m_bDeath; }					// ���S����̎擾

	static CScene *GetScene(const int nPriority)	{ return m_pTop[nPriority]; }	// �V�[���擾
	static CScene *GetSceneNext(CScene *pTop)		{ return pTop->m_pNext; }		// ���V�[���擾

protected:
	void Release(); 																// ���g�̔j��

private:
	static CScene	*m_pTop[OBJTYPE_MAX];					// �擪�̃I�u�W�F�N�g�̃|�C���^(�I�u�W�F�N�g�̐���)
	static CScene	*m_pCur[OBJTYPE_MAX];					// ����(��Ԍ��)�̃I�u�W�F�N�g�̃|�C���^(�I�u�W�F�N�g�̐���)
	CScene			*m_pPrev;								// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene			*m_pNext;								// ���̃I�u�W�F�N�g�ւ̃|�C���^

	// ����ǉ����邱�ƂŁ���m_apScene(�z��)���K�v�Ȃ��Ȃ�
	// static CScene *m_apScene[MAX_POLYGON];				// �I�u�W�F�N�g�̊i�[�ꏊ
	static int		m_nNumAll[OBJTYPE_MAX];					// �I�u�W�F�N�g�̍ő吔(�I�u�W�F�N�g�̐���)

	int				m_nPriority;							// �D�揇��
	bool			m_bDeath;								// ���S�t���O
	D3DXVECTOR3		m_pos,m_posOld;							// �ʒu
	D3DXVECTOR3		m_rot;									// ��]
	D3DXVECTOR3		m_size;									// �X�P�[��
	D3DXCOLOR		m_col;									// �J���[
	D3DXVECTOR2		m_Tex;									// �e�N�X�`��
	OBJTYPE			m_objType;								// �I�u�W�F�N�g�^�C�v�̕ϐ�
	MODTYPE			m_ModelType;							// ���f���̎��
};

#endif
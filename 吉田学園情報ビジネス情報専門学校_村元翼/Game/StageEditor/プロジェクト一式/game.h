//============================================================
// �X�e�[�W�����ʂ̏���
// Author:������
//============================================================
#ifndef _GAME_H_
#define _GAME_H_
#include "main.h"

//============================================================
// �O���錾
//============================================================
class CTime;
class CScore;
class CMeshField;
class CMeshCylinder;
class CMeshSphere;
class CStage;
class CUi;
class CTimerCount;
class CModel_Spawner;
class CStringManager;

//============================================================
// �N���X�̐���
//============================================================
class CGame
{
public:
	//-----------------------------
	// �I�u�W�F�N�g�^�C�v�\����
	//-----------------------------
	typedef enum
	{
		OBJ_MODEL = 0,
		OBJ_MESHFIELD,
		OBJ_MAX
	}OBJ;

	//-----------------------------
	// ���͍��W�̃��[�h�ؑ֍\����
	//-----------------------------
	typedef enum
	{
		INPUT_POSX = 0,
		INPUT_POSY,
		INPUT_POSZ,
		INPUT_MAX
	}INPUT_POS;

	CGame();				// �R���X�g���N�^
	~CGame();				// �f�X�g���N�^
	HRESULT Init(void);		// ������
	void Uninit(void);		// �I��
	void Update(void);		// �X�V
	void Draw(void);		// �`��

	void SetUiCreate(const char* TexStr = "",float fSizeMulti = 1);		// UI�̐���
	void SetUiDelete(void);												// UI�̍폜

	void SetMessage(const char* sMessage);																// ���b�Z�[�W�̐ݒ�(����)
	void MatchSquare(float &fEven,const int &nMulti, const float &fShift);								// �}�X�ڂɍ��킹��֐�
	void SelectLoop(int &nNow,const int &nMin, const int &nMax ,const int &nKey1,const int nKey2);		// �I�����̐؂�ւ�(���[�v)
	void InputPos(float &fpos);																			// �ʒu�̓���
	void ObjDelete(void);																				// �I�u�W�F�N�g�폜
	void ObjCreate(int nObjtype);																		// �I�u�W�F�N�g����
	bool Save(const char* sSaveName);																	// �Z�[�u�֐�
	bool Load(const char* sLoadName);																	// ���[�h�֐�


	//--------------------------
	// �ݒ�E�擾�֐�
	//--------------------------
	CModel_Spawner	*GetModelPrev()	{ return m_pModelPrev; }				// ���f���v���r���[�̎擾
	CMeshField		*GetFieldPrev()	{ return m_pFieldPrev; }				// �t�B�[���h�v���r���[�̎擾

	D3DXVECTOR3	GetInputPos()		{ return m_Inputpos; }					// ���͗p���W�̎擾
	bool		GetInputMode()		{ return m_bInputMode; }				// ���̓��[�h�̎擾
	int			GetObjtype()		{ return m_nObjType; }					// �I�u�W�F�N�g�^�C�v�̎擾
	int			GetSelect()			{ return m_nSelect; }					// �I�����̎擾
	int			GetMaxSelect()		{ return m_nSelectMax; }				// �I���ő�l�̎擾
	int			GetNumModel()		{ return m_vModel.size(); }				// ���f���������̎擾
	int			GetNumField()		{ return m_vMeshField.size(); }			// �t�B�[���h�������̎擾	

private:
	CUi							*m_pUI;				// UI�N���X
	CMeshField					*m_pField;			// ���b�V���t�B�[���h�N���X
	int							m_nFrame;			// �t���[���J�E���g�ϐ�
	int							m_nObjType;			// �I�u�W�F�N�g�̎��
	int							m_nSelectMax;		// ���f���ő吔
	int							m_nSelect;			// ���ݑI������Ă��郂�f���̔ԍ�
	int							m_nPosMode;			// ���W���[�h
	static int					m_nMultiX;			// �t�B�[���h�T�C�Y�Ɋ|����{��X
	static int					m_nMultiZ;			// �t�B�[���h�T�C�Y�Ɋ|����{��Z
	static float				m_fEvenX;			// ���b�V�����������̏ꍇ�Ɏg���ϐ�X
	static float				m_fEvenZ;			// ���b�V�����������̏ꍇ�Ɏg���ϐ�Z
	static bool					m_bInputMode;		// ���̓��[�h����

	D3DXVECTOR3					m_Inputpos;			// ���͈ʒu
	CStringManager				*m_pString;			// ���b�Z�[�W
	CModel_Spawner				*m_pModelPrev;		// ���f���v���r���[
	CMeshField					*m_pFieldPrev;		// ���b�V���t�B�[���h�v���r���[
	vector<CModel_Spawner*>		m_vModel;			// ���f��vector�N���X
	vector<CMeshField*>			m_vMeshField;		// ���b�V���t�B�[���hvector�N���X
};
#endif